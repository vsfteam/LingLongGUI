/*
 * Copyright 2021 Ou Jianbo 59935554@qq.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 * additional license
 * If you use this software to write secondary development type products,
 * must be released under GPL compatible free software license or commercial license.
*/

#include "LL_General.h"
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
#include "LL_Linked_List.h"
#include "LL_Window.h"
#include "LL_Background.h"


uint8_t llUtf8ToUnicode(uint8_t* unicode, uint8_t *utf8)
{
    uint8_t nBytes = 0;

    if (0 == (*utf8 & 0x80))
    {
        /*
         * single-byte char
         */
        nBytes = 1;
        unicode[0] = *utf8;
    }
    else
    {
        /*
         * 3-byte char (chinese char)
         */
        if ( (*utf8 & 0xf0) == 0xe0 )
        {
            nBytes  = 3;
            unicode[0] = ((utf8[0] & 0x0f) <<4) + ((utf8[1] & 0x3c) >>2);
            unicode[1] = ((utf8[1] & 0x03) <<6) + (utf8[2] & 0x3f);
        }
        else
        {
            //            debug("ERROR: utf-8 to unicode, nBytes !=3\n");
            nBytes = 0;
            unicode[0] = '?';
        }
    }
    return nBytes;
}

/*******************************************************************************
 * @name     :llStringCompare
 * @brief    :比较两个字符串是否相同
 * @param    :str1 str2
 * @return   :bool
 * @version  :V0.1
 * @author   :
 * @date     :2018.5.10
 * @details  :比较字符串和长度是否都相同
*******************************************************************************/
bool llStringCompare(uint8_t* str1,uint8_t* str2)
{
    uint32_t len1,len2;

    len1=strlen((char*)str1);
    len2=strlen((char*)str2);

    if(len1==len2)
    {
        if(memcmp(str1,str2,len1)==0)
        {
            return true;
        }
    }
    return false;
}

uint8_t* llStrcat(uint8_t* str1,uint8_t* str2)
{
    uint8_t *outString;
    outString=(uint8_t *)llMalloc(strlen((const char*)str1)+strlen((const char*)str2));
    if(outString)
    {
        strcpy((char*)outString,(const char*)str1);
        strcat((char*)outString,(const char*)str2);
    }

    return outString;
}

/**
 * @brief 判断两个矩形是否重叠
 * @param rc1 第一个矩阵的位置
 * @param rc2 第二个矩阵的位置
 * @return 两个矩阵是否重叠（边沿重叠，也认为是重叠）
 */
bool llRectIsOverlap(llGeometry rc1, llGeometry rc2)
{
    if((rc1.x + rc1.width-1)>=rc2.x &&
            (rc2.x + rc2.width-1)>=rc1.x &&
            (rc1.y + rc1.height-1)>=rc2.y &&
            (rc2.y + rc2.height-1)>=rc1.y )
    {
        return true;
    }
    return false;
}

/**
 * @brief 判断一个矩形是否在另一个矩形内
 * @param bigRect 大矩阵的位置
 * @param smallRect 小矩阵的位置
 * @return 小矩阵是否在大矩形内（包括边沿重叠）
 */
bool llRectIsIn(llGeometry bigRect, llGeometry smallRect)
{
    if((smallRect.x>=bigRect.x) &&
            (smallRect.y>=bigRect.y) &&
            ((smallRect.x+smallRect.width-1)<=(bigRect.x+bigRect.width-1)) &&
            ((smallRect.y+smallRect.height-1)<=(bigRect.y+bigRect.height-1)) )
    {
        return true;
    }
    return false;
}

/**
 * @brief 判断一个矩形是否在另一个矩形内
 * @param bigRect 大矩阵的位置
 * @param smallRect 小矩阵的位置
 * @return 小矩阵是否在大矩形内（包括边沿重叠）
 */
bool llRectIsFullIn(llGeometry bigRect, llGeometry smallRect)
{
    if((smallRect.x>=bigRect.x) &&
            (smallRect.y>=bigRect.y) &&
            ((smallRect.x+smallRect.width-1)<=(bigRect.x+bigRect.width-1)) &&
            ((smallRect.y+smallRect.height-1)<=(bigRect.y+bigRect.height-1)) )
    {
        return true;
    }
    return false;
}

bool llPointInRect(llPoint point, llGeometry rc)
{
    if((point.x>=rc.x)&&(point.x<=(rc.x+rc.width-1))&&(point.y>=rc.y)&&(point.y<=(rc.y+rc.height-1)))
    {
        return true;
    }
    return false;
}

