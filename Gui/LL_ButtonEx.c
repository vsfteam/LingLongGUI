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

#include "LL_ButtonEx.h"
#include "LL_Timer.h"
#include "LL_Linked_List.h"


#define BUTTON_EVENT_NO_OPERATION    0 //��ⰴ���ް����ź�
#define BUTTON_EVENT_PRESS           1 //��ⰴ�������ź�
#define BUTTON_EVENT_HOLD_DOWN       2 //��ⰴ����ס�����ź�
#define BUTTON_EVENT_RELEASE         3 //��ⰴ���ͷ��ź�
#define BUTTON_EVENT_CLICK           4 //��ⰴ�������ź�


#define BUTTON_TICK_MS          10    //��ʱ���ж�����
#define BUTTON_DEBOUNCE_MS      10    //�����˲�ʱ��

typedef struct
{
    llListHead parent_link_pos;
    llButton *widget;
    uint8_t event;
    uint8_t FSM_State;
    uint8_t doubleClickCount;
    uint8_t repeatCount;
    uint16_t repeatTimeOutCount;
    uint16_t holdCount;
    uint16_t shootCount;
    bool pressLevel:1;
    bool btnNewState:1;
    bool btnOldState:1;
    bool isPressed:1;
    bool isReleased:1;
    bool isClicked:1;
    bool isDoubleClicked:1;
    bool isRepeatEnd:1;
    bool isShoot:1;
} llButtonExInfoTypedef;


LL_LINKED_LIST(llButtonExLink);

//���btn��multiButton������
void pButtonExInit(llButton *widget)
{
    llButtonExInfoTypedef * link = (llButtonExInfoTypedef *)llMalloc(sizeof(llButtonExInfoTypedef));
    if(link!=NULL)
    {
        memset(link,0,sizeof(llButtonExInfoTypedef));
        link->pressLevel=1;
        link->widget=widget;
        llLinkedListAddTail(&(link->parent_link_pos), &llButtonExLink);
    }
}

void nButtonExInit(uint16_t nameId)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeButton);
    if(widget!=NULL)
    {
        pButtonExInit(widget);
    }
}

void llButtonExProcess(llButtonExInfoTypedef *btnInfo)
{
    btnInfo->btnOldState=btnInfo->btnNewState;
    btnInfo->btnNewState=btnInfo->widget->isPressed;

    switch(btnInfo->FSM_State)
    {
    case BUTTON_EVENT_NO_OPERATION:
    {
        if((btnInfo->btnNewState==btnInfo->pressLevel)&&(btnInfo->btnNewState==btnInfo->btnOldState))
        {
//                btnInfo->FSM_State=BUTTON_PRESSED;
            btnInfo->holdCount=0;
            btnInfo->isPressed=true;
            btnInfo->FSM_State=BUTTON_EVENT_HOLD_DOWN;
        }
        break;
    }
//        case BUTTON_PRESSED:
//        {
//
//            break;
//        }
    case BUTTON_EVENT_HOLD_DOWN:
    {
        btnInfo->holdCount++;

        if((btnInfo->holdCount>(BUTTON_LONG_MS/BUTTON_TICK_MS))&&(btnInfo->shootCount==0))
        {
            btnInfo->shootCount++;
            btnInfo->isShoot=true;
        }
        else
        {
            if(((btnInfo->holdCount*BUTTON_TICK_MS-BUTTON_LONG_MS)/BUTTON_LONG_SHOOT_MS+1==btnInfo->shootCount)&&(btnInfo->shootCount>0))
            {
                btnInfo->shootCount++;
                btnInfo->isShoot=true;
            }
        }

        if((btnInfo->btnNewState!=btnInfo->pressLevel)&&(btnInfo->btnNewState==btnInfo->btnOldState))
        {
//                btnInfo->FSM_State=BUTTON_RELEASED;
            btnInfo->holdCount=0;
            btnInfo->shootCount=0;
            btnInfo->isShoot=false;
            btnInfo->isReleased=true;
            btnInfo->isClicked=true;

            btnInfo->doubleClickCount++;//��������
            if(btnInfo->doubleClickCount>=2)
            {
                btnInfo->isDoubleClicked=true;
                btnInfo->doubleClickCount=0;
            }

            btnInfo->repeatCount++;
            btnInfo->repeatTimeOutCount=0;//��λ��ʱ

            btnInfo->FSM_State=BUTTON_EVENT_NO_OPERATION;
        }
        break;
    }
//        case BUTTON_RELEASED:
//        {
//
//            break;
//        }
    default:
        break;
    }

    //������ʱ����
    if(btnInfo->repeatCount>0)//�е���ſ�ʼ���㳬ʱ
    {
        btnInfo->repeatTimeOutCount++;
        if(btnInfo->repeatTimeOutCount>=(BUTTON_CLICK_TIME_OUT_MS/BUTTON_TICK_MS))
        {
            btnInfo->repeatTimeOutCount=0;//��λ��ʱ
            btnInfo->isRepeatEnd=true;
        }
    }
}



#define BUTTON_DEBOUNCE_TICK_TIMES   (((BUTTON_DEBOUNCE_MS>=BUTTON_TICK_MS)?(BUTTON_DEBOUNCE_MS/BUTTON_TICK_MS):1)-1)


