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

#include "LL_TTF.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
#include "LL_General.h"

static stbtt_fontinfo font_info;
static uint8_t *fontLibraryBuffer=NULL;
static float fontScale;
static int bascLine;
static uint32_t fontGap;

//static uint8_t utf8ToUnicode(uint8_t* unicode, uint8_t *utf8)
//{
//    uint8_t nBytes = 0;

//    if (0 == (*utf8 & 0x80))
//    {
//        /*
//         * single-byte char
//         */
//        nBytes = 1;
//        unicode[0] = *utf8;
//    }
//    else
//    {
//        /*
//         * 3-byte char (chinese char)
//         */
//        if ( (*utf8 & 0xf0) == 0xe0 )
//        {
//            nBytes  = 3;
//            unicode[0] = ((utf8[0] & 0x0f) <<4) + ((utf8[1] & 0x3c) >>2);
//            unicode[1] = ((utf8[1] & 0x03) <<6) + (utf8[2] & 0x3f);
//        }
//        else
//        {
//            //            debug("ERROR: utf-8 to unicode, nBytes !=3\n");
//            nBytes = 0;
//            unicode[0] = '?';
//        }
//    }
//    return nBytes;
//}

bool llTTF_fontOpenLibrary(uint8_t * path,uint16_t pxSize)
{
    FILE *fp = NULL;
    uint32_t fileSize;
    int ascent;
    int descent = 0;
    int lineGap = 0;
    fp = fopen((const char *)path, "r");
    if (NULL == fp)
    {
        return false; /* 要返回错误代码 */
    }

    fseek(fp,0,SEEK_SET);
    fseek(fp,0,SEEK_END);
    fileSize=ftell(fp);// longBytes就是文件的长度
    fseek(fp,0,SEEK_SET);//还原偏移位置

    fontLibraryBuffer=(uint8_t*)llMalloc(fileSize*sizeof(uint8_t));

    if(fontLibraryBuffer!=NULL)
    {
        fread(fontLibraryBuffer, 1, fileSize, fp);
        int offset=stbtt_GetFontOffsetForIndex((const uint8_t *)fontLibraryBuffer,0);
        stbtt_InitFont(&font_info, (const uint8_t *)fontLibraryBuffer, offset);

        fontScale = stbtt_ScaleForPixelHeight(&font_info, pxSize);
        stbtt_GetFontVMetrics(&font_info, &ascent, &descent, &lineGap);
        //        printf("*****%d,%d,%d\n",ascent,descent,lineGap);
        bascLine=roundf(fontScale*ascent);
        fontGap=roundf(fontScale*ascent);
        return true;
    }


    return false;
}

void llTTF_setSize(uint16_t pxSize)
{
    int ascent;
    int descent = 0;
    int lineGap = 0;

    fontScale = stbtt_ScaleForPixelHeight(&font_info, pxSize);
    stbtt_GetFontVMetrics(&font_info, &ascent, &descent, &lineGap);
    //        printf("*****%d,%d,%d\n",ascent,descent,lineGap);
    bascLine=roundf(fontScale*ascent);
    fontGap=roundf(fontScale*ascent);
}

void llTTF_fontCloseLibrary(void)
{
    if(fontLibraryBuffer!=NULL)
    {
        llFree(fontLibraryBuffer);
        fontLibraryBuffer=NULL;
    }
}



#define TTF_DATA_ZERO  0
#define TTF_DATA_ONE   1
#define TTF_LINE_END   2
#define TTF_WORD_END   3

static void llTTF_fontSaveLattice(uint8_t dat,uint8_t *buffer)
{
    static uint8_t uiData=0;
    static uint8_t bit_count=0;
    static uint16_t byte_count=0;
    if(dat==TTF_DATA_ZERO)
    {
//                printf("0");
        bit_count++;
        if(bit_count==8)
        {
            CLRBIT(uiData,(bit_count-1));
            //            printf(" %X\n",uiData);
            buffer[byte_count]=uiData;
            byte_count++;
            bit_count=0;
        }
        else
        {
            CLRBIT(uiData,(bit_count-1));
        }
    }
    if(dat==TTF_DATA_ONE)
    {
//                printf("1");
        bit_count++;
        if(bit_count==8)
        {
            SETBIT(uiData,(bit_count-1));
            //            printf(" %X\n",uiData);
            buffer[byte_count]=uiData;
            byte_count++;
            bit_count=0;
        }
        else
        {
            SETBIT(uiData,(bit_count-1));
        }
    }
    if(dat==TTF_LINE_END)
    {
//        printf("\n");
    }
    if(dat==TTF_WORD_END)
    {
        if(bit_count!=0)
        {
            //            printf(" %X\n",uiData);
            buffer[byte_count]=uiData;

        }
        byte_count=0;
        uiData=0;
        bit_count=0;
//        printf("\n");
    }
}