/***************************************************************************//**
 * @fn         bool llRectIntersect(const llGeometry rect1, const llGeometry rect2,llGeometry * outRect)
 * @brief      两个矩形的相交区域
 * @param      rect1    矩形1
 *             rect2    矩形2
 *             outRect  返回矩形
 * @return     bool     是否有相交
 * @version    V0.1
 * @date       2020-11-04
 * @details    
 ******************************************************************************/
bool llRectIntersect(const llGeometry rect1, const llGeometry rect2,llGeometry * outRect)
{
    int16_t outX2,outY2;
    
    /* Get the smaller area from 'a1_p' and 'a2_p' */
    outRect->x = MAX(rect1.x, rect2.x);
    outRect->y = MAX(rect1.y, rect2.y);
    outX2 = MIN(rect1.x+rect1.width-1, rect2.x+rect2.width-1);
    outY2 = MIN(rect1.y+rect1.height-1, rect2.y+rect2.height-1);

    /*If x1 or y1 greater then x2 or y2 then the areas union is empty*/
    bool union_ok = true;
    if((outRect->x > outX2) || (outRect->y > outY2))
    {
        union_ok = false;
    }

    outRect->width=MAX(outX2,outRect->x)-MIN(outX2,outRect->x)+1;
    outRect->height=MAX(outY2,outRect->y)-MIN(outY2,outRect->y)+1;

    return union_ok;
}

///**
//   * @description 射线法判断点是否在多边形内部
//   * @param {Object} p 待判断的点，格式：{ x: X坐标, y: Y坐标 }
//   * @param {Array} poly 多边形顶点，数组成员的格式同 p
//   * @return {String} 点 p 和多边形 poly 的几何关系
//   */
//bool rayCasting(llPoint p, llPoint* poly,uint16_t length)
//{
//    uint16_t px = p.x;
//    uint16_t  py = p.y;
//    bool    flag = false  ;
//
//    int i,j,l,sx,sy,tx,ty,x;
//
//    for( i = 0, l = length, j = l - 1; i < l; j = i, i++)
//    {
//       sx = poly[i].x;
//          sy = poly[i].y;
//          tx = poly[j].x;
//          ty = poly[j].y;
//
//      // 点与多边形顶点重合
//      if((sx == px && sy == py) || (tx == px && ty == py))
//       {
//        return true;
//      }
//
//      // 判断线段两端点是否在射线两侧
//      if((sy < py && ty >= py) || (sy >= py && ty < py)) {
//        // 线段上与射线 Y 坐标相同的点的 X 坐标
//        x = sx + (py - sy) * (tx - sx) / (ty - sy) ;
//
//        // 点在多边形的边上
//        if(x == px)
//            {
//          return true;
//        }
//
//        // 射线穿过多边形的边界
//        if(x > px)
//            {
//          flag = !flag;
//        }
//      }
//    }
//
//    // 射线穿过多边形边界的次数为奇数时点在多边形内
//    return flag;
//}

////任意凸4边型
//bool llPointInRect(int16_t x, int16_t y,int16_t x1,int16_t y1,int16_t x2,int16_t y2,int16_t x3,int16_t y3,int16_t x4,int16_t y4)
//{
////    llPoint point[5];
////    point[0].x=x;
////    point[0].y=y;
////    point[1].x=x1;
////    point[1].y=y1;
////    point[2].x=x2;
////    point[2].y=y2;
////    point[3].x=x3;
////    point[3].y=y3;
////    point[4].x=x4;
////    point[4].y=y4;
////    return rayCasting(point[0], &point[1],4);

//	int a, b, c,d;//分别存四个向量的计算结果;
//	a = (x2 - x1)*(y - y1) - (y2 - y1)*(x - x1);
//    b = (x3 - x2)*(y - y2) - (y3 - y2)*(x - x2);
//    c = (x4 - x3)*(y - y3) - (y4 - y3)*(x - x3);
//    d = (x1 - x4)*(y - y4) - (y1 - y4)*(x - x4);
//    if((a > 0 && b > 0 && c > 0 && d > 0) || (a < 0 && b < 0 && c < 0 && d < 0))
//	{
//            return 1;
//    }
//	else
//	{
//		return 0;
//	}
//}

void llRgbaFillMultipleColors(llGeometry geometry,uint8_t *color)
{
    uint16_t i,j;
    uint16_t x,y,width,height;
    uint32_t rgba;
    uint8_t r,g,b,a;

    x=geometry.x;
    y=geometry.y;
    width=geometry.width;
    height=geometry.height;

    for(j=0; j<height; j++)
    {
        for(i=0; i<width; i++)
        {
            a=color[(j*width+i)*4+3];
            if(a!=0)//判断不是透明
            {
                //                Alpha=(float)a/256;
                r=color[(j*width+i)*4];//*Alpha;
                g=color[(j*width+i)*4+1];//*Alpha;
                b=color[(j*width+i)*4+2];//*Alpha;
                rgba=RGB_CONVERT(r,g,b);
                llSetPoint(x+i,y+j,rgba);
            }
        }
    }
}


