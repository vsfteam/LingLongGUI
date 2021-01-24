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

#include "LL_String.h"
//反转字符串
static char *reverse(char *s)
{
    char temp;
    char *p = s;    //p指向s的头部
    char *q = s;    //q指向s的尾部
    while(*q)
        ++q;
    q--;

    //交换移动指针，直到p和q交叉
    while(q > p)
    {
        temp = *p;
        *p++ = *q;
        *q-- = temp;
    }
    return s;
}

static char *my_itoa(int value, char *string,int radix)
{
    int i = 0,isNegative = 0;
    if((isNegative = value) < 0) //如果是负数，先转为正数
    {
        value = -value;
    }
    do      //从个位开始变为字符，直到最高位，最后应该反转
    {
        string[i++] = (char)(value%radix + '0');
        value = value/radix;
    }while(value > 0);

    if(isNegative < 0)   //如果是负数，补上负号
    {
        string[i++] = '-';
    }
    string[i] = '\0';    //最后加上字符串结束符
    return reverse(string);
}

int32_t llStrToI32(uint8_t * str)
{
    return atoi((const char*)str);
}

uint8_t* llI32ToStr(int32_t inInt32,uint8_t *buf)
{
    return (uint8_t*)my_itoa(inInt32,(char*)buf,10);
//    sprintf((char*)buf,"%d",inInt32);
//    return buf;
}

float llStrToFloat(uint8_t * str)
{
    return atof((const char*)str);
}

uint8_t* llFloatToStr(float inFloat,uint8_t afterDecimalPointNum,uint8_t *buf)
{
    int temp;
    uint8_t size;
    if(afterDecimalPointNum>7)
    {
        afterDecimalPointNum=7;
    }

    //去除小数
    for(temp=0;temp<afterDecimalPointNum;temp++)
    {
        inFloat*=10;
    }
    //整数输出
    temp=inFloat;
    my_itoa(temp,(char*)buf,10);

    //插入小数点
    size=strlen((char*)buf);
    buf[size+1]=0;
    for(temp=0;temp<afterDecimalPointNum;temp++)
    {
        buf[size]=buf[size-1];
        size--;
    }
    buf[size]='.';

    //整数是0
    if(size==0)
    {
        //插入整数0
        size=strlen((char*)buf);
        buf[size+1]=0;
        for(temp=0;temp<afterDecimalPointNum+1;temp++)
        {
            buf[size]=buf[size-1];
            size--;
        }
        buf[size]='0';
    }

    //自动删除尾部'0'
    size=strlen((char*)buf);
    while(buf[--size]=='0')
    {
        buf[size]=0;
    }
    return buf;
}