void llTTF_fontGetData(uint8_t * utf8In, uint16_t pxSize, uint8_t *retBitmap,uint16_t *retWidth,uint16_t *retHeight)
{
    int unicode_c;
    uint8_t out[3];
    uint16_t i,j;
    uint16_t width,height;

    int advance,leftSideBearing,x0,y0,x1,y1;
    int32_t leftWidth;

    uint8_t *screen;


    if (0 == (utf8In[0] & 0x80))
    {
        unicode_c = utf8In[0];

    }
    else
    {

        llUtf8ToUnicode(out,utf8In);
        unicode_c=out[0];
        unicode_c=unicode_c<<8;
        unicode_c+=out[1];

    }

    stbtt_GetCodepointHMetrics(&font_info, unicode_c, &advance, &leftSideBearing);//字体宽度和左侧空白区域宽度(负值为越界宽度)
    width=roundf(advance*fontScale);
    height=pxSize;

    if(leftSideBearing>=0)
    {
        leftWidth=0;
    }
    else
    {
        leftWidth=-floor(leftSideBearing*fontScale);
    }
    //        printf("*****%d,%d,%lf,%lf\n",advance,leftSideBearing,advance*font_scale,leftSideBearing*font_scale);
    stbtt_GetCodepointBitmapBox(&font_info, unicode_c, fontScale,fontScale,&x0,&y0,&x1,&y1);
    //    printf("x0,y0,x1,y1=%d,%d,%d,%d\n",x0,y0,x1,y1);
    //printf("*****%d\n",x1-x0);
    //    printf("width,height,leftWidth*****%d,%d,%d\n",width,height,leftWidth);


    screen=(uint8_t *)llMalloc(sizeof(uint8_t)*(height+1)*width);

    if(screen!=NULL)
    {
        memset(screen,0,sizeof(uint8_t)*(height+1)*width);
//        printf("w,h,width,s*****%d,%d,%d,%lf\n",x1-x0,y1-y0,width,font_scale);
        //printf("bascLine %d\n",bascLine);
        //printf("%d\n",(bascLine + y0)*width + x0+leftWidth);

        //由于缩放后，float 转int 会有+-1行的误差，根据获取的字母边框数据计算top button是否超出范围
        int start_line = bascLine+y0;
        int end_line=start_line-1+(y1-y0);
        if(start_line>=0)
        {
            if((end_line>pxSize-1)&&(start_line>0))
            {
                // watch out bascline offset
                stbtt_MakeCodepointBitmap(&font_info, &screen[(bascLine-1 + y0)*width + x0+leftWidth], x1-x0,y1-y0, width, fontScale,fontScale,unicode_c);
            }
            else
            {
//                if((end_line>pxSize-1)&(start_line=0))
//                {
//                    //字体过大
//                }
//                else
                {
                    // 正常
                    // watch out bascline offset
                    stbtt_MakeCodepointBitmap(&font_info, &screen[(bascLine + y0)*width + x0+leftWidth], x1-x0,y1-y0, width, fontScale,fontScale,unicode_c);
                }

            }
        }
        else
        {
            if(end_line<pxSize-1)
            {
                // watch out bascline offset
                stbtt_MakeCodepointBitmap(&font_info, &screen[(bascLine+1 + y0)*width + x0+leftWidth], x1-x0,y1-y0, width, fontScale,fontScale,unicode_c);
            }
//            else
//            {
//                //字体过大
//            }
        }

//        // watch out bascline offset
//        stbtt_MakeCodepointBitmap(&font_info, &screen[(bascLine-1 + y0)*width + x0+leftWidth], x1-x0,y1-y0, width, font_scale,font_scale,unicode_c);

        *retWidth=width;
        *retHeight=height;
        //        printf("==========\n");
        for (j=0; j < height; ++j)
        {
            for (i=0; i < width; ++i)
            {
                //                printf("%c", screen[j*width+i]>>5? '1' : ' ');
                llTTF_fontSaveLattice((screen[j*width+i]>>5?TTF_DATA_ONE:TTF_DATA_ZERO),retBitmap);
            }
                        llTTF_fontSaveLattice(TTF_LINE_END,retBitmap);
            //            putchar('\n');
        }
        llTTF_fontSaveLattice(TTF_WORD_END,retBitmap);
        //        printf("==========\n");


        llFree(screen);


    }

}