void llRgbFillMultipleColors(llGeometry geometry,uint8_t *color)
{
    uint16_t i,j;
    int16_t x,y,width,height;
    uint32_t rgb;
    uint8_t r,g,b;

    x=geometry.x;
    y=geometry.y;
    width=geometry.width;
    height=geometry.height;


    for(j=0; j<height; j++)
    {
        for(i=0; i<width; i++)
        {
            r=color[(j*width+i)*3];
            g=color[(j*width+i)*3+1];
            b=color[(j*width+i)*3+2];
            rgb=RGB_CONVERT(r,g,b);
            llSetPoint(x+i,y+j,rgb);
        }
    }
}

void llFillLineMultipleTransparent5515Colors(int16_t x0,int16_t x1,int16_t y,llColor *color)
{
    int16_t startX,endX;
    int16_t i;
    llColor *pos;
    bool isWait=true;

    for(i=x0; i<=x1; i++)
    {
        if(isWait)
        {
            if((((*color>>5)&1)!=0)&&(*color!=0x20))
            {
                startX=i;
                pos=color;
                isWait=false;
            }
        }
        else
        {
            if(((*color>>5)&1)==0)
            {
                endX=i-1;
                isWait=true;
                llFillMultipleColors(startX,y,endX,y,pos);
            }
        }
        color++;
    }
    if(isWait==false)
    {
        llFillMultipleColors(startX,y,x1,y,pos);
    }
}

void llFillLineMultipleTransparent8565Colors(int16_t x0,int16_t x1,int16_t y,uint32_t *alphaColor)
{
    int16_t startX,endX;
    int16_t i;
    llColor bColor,outColor;
    uint8_t r1,g1,b1,r2,g2,b2,alpha;
    uint32_t r,g,b;
    bool isWait=true;
    

    for(i=x0; i<=x1; i++)
    {
        alpha=(uint8_t)(*alphaColor>>16);
        if(alpha!=0)
        {
        bColor=llGetPoint(i,y);
        llGeometryGetR_G_B(bColor,&r1,&g1,&b1);
        llGeometryGetR_G_B((*alphaColor)&0xffff,&r2,&g2,&b2);

//            R(C)=(1-alpha)*R(B) + alpha*R(A)
//            G(C)=(1-alpha)*G(B) + alpha*G(A)
//            B(C)=(1-alpha)*B(B) + alpha*B(A)
            
            r = (255-alpha)*r1 + alpha*r2;
            g = (255-alpha)*g1 + alpha*g2;
            b = (255-alpha)*b1 + alpha*b2;
            r=r/255;
            g=g/255;
            b=b/255;
        
        outColor=(r<<11)+(g<<5)+b;
        llSetPoint(i,y,outColor);
        }
        alphaColor++;
    }
}

void llFillMultipleTransparent5515Colors(int16_t x0,int16_t y0,int16_t x1,int16_t y1,llColor *color)
{
    int16_t y;

    for(y = y0; y <= y1; y++)
    {
        llFillLineMultipleTransparent5515Colors(x0,x1,y,color);
        color=color+(x1-x0)+1;
    }
}

void llFillMultipleTransparent8565Colors(int16_t x0,int16_t y0,int16_t x1,int16_t y1,uint32_t *alphaColor)
{
    int16_t y;

    for(y = y0; y <= y1; y++)
    {
        llFillLineMultipleTransparent8565Colors(x0,x1,y,alphaColor);
        alphaColor=alphaColor+(x1-x0)+1;
    }
}


#define CONVERT_TYPE_5515       0
#define CONVERT_TYPE_565       1
#define CONVERT_TYPE_888       2
#define CONVERT_TYPE_8888       3
#define CONVERT_TYPE_8565       4
//此函数有bug 缓冲区和宽度相关读刷数据问题

//图片宽度必须小于 IMAGE_READ_BYTE_MAX/位数
#define IMAGE_READ_BYTE_MAX      1024

static uint8_t convertTypeBit[5]= {16,16,32,32,32};//数据占用多少位

typedef struct
{
    int16_t width;
    int16_t height;
    uint8_t convertType;
    uint8_t reserved1;
    uint8_t reserved2;
    uint8_t reserved3;
    uint8_t reserved4;
    uint8_t reserved5;
    uint8_t reserved6;
    uint8_t reserved7;
    uint8_t reserved8;
    uint8_t reserved9;
    uint8_t reserved10;
    uint8_t reserved11;
} imageHeaderTypedef;

