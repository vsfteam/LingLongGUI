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
//��ת�ַ���
static char *reverse(char *s)
{
    char temp;
    char *p = s;    //pָ��s��ͷ��
    char *q = s;    //qָ��s��β��
    while(*q)
        ++q;
    q--;

    //�����ƶ�ָ�룬ֱ��p��q����
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
    if((isNegative = value) < 0) //����Ǹ�������תΪ����
    {
        value = -value;
    }
    do      //�Ӹ�λ��ʼ��Ϊ�ַ���ֱ�����λ�����Ӧ�÷�ת
    {
        string[i++] = (char)(value%radix + '0');
        value = value/radix;
    }while(value > 0);

    if(isNegative < 0)   //����Ǹ��������ϸ���
    {
        string[i++] = '-';
    }
    string[i] = '\0';    //�������ַ���������
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

    //ȥ��С��
    for(temp=0;temp<afterDecimalPointNum;temp++)
    {
        inFloat*=10;
    }
    //�������
    temp=inFloat;
    my_itoa(temp,(char*)buf,10);

    //����С����
    size=strlen((char*)buf);
    buf[size+1]=0;
    for(temp=0;temp<afterDecimalPointNum;temp++)
    {
        buf[size]=buf[size-1];
        size--;
    }
    buf[size]='.';

    //������0
    if(size==0)
    {
        //��������0
        size=strlen((char*)buf);
        buf[size+1]=0;
        for(temp=0;temp<afterDecimalPointNum+1;temp++)
        {
            buf[size]=buf[size-1];
            size--;
        }
        buf[size]='0';
    }

    //�Զ�ɾ��β��'0'
    size=strlen((char*)buf);
    while(buf[--size]=='0')
    {
        buf[size]=0;
    }
    return buf;
}


