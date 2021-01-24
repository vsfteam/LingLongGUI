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

#ifndef _LL_TTF_H_
#define _LL_TTF_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "LL_General.h"

bool llTTF_fontOpenLibrary(uint8_t * path,uint16_t pxSize);
void llTTF_fontCloseLibrary(void);
void llTTF_setSize(uint16_t pxSize);
void llTTF_fontGetData(uint8_t * utf8In, uint16_t pxSize, uint8_t *retBitmap,uint16_t *retWidth,uint16_t *retHeight);
//    void TTF_Font_Get_Data_With_Gap(uint8_t * utf8_in, uint16_t px_size, uint8_t *ret_bitmap,uint16_t *ret_width,uint16_t *ret_height);
uint8_t llTTF_fontGetWidthSize(uint8_t * utf8In);
uint8_t llTTF_fontGetHeightSize(void);

#ifdef __cplusplus
}
#endif

#endif //_LL_TTF_H_