llSize llGeneralGetImageSize(uint32_t imageAddr)
{
    llSize retSize= {0};
    imageHeaderTypedef imageHeader;
    if(imageAddr!=0xFFFFFFFF)
    {
        llReadExFlash(imageAddr,(uint8_t*)&imageHeader,16);
        retSize.width=imageHeader.width;
        retSize.height=imageHeader.height;
    }
    return retSize;
}

llGeometry llGeneralGetImageGeometry(int16_t x,int16_t y,uint32_t imageAddr)
{
    llGeometry retGeometry= {0};
    imageHeaderTypedef imageHeader;
    if(imageAddr!=0xFFFFFFFF)
    {
        llReadExFlash(imageAddr,(uint8_t*)&imageHeader,16);
        retGeometry.x=x;
        retGeometry.y=y;
        retGeometry.width=imageHeader.width;
        retGeometry.height=imageHeader.height;
    }
    return retGeometry;
}

llGeometry llGeneralImageShow(uint32_t imageAddr,int16_t x,int16_t y)//针对16色图片和16色显示
{
    uint32_t addr=0;
    llGeometry geometry= {0,0,0,0};
    uint16_t writePixelCount=0;//读取一次能写多少个像素
    uint16_t writeHeightCount=0; //一次最多能读多少行， 如果不够一行的话就是 0
    uint16_t currHeight = 0; // 当可以写多于一行， 这个是用于存储当前实际能写多少行
    uint16_t currLineX = 0 ;//下一个要写的点的x坐标， 当每次能读的像素少于 一行的宽 才会用到。
    uint16_t currPixelCount = 0; //当只能写少于一行， 这用于储存这一行实际写多少个像素

    imageHeaderTypedef imageHeader;

    union
    {
        uint8_t u8ReadBuf[IMAGE_READ_BYTE_MAX];
        uint16_t u16ReadBuf[IMAGE_READ_BYTE_MAX/2];
        uint32_t u32ReadBuf[IMAGE_READ_BYTE_MAX/4];
    } unionReadBuf;

    //    FRESULT ret;
    //    FIL f;
    uint32_t i,totalCount;

    if(imageAddr!=0xFFFFFFFF)
    {
        addr=imageAddr;
        llReadExFlash(addr,(uint8_t*)&imageHeader,16);
        addr=addr+16;

        geometry.x=x;
        geometry.y=y;
        geometry.width=imageHeader.width;
        geometry.height=imageHeader.height;

        totalCount=imageHeader.width*imageHeader.height;
        writePixelCount = IMAGE_READ_BYTE_MAX /( convertTypeBit[imageHeader.convertType] / 8);

        if((geometry.width>0)&&(geometry.width>0))//0不能做除数
        {
            writeHeightCount = writePixelCount / geometry.width;
        }
        else
        {
            writeHeightCount = 0;
        }

        i=0;
        while(i < totalCount)
        {
            switch (imageHeader.convertType)
            {
            case CONVERT_TYPE_5515:
            case CONVERT_TYPE_565:
            {
                if(writeHeightCount > 0)
                {
                    currHeight = MIN(writeHeightCount, geometry.height - i/ geometry.width );
                    llReadExFlash(addr,(uint8_t*)&unionReadBuf.u8ReadBuf,2 *geometry.width*currHeight);
                    addr=addr+(2 *geometry.width*currHeight);

                    if(imageHeader.convertType==CONVERT_TYPE_565)
                    {
                        llFillMultipleColors(x, y + i/geometry.width, x+geometry.width -1,  y + i/geometry.width + currHeight -1,unionReadBuf.u16ReadBuf);
                    }
                    else
                    {
                        if(imageHeader.convertType==CONVERT_TYPE_5515)
                        {
                            llFillMultipleTransparent5515Colors(x, y + i/geometry.width, x+geometry.width -1,  y + i/geometry.width + currHeight -1,unionReadBuf.u16ReadBuf);
                        }
                    }
                    i += currHeight * geometry.width;
                }
                else
                {
                    currLineX = i % geometry.width;
                    currPixelCount = MIN(writePixelCount, geometry.width - currLineX);
                    llReadExFlash(addr,(uint8_t*)&unionReadBuf.u8ReadBuf,2 *currPixelCount);
                    addr=addr+(2 *currPixelCount);

                    if(imageHeader.convertType==CONVERT_TYPE_565)
                    {
                        llFillMultipleColors(x + currLineX, y + i /geometry.width, x + currLineX + currPixelCount, y + i /geometry.width,unionReadBuf.u16ReadBuf);
                    }
                    else
                    {
                        if(imageHeader.convertType==CONVERT_TYPE_5515)
                        {
                            llFillMultipleTransparent5515Colors(x + currLineX, y + i /geometry.width, x + currLineX + currPixelCount, y + i /geometry.width,unionReadBuf.u16ReadBuf);
                        }
                    }
                    i += currPixelCount;
                }
            }
            break;
            case CONVERT_TYPE_888:
            case CONVERT_TYPE_8888:
            {
//                if(writeHeightCount > 0)
//                {
//                    currHeight = MIN(writeHeightCount, geometry.height - i/ geometry.width );
//                    llReadExFlash(addr,(uint8_t*)&unionReadBuf.u8ReadBuf,4 *geometry.width*currHeight);
//                    addr=addr+(4 *geometry.width*currHeight);
//                    llFillMultipleColors(x, y + i/geometry.width, x+geometry.width -1,  y + i/geometry.width + currHeight -1,unionReadBuf.u32ReadBuf);
//                    i += currHeight * geometry.width;
//                }
//                else
//                {
//                    currLineX = i % geometry.width;
//                    currPixelCount = MIN(writePixelCount, geometry.width - currLineX);
//                    llReadExFlash(addr,(uint8_t*)&unionReadBuf.u8ReadBuf,4 *currPixelCount);
//                    addr=addr+(4 *currPixelCount);
//                    llFillMultipleColors(x + currLineX, y + i /geometry.width, x + currLineX + currPixelCount, y + i /geometry.width,unionReadBuf.u32ReadBuf);
//                    i += currPixelCount;
//                }
            }
            break;
            case CONVERT_TYPE_8565:
            {
                if(writeHeightCount > 0)
                {
                    currHeight = MIN(writeHeightCount, geometry.height - i/ geometry.width );
                    llReadExFlash(addr,(uint8_t*)&unionReadBuf.u8ReadBuf,4 *geometry.width*currHeight);
                    addr=addr+(4 *geometry.width*currHeight);

                    llFillMultipleTransparent8565Colors(x, y + i/geometry.width, x+geometry.width -1,  y + i/geometry.width + currHeight -1,unionReadBuf.u32ReadBuf);
                    
                    i += currHeight * geometry.width;
                }
                else
                {
                    currLineX = i % geometry.width;
                    currPixelCount = MIN(writePixelCount, geometry.width - currLineX);
                    llReadExFlash(addr,(uint8_t*)&unionReadBuf.u8ReadBuf,4 *currPixelCount);
                    addr=addr+(4 *currPixelCount);

                    llFillMultipleTransparent8565Colors(x + currLineX, y + i /geometry.width, x + currLineX + currPixelCount, y + i /geometry.width,unionReadBuf.u32ReadBuf);
                    
                    i += currPixelCount;
                }
                break;
            }
            
            default:
                break;
            }
        }
    }
    return geometry;
}

