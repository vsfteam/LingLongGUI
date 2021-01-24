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

#include "LL_Config.h"
#include "string.h"
#include "gt9xx.h"
#include "ns2009.h"
#include "lcd.h"
#include "LL_Mem.h"
#include "w25qxx.h"
#include "buzzer.h"
#include "setDefine.h"
#include "bm8563.h"
#include "LL_User.h"
#include "iic.h"
uint8_t cfgButtonTextLengthMax = LLBUTTON_TEXT_LENGTH_MAX;
uint8_t cfgLineEditTextLengthMax = LLLINEEDIT_TEXT_LENGTH_MAX;
uint8_t cfgCheckBoxTextLengthMax = LLCHECKBOX_TEXT_LENGTH_MAX;

uint8_t cfgColorDepth = CONFIG_COLOR_DEPTH;
uint16_t cfgMonitorWidth = 0;
uint16_t cfgMonitorHeight = 0;

uint8_t touchType;

#if USE_EXRAM == 1
__align(8) uint8_t cfgMemHeapBuffer[ CONFIG_MEM_SIZE ]  __attribute__((at(EX_MEM_ADDR)));
#else
uint8_t cfgMemHeapBuffer[CONFIG_MEM_SIZE] = {0};
#endif


uint32_t *lcdFrontBuf=LL_LCD_BUF1_POINTER;
uint32_t *lcdBackBuf=LL_LCD_BUF2_POINTER;

void llCfgSetLcdBufAddr(uint32_t *addr)
{
    #if USE_DOUBLE_BUFFERING == 1
    lcdSetBufferAddr(addr);
    #endif
}

void llCfgSetLcdSrcAddr(uint32_t *addr)
{
    #if USE_DOUBLE_BUFFERING == 1
    lcdSetSrcAddr(addr);
    #endif
}

void llCfgLcdCopyFront2Back(void)
{
    #if USE_DOUBLE_BUFFERING == 1
//    memcpy(lcdBackBuf,lcdFrontBuf,SDRAM_LCD_SIZE);
    uint64_t i;
    for(i=0;i<SDRAM_LCD_SIZE;i++)
    {
        lcdBackBuf[i]=lcdFrontBuf[i];
    }
    #endif
}


bool llCfgClickGetPoint(int16_t *x,int16_t *y)
{
    bool touchState=false;
    uint16_t rx;
    uint16_t ry;
    if(touchType==TOUCH_TYPE_GT911)
    {
        touchState= gt9xx_scan(&rx,&ry);
    }
    else
    {
        touchState=ns2009_scan(0,&rx,&ry);
    }
    *x=rx;
    *y=ry;
    if((touchState!=0)&&(rx!=0xffff)&&(ry!=0xffff))
    {
        touchState=true;
    }
    else
    {
        touchState=false;
        *x=0xffff;
        *y=0xffff;
    }
    return touchState;
}

void llCfgSetPoint(int16_t x,int16_t y,llColor color)
{
    lcdSetPoint(x,y,color);
}

llColor llCfgGetPoint(int16_t x,int16_t y)
{
    return lcdGetPoint(x,y);
}

void llCfgFillSingleColor(int16_t x0,int16_t y0,int16_t x1,int16_t y1,llColor color)
{
    lcdFill(x0,y0,x1,y1,color);
}

void llExFlashCopy(void)
{
    uint16_t i=0;
    uint8_t pBuffer[1024];

    //�Զ�����nor��sdram
    for(i=0; i<1024; i++)
    {
        w25qxxRead(pBuffer,(i*1024),1024);
        sdram_8bitWrite(SDRAM_NOR_ADDR+(i*1024),pBuffer,1024);
    }
}

void llReadExFlash(uint32_t addr,uint8_t* pBuffer,uint16_t length)
{
//    w25qxxRead(pBuffer,addr,length);
    sdram_8bitRead(SDRAM_NOR_ADDR+addr,pBuffer,length);
}

void llBuzzerBeep(void)
{
    buzzerSetOn();
    llTimer_delayMs(50);
    buzzerSetOff();
}


// yy yy mm dd hh mm ss ww
void llGetRtc(uint8_t *readBuf)
{
    uint16_t year;
    uint8_t month,day,hour,minute,second,week;

    bm8563_readDate(&year,&month,&day);
    bm8563_readTime(&hour,&minute,&second);
    week= bm8563_getWeek(year,month,day);
    readBuf[0]=year>>8;
    readBuf[1]=year;
    readBuf[2]=month;
    readBuf[3]=day;
    readBuf[4]=hour;
    readBuf[5]=minute;
    readBuf[6]=second;
    readBuf[7]=week;
}

// yy yy mm dd hh mm ss
void llSetRtc(uint8_t *writeBuf)
{
    bm8563_writeDate((writeBuf[0]<<8)+writeBuf[1],writeBuf[2],writeBuf[3]);
    bm8563_writeTime(writeBuf[4],writeBuf[5],writeBuf[6]);
}