//void llTTF_fontGetDataWithGap(uint8_t * utf8_in, uint16_t px_size, uint8_t *ret_bitmap,uint16_t *ret_width,uint16_t *ret_height)
//{
//    int unicode_c;
//    unsigned char out[3];
//    uint16_t i,j;
//    int32_t width,height;
//    int advance,leftSideBearing,x0,y0,x1,y1;
//    int32_t leftWidth;
//    uint8_t *screen;

//    if (0 == (utf8_in[0] & 0x80))
//    {
//        unicode_c = utf8_in[0];

//    }
//    else
//    {

//        utf8ToUnicode((char *)out,(char *)utf8_in);
//        unicode_c=out[0];
//        unicode_c=unicode_c<<8;
//        unicode_c+=out[1];

//    }


//    stbtt_GetCodepointHMetrics(&font_info, unicode_c, &advance, &leftSideBearing);//字体宽度和左侧空白区域宽度(负值为越界宽度)
//    width=roundf(advance*font_scale);
//    height=px_size+fontGap;
//    if(leftSideBearing>0)
//    {
//        leftWidth=0;
//    }
//    else
//    {
//        leftWidth=-floor(leftSideBearing*font_scale);
//    }

//    stbtt_GetCodepointBitmapBox(&font_info, unicode_c, font_scale,font_scale,&x0,&y0,&x1,&y1);

//    screen=(uint8_t *)TTF_FONT_MALLOC(sizeof(uint8_t)*height*width);
//    if(screen!=NULL)
//    {
//        memset(screen,0,sizeof(uint8_t)*height*width);


//        stbtt_MakeCodepointBitmap(&font_info, &screen[(bascLine + y0-1 +fontGap/2)*width+ x0+leftWidth], x1-x0,y1-y0, width, font_scale,font_scale,unicode_c);

//        *ret_width=width;
//        *ret_height=height;
//        for (j=0; j < height; ++j) {
//            for (i=0; i < width; ++i)
//            {
//                Save_Font_Lattice((screen[j*width+i]>>5?TTF_DATA_ONE:TTF_DATA_ZERO),ret_bitmap);
//            }

//        }
//        Save_Font_Lattice(TTF_WORD_END,ret_bitmap);
//        TTF_FONT_FREE(screen);


//    }
//}


uint8_t llTTF_fontGetWidthSize(uint8_t * utf8In)
{
    int unicode_c;
    unsigned char out[3];

    uint16_t width=0;

    int advance,leftSideBearing;


    if (0 == (utf8In[0] & 0x80))
    {
        unicode_c = utf8In[0];

    }
    else
    {

        llUtf8ToUnicode(out,utf8In);
        unicode_c=out[0];
        unicode_c=unicode_c<<8;
        unicode_c+=out[1];

    }

    stbtt_GetCodepointHMetrics(&font_info, unicode_c, &advance, &leftSideBearing);//字体宽度和左侧空白区域宽度(负值为越界宽度)
    width=roundf(advance*fontScale);

    return width;
}

uint8_t llTTF_fontGetHeightSize(void)
{
    uint16_t lineHeight=0;

    int ascent = 0;
    int descent = 0;
    int lineGap = 0;

    stbtt_GetFontVMetrics(&font_info, &ascent, &descent, &lineGap);
    lineHeight = ascent - descent + lineGap;

    lineHeight=roundf(lineHeight*fontScale);

    return (uint8_t)lineHeight;
}