void llRgbaFillSpecificAreaMultipleColors(llGeometry originGeometry,llGeometry targetGeometry, uint8_t *color)
{
    uint16_t i,j;
    uint16_t x,y,width,height;
    uint16_t xT,yT,widthT,heightT;
    uint32_t rgba;
    uint8_t a,r,g,b;

    x=originGeometry.x;
    y=originGeometry.y;
    width=originGeometry.width;
    height=originGeometry.height;

    xT=targetGeometry.x;
    yT=targetGeometry.y;
    widthT=targetGeometry.width;
    heightT=targetGeometry.height;

    if((x<=xT)&&(y<=yT)&&(width>=widthT)&&(height>=heightT))
    {
        for(j=yT; j<(heightT+yT); j++)
        {
            for(i=xT; i<(widthT+xT); i++)
            {
                a=color[(j*width+i)*4+3];
                if(a!=0)//判断不是透明
                {
                    r=color[(j*width+i)*4];//*Alpha;
                    g=color[(j*width+i)*4+1];//*Alpha;
                    b=color[(j*width+i)*4+2];//*Alpha;
                    rgba=RGB_CONVERT(r,g,b);
                    llSetPoint(x+i,y+j,rgba);
                }
            }
        }
    }
}

void llRgbFillSpecificAreaMultipleColors(llGeometry originGeometry,llGeometry targetGeometry, uint8_t *color)
{
    uint16_t i,j;
    uint16_t x,y,width,height;
    uint16_t xT,yT,widthT,heightT;
    uint32_t rgb;
    uint8_t r,g,b;

    x=originGeometry.x;
    y=originGeometry.y;
    width=originGeometry.width;
    height=originGeometry.height;

    xT=targetGeometry.x;
    yT=targetGeometry.y;
    widthT=targetGeometry.width;
    heightT=targetGeometry.height;

    if((x<=xT)&&(y<=yT)&&(width>=widthT)&&(height>=heightT))
    {
        for(j=yT; j<(heightT+yT); j++)
        {
            for(i=xT; i<(widthT+xT); i++)
            {
                r=color[(j*width+i)*3];
                g=color[(j*width+i)*3+1];
                b=color[(j*width+i)*3+2];
                rgb=RGB_CONVERT(r,g,b);
                llSetPoint(x+i,y+j,rgb);
            }
        }
    }
}

