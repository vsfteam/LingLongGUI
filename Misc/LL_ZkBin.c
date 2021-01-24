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

#include "LL_ZkBin.h"


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

bool llZkBin_fontOpenLibrary(uint8_t * path)
{
    FILE *fp = NULL;
    uint32_t fileSize;
    int ascent;
    int descent = 0;
    int lineGap = 0;
    uint8_t buffer[32]={0};
    uint16_t charTotal=0;
    uint16_t charSize=0;
    fp = fopen((const char *)path, "r");

    if (NULL == fp)
    {
        return false; /* 要返回错误代码 */
    }

    fread(buffer, 1, 2, fp);
charTotal=buffer[0];
charTotal=charTotal<<8;
charTotal+=buffer[1];

fread(buffer, 1, 2, fp);
charSize=buffer[0];
charSize=charSize<<8;
charSize+=buffer[1];

//LLPRINT_NUM("charTotal",charTotal);
//LLPRINT_NUM("charSize",charSize);
//    fontLibraryBuffer=(uint8_t*)llMalloc(fileSize*sizeof(uint8_t));

//    if(fontLibraryBuffer!=NULL)
//    {
//        fread(fontLibraryBuffer, 1, fileSize, fp);
//        int offset=stbtt_GetFontOffsetForIndex((const uint8_t *)fontLibraryBuffer,0);
//        stbtt_InitFont(&font_info, (const uint8_t *)fontLibraryBuffer, offset);

//        fontScale = stbtt_ScaleForPixelHeight(&font_info, pxSize);
//        stbtt_GetFontVMetrics(&font_info, &ascent, &descent, &lineGap);
//        //        printf("*****%d,%d,%d\n",ascent,descent,lineGap);
//        bascLine=roundf(fontScale*ascent);
//        fontGap=roundf(fontScale*ascent);
//        return true;
//    }


    return false;
}

void llZkBin_fontCloseLibrary(void)
{
//    if(latticeActiveFont!=NULL)
//    {
//        latticeActiveFont=NULL;
//    }
}

void llZkBin_fontGetData(uint8_t * utf8In, void **retBitmap, uint16_t *retWidth, uint16_t *retHeight)
{
//    uint8_t unicode[2]={0};
//    uint32_t fontCount,i;
//    uint8_t ret;

//    ret=utf8ToUnicode(unicode,utf8In);

//    if(ret!=0)
//    {
//        //查找unicode

//        fontCount=latticeActiveFont->totalCharNum;

//        for(i=0;i<fontCount;i++)
//        {
//            if(ret==1)
//            {
//                if(latticeActiveFont->dataInfo[i].unicodeL==unicode[0])
//                {
//                    *retHeight=latticeActiveFont->dataInfo[i].height;
//                    *retWidth=latticeActiveFont->dataInfo[i].width;
//                    *retBitmap=latticeActiveFont->dataInfo[i].dataPointer;
//                    break;
//                }
//            }
//            else
//            {
//                if((latticeActiveFont->dataInfo[i].unicodeL==unicode[1])&(latticeActiveFont->dataInfo[i].unicodeH==unicode[0]))
//                {
//                    *retHeight=latticeActiveFont->dataInfo[i].height;
//                    *retWidth=latticeActiveFont->dataInfo[i].width;
//                    *retBitmap=latticeActiveFont->dataInfo[i].dataPointer;
//                    break;
//                }
//            }


//        }
//    }
}

uint8_t llZkBin_fontGetWidthSize(uint8_t * utf8In)
{
//    unsigned char unicode[2]={0};
//    uint32_t fontCount,i;
//    uint8_t ret;

//    ret=utf8ToUnicode(unicode,utf8In);

//    if(ret!=0)
//    {
//        //查找unicode

//        fontCount=latticeActiveFont->totalCharNum;

//        for(i=0;i<fontCount;i++)
//        {
//            if(ret==1)
//            {
//                if(latticeActiveFont->dataInfo[i].unicodeL==unicode[0])
//                {
//                    return latticeActiveFont->dataInfo[i].width;
//                }
//            }
//            else
//            {
//                if((latticeActiveFont->dataInfo[i].unicodeL==unicode[1])&(latticeActiveFont->dataInfo[i].unicodeH==unicode[0]))
//                {
//                    return latticeActiveFont->dataInfo[i].width;
//                }
//            }

//        }
//    }
    return 0;
}

uint8_t llZkBin_fontGetHeightSize(void)
{
//    return (uint8_t)latticeActiveFont->dataInfo[0].height;
    return 0;
}