void llButtonExLoop(void)
{
    llListHead *tempPos,*safePos;
    llButtonExInfoTypedef *linkInfo;
    static uint32_t btnExTimer=0;
    static bool startFlag=false;

    if(startFlag==false)
    {
        startFlag=true;
        llTimer_start(&btnExTimer);
    }

    if(llTimer_timeOut(&btnExTimer,BUTTON_TICK_MS,true))
    {
        //ѭ���������btnEx
        list_for_each_prev_safe(tempPos,safePos, &llButtonExLink)
        {
            linkInfo = list_entry(tempPos, llButtonExInfoTypedef, parent_link_pos);
            if(linkInfo!=NULL)
            {
                llButtonExProcess(linkInfo);
            }
        }

    }
}

uint16_t nButtonExGetState(uint16_t nameId,uint8_t state)
{
    uint16_t ret=false;
    llButtonExInfoTypedef *btnExInfo=NULL;
    llListHead *tempPos,*safePos;
    llButtonExInfoTypedef *linkInfo;
//    static uint32_t btnExTimer=0;
//    static bool startFlag=false;

//    if(startFlag==false)
//    {
//        startFlag=true;
//        llTimer_start(&btnExTimer);
//    }

//    if(llTimer_timeOut(&btnExTimer,20,true))
    {
        //���� ����
        list_for_each_prev_safe(tempPos,safePos, &llButtonExLink)
        {
            linkInfo = list_entry(tempPos, llButtonExInfoTypedef, parent_link_pos);
            if(linkInfo!=NULL)
            {
                if(linkInfo->widget->nameId==nameId)
                {
                    btnExInfo=linkInfo;
                }
            }
        }

        if(btnExInfo!=NULL)
        {
            switch(state)
            {
            case BUTTON_EVENT_NO_OPERATION:
            {
                ret=(state==btnExInfo->FSM_State)?true:false;
                break;
            }
            case BUTTON_EVENT_PRESS:
            {
                if(btnExInfo->isPressed)
                {
                    btnExInfo->isPressed=false;
                    ret=true;
                }
                else
                {
                    ret=false;
                }
                break;
            }
            case BUTTON_EVENT_HOLD_DOWN:
            {
                ret=(state==btnExInfo->FSM_State)?true:false;
                break;
            }
            case BUTTON_EVENT_RELEASE:
            {
                if(btnExInfo->isReleased)
                {
                    btnExInfo->isReleased=false;
                    ret=true;
                }
                else
                {
                    ret=false;
                }
                break;
            }
            case BUTTON_EVENT_CLICK:
            {
                if(btnExInfo->isClicked)
                {
                    btnExInfo->isClicked=false;
                    ret=true;
                }
                else
                {
                    ret=false;
                }
                break;
            }
            case BUTTON_EVENT_DOUBLE_CLICK:
            {
                if(btnExInfo->isDoubleClicked)
                {
                    btnExInfo->isDoubleClicked=false;
                    ret=true;
                }
                else
                {
                    ret=false;
                }
                break;
            }
            case BUTTON_EVENT_REPEAT_TIMES:
            {
                if(btnExInfo->isRepeatEnd)
                {
                    btnExInfo->isRepeatEnd=false;
                    ret=btnExInfo->repeatCount;
                    btnExInfo->repeatCount=0;
                }
                break;
            }
            case BUTTON_EVENT_HOLD_TIME_MS:
            {
                ret=btnExInfo->holdCount*BUTTON_TICK_MS;
                break;
            }

//            case BUTTON_EVENT_LONG_START:
//            {
//                if(btnExInfo->holdCount>=(BUTTON_LONG_MS/BUTTON_TICK_MS))
//                {
//                    ret=true;
//                }
//                else
//                {
//                    ret=false;
//                }
//                break;
//            }

            case BUTTON_EVENT_LONG_SHOOT:
            {
                if(btnExInfo->isShoot==true)
                {
                    ret=true;
                    btnExInfo->isShoot=false;
                }
//                if((btnExInfo->holdCount>(BUTTON_LONG_MS/BUTTON_TICK_MS))&&(btnExInfo->shootCount==0))
//                {
//                    btnExInfo->shootCount++;
//                    ret=true;
//                }
//                else
//                {
//                    if(((btnExInfo->holdCount*BUTTON_TICK_MS-BUTTON_LONG_MS)/BUTTON_LONG_SHOOT_MS+1==btnExInfo->shootCount)&&(btnExInfo->shootCount>0))
//                    {
//                        btnExInfo->shootCount++;
//                        ret=true;
//                    }
//                }
                break;
            }
            default:
                break;
            }
        }
    }
    return ret;
}

void llButtonExClear(void)
{
    llListHead *tempPos,*safePos;
    llButtonExInfoTypedef *tempInfo;
    llButton *widget;

    list_for_each_prev_safe(tempPos, safePos,&llButtonExLink)
    {
        tempInfo = list_entry(tempPos, llButtonExInfoTypedef, parent_link_pos);
        if(tempInfo->widget!=NULL)
        {
            llLinkedListDelete(tempPos);
            llFree(tempInfo);
        }
    }
}