//全屏绝对坐标计算
void llGeneralImageSpecificAreaShow(llGeometry originGeometry,llGeometry targetGeometry,uint32_t imageAddr)
{
    uint32_t addr;

    struct
    {
        int16_t width;
        int16_t height;
        uint8_t convertType;
        uint8_t reserved1;
        uint8_t reserved2;
        uint8_t reserved3;
        uint8_t reserved4;
        uint8_t reserved5;
        uint8_t reserved6;
        uint8_t reserved7;
        uint8_t reserved8;
        uint8_t reserved9;
        uint8_t reserved10;
        uint8_t reserved11;
    } imageHeader;

    union
    {
        uint8_t u8ReadBuf[IMAGE_READ_BYTE_MAX];
        uint16_t u16ReadBuf[IMAGE_READ_BYTE_MAX/2];
        uint16_t u32ReadBuf[IMAGE_READ_BYTE_MAX/4];
    } unionReadBuf;

    uint16_t writePixelCount=0;//读取一次能写多少个像素
    uint16_t writeHeightCount=0; //一次最多能读多少行， 如果不够一行的话就是 0
    uint16_t currLineX = 0 ;//下一个要写的点的x坐标， 当每次能读的像素少于 一行的宽 才会用到。
    uint16_t currPixelCount = 0; //当只能写少于一行， 这用于储存这一行实际写多少个像素
    uint16_t leftOffset = targetGeometry.x - originGeometry.x;
    uint16_t rightOffset = originGeometry.width - targetGeometry.width - leftOffset;
    uint32_t i,totalCount;

    addr=imageAddr;
    llReadExFlash(addr,(uint8_t*)&imageHeader,16);
    addr=addr+16;

    totalCount =targetGeometry.width * targetGeometry.height;
    writePixelCount = IMAGE_READ_BYTE_MAX /( convertTypeBit[imageHeader.convertType] / 8);

    writeHeightCount = MIN(1, writePixelCount / targetGeometry.width);

    switch (imageHeader.convertType)
    {
    case CONVERT_TYPE_5515:
    case CONVERT_TYPE_565:
    {
        //跳过上方行
        addr+=originGeometry.width*(targetGeometry.y-originGeometry.y)*2;

        //跳过左边空位
        addr+=leftOffset * 2;
        break;
    }
    case CONVERT_TYPE_888:
    case CONVERT_TYPE_8888:
    {
        //跳过上方行
        addr+=originGeometry.width*(targetGeometry.y-originGeometry.y)*4;

        //跳过左边空位
        addr+=leftOffset * 4;
        break;
    }
    default:
        break;
    }

    i=0;
    while(i < totalCount)
    {
        switch (imageHeader.convertType)
        {
        case CONVERT_TYPE_5515:
        case CONVERT_TYPE_565:
        {
            //缓存足够大， 每次可以写一行
            if(writeHeightCount == 1)
            {
                llReadExFlash(addr,(uint8_t*)&unionReadBuf.u8ReadBuf,2 *targetGeometry.width);
                addr+=2 *originGeometry.width;
                if(imageHeader.convertType==CONVERT_TYPE_565)
                {
                    llFillMultipleColors(targetGeometry.x, targetGeometry.y + i/targetGeometry.width, targetGeometry.x + targetGeometry.width -1,  targetGeometry.y + i/targetGeometry.width,unionReadBuf.u16ReadBuf);
                }
                else
                {
                    if(imageHeader.convertType==CONVERT_TYPE_5515)
                    {
                        llFillMultipleTransparent5515Colors(targetGeometry.x, targetGeometry.y + i/targetGeometry.width, targetGeometry.x + targetGeometry.width -1,  targetGeometry.y + i/targetGeometry.width,unionReadBuf.u16ReadBuf);
                    }
                }
                i += targetGeometry.width;
            }
            else
            {
                //上次写完留下来的位置
                currLineX = i % targetGeometry.width;
                // 这一行还有10个像素还没写， 缓存可以放20个像素，只会读MIN（20，10） = 10个像素，
                //如果这一行还有 30个像素还没写， 缓存可以放20个像素， 就会读MIN（20，30） = 20 个像素
                //换句话说这是用来确保只读取缓存可以存的像素数目或更少,和确保不会写超过右边边界
                currPixelCount = MIN(writePixelCount, targetGeometry.width - currLineX);
                if(currLineX == 0)
                {
                    addr=addr+(leftOffset * 2);
                }
                llReadExFlash(addr,(uint8_t*)&unionReadBuf.u8ReadBuf,2*currPixelCount);
                addr=addr+2*currPixelCount;
                if(imageHeader.convertType==CONVERT_TYPE_565)
                {
                    llFillMultipleColors(targetGeometry.x + currLineX, targetGeometry.y + i /targetGeometry.width, targetGeometry.x + currLineX + currPixelCount, targetGeometry.y + i /targetGeometry.width,unionReadBuf.u16ReadBuf);
                }
                else
                {
                    if(imageHeader.convertType==CONVERT_TYPE_5515)
                    {
                        llFillMultipleTransparent5515Colors(targetGeometry.x + currLineX, targetGeometry.y + i /targetGeometry.width, targetGeometry.x + currLineX + currPixelCount, targetGeometry.y + i /targetGeometry.width,unionReadBuf.u16ReadBuf);
                    }
                }
                if(currLineX + currPixelCount == targetGeometry.width)
                {
                    addr=addr+(rightOffset * 2);
                }
                i += currPixelCount;
            }
            break;
        }

        case CONVERT_TYPE_888:
        case CONVERT_TYPE_8888:
        {

            break;
        }
        default:
            break;
        }
    }
}

void* llGeneralGetWidget(uint16_t nameId,llWidgetType widgetType)
{
    llListWidgetInfo *widgetInfo;

    if(llList_GetInfoByName(&widgetInfo,nameId)==true)
    {
        if(((llGeneral*)widgetInfo->widget)->widgetType==widgetType)
        {
            return (llGeneral*)widgetInfo->widget;
        }
    }
    return NULL;
}

uint8_t llRgbToGray(uint8_t r,uint8_t g,uint8_t b)
{
    return ((r*38 + g*75 + b*15) >> 7);
}

uint16_t llRgb565ToGray(uint16_t rgb565)
{
    uint8_t gray=0;

    gray= llRgbToGray(((rgb565>>11)&0x1F),(((rgb565) >> 5) & 0x3f),(rgb565&0x1F));
    return RGB_CONVERT(gray,gray,gray);
}

uint32_t llRgb888ToGray(uint32_t rgb888)
{
    uint8_t gray=0;
    gray= llRgbToGray(((rgb888>>16)&0xFF),(((rgb888) >> 8) & 0xFF),(rgb888&0xFF));
    return RGB_CONVERT(gray,gray,gray);
}


void llGeneralWidgetSelect(llGeneral *widget)
{
    int16_t x,y,width,height;

    llPoint globalPos;
    globalPos=llListGetGlobalPos(widget->parentWidget);

    x=widget->geometry.x+globalPos.x;
    y=widget->geometry.y+globalPos.y;
    width=widget->geometry.width;
    height=widget->geometry.height;

    llDrawLine(x,y,x+width,y,0);
    llDrawLine(x,y,x,y+height,0);
    llDrawLine(x,y+height,x+width,y+height,0);
    llDrawLine(x+width,y,x+width,y+height,0);
}

void llGeneralWidgetParentRecover(llGeneral *widget,llGeometry geometry)
{
    if(widget->parentType==widgetTypeBackground)
    {
        pBackgroundSpecificAreaRecover((llBackground*)widget->parentWidget,geometry);
    }
    else
    {
        if(widget->parentType==widgetTypeWindow)
        {
            pWindowSpecificAreaRecover((llWindow*)widget->parentWidget,geometry);
        }
    }
}

//查找控件的所有父控件是否有隐藏，bg(show) - win(show) - win(hide) - btn(不能显示)
bool llGeneralParentLinkHidden(llGeneral *widget)
{
    llGeneral *w;
    w=widget->parentWidget;
    while(w->widgetType!=widgetTypeBackground)
    {
        if(w->isHidden)
        {
            return true;
        }
        w=w->parentWidget;
    }
    return false;
}


//#define monochrome_2_RGB888(color)                (color?0x000000:0xffffff)
//#define RGB233_2_RGB888(color)                    (((color&0xC0)<<16)+((color&0x38)<<10)+((color&0x07)<<5))
//#define RGB565_2_RGB888(color)                    (((color&0xF800)<<8)+((color&0x7E0)<<5)+((color&0x1F)<<3))


#define RGB888_2_monochrome(color)                ((color)?0:1)
#define RGB888_2_RGB233(color)                    ((((color&0xff0000)>>22) << 6) + (((color&0xff00)>>13) << 3) + (((color&0xff)>>5)))
#define RGB888_2_RGB565(color)                    ((((color&0xff0000)>>19) << 11) + (((color&0xff00)>>10) << 5) + (((color&0xff)>>3)))

//// 1 8(233) 16(565) 24(888)
//#if CONFIG_COLOR_DEPTH == 1
//#define RGB888(color)                  RGB888_2_monochrome(color)
//#define RGB_CONVERT(R,G,B)             RGB888((uint32_t)(((uint32_t)R<<16)|((uint32_t)G<<8)|B))
//#elif CONFIG_COLOR_DEPTH == 8
//#define RGB888(color)                 RGB888_2_RGB233(color)
//#define RGB_CONVERT(R,G,B)             RGB888((uint32_t)(((uint32_t)R<<16)|((uint32_t)G<<8)|B))
//#elif CONFIG_COLOR_DEPTH == 16
//#define RGB888(color)                  RGB888_2_RGB565(color)
//#define RGB_CONVERT(R,G,B)             RGB888((uint32_t)(((uint32_t)R<<16)|((uint32_t)G<<8)|B))
//#elif CONFIG_COLOR_DEPTH == 24
//#define RGB888(color)                 (color)
//#define RGB_CONVERT(R,G,B)             RGB888((uint32_t)(((uint32_t)R<<16)|((uint32_t)G<<8)|B))
//#endif

llColor RGB888(uint32_t color)
{
    llColor ret=0;
    // 1 8(233) 16(565) 24(888)
    switch(cfgColorDepth)
    {
    case 1:
    {
        ret= RGB888_2_monochrome(color);
        break;
    }
    case 8:
    {
        ret= RGB888_2_RGB233(color);
        break;
    }
    case 16:
    {
        ret= RGB888_2_RGB565(color);
        break;
    }
    case 24:
    {
        ret= color;
        break;
    }
    default:
        break;
    }
    return ret;
}

llColor RGB_CONVERT(uint8_t R,uint8_t G,uint8_t B)
{
    llColor ret=0;
    // 1 8(233) 16(565) 24(888)
    switch(cfgColorDepth)
    {
    case 1:
    {
        ret= RGB888((uint32_t)(((uint32_t)R<<16)|((uint32_t)G<<8)|B));
        break;
    }
    case 8:
    {
        ret= RGB888((uint32_t)(((uint32_t)R<<16)|((uint32_t)G<<8)|B));
        break;
    }
    case 16:
    {
        ret= RGB888((uint32_t)(((uint32_t)R<<16)|((uint32_t)G<<8)|B));
        break;
    }
    case 24:
    {
        ret= RGB888((uint32_t)(((uint32_t)R<<16)|((uint32_t)G<<8)|B));
        break;
    }
    default:
        break;
    }
    return ret;
}

void llGeometryGetR_G_B(llColor color,uint8_t *r,uint8_t *g,uint8_t *b)
{
    // 1 8(233) 16(565) 24(888)
    switch(cfgColorDepth)
    {
    case 1:
    {

        break;
    }
    case 8:
    {

        break;
    }
    case 16:
    {
        *r=(color>>11)&0x1F;
        *g=(color>>5)&0x3F;
        *b=color&0x1F;
        break;
    }
    case 24:
    {

        break;
    }
    default:
        break;
    }
}

bool llGeometryAutoSetHiddenIsShow(llGeneral *widget,bool isHidden)
{
    if(widget->isEnable)
    {
        if(widget->isHidden)//控件现在处于隐藏
        {
            if(isHidden==false)//变为显示
            {
                //变为显示
                widget->isHidden=false;
                return true;
            }
        }
        else//控件现在处于显示
        {
            if(isHidden==true)//变为隐藏
            {
                widget->isHidden=true;
                //变为隐藏,恢复背景
                llGeneralWidgetParentRecover(widget,widget->geometry);
            }
        }
    }
    return false;
}

//llPointColor llGeneralGetColor(int16_t x,int16_t y)
//{
//    llPointColor ret;
//    ret.x=x;
//    ret.y=y;
//    ret.color=llGetPoint(x,y);
//    return ret;
//}

//void llGeneralRectBackupPointColor(llPointColor* outBuf,llGeometry geometry)
//{
//    uint16_t i,j;
//    for(i=0;i<geometry.width;i++)
//    {
//        for(j=0;j<geometry.height;j++)
//        {
//            outBuf[i+j*geometry.height]=llGeneralGetColor(geometry.x+i,geometry.y+j);
//        }
//    }
//}

void llGeneralRectBackupColor(llColor* outBuf,llGeometry geometry)
{
    uint16_t i,j;
    for(i=0; i<geometry.width; i++)
    {
        for(j=0; j<geometry.height; j++)
        {
            outBuf[i+j*geometry.height]=llGetPoint(geometry.x+i,geometry.y+j);
        }
    }
}


