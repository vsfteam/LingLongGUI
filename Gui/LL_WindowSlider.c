#include "LL_WindowSlider.h"
#include "LL_Linked_List.h"
#include "LL_Handler.h"

void llWindowSliderAction(void *widget, uint8_t touchSignal);
void nWindowSliderDelete(uint16_t nameId);
void pWindowSliderRefresh(llWindowSlider *widget);
void nWindowSliderRefresh(uint16_t nameId);

bool slotWindowSliderPressed(llConnectInfo info)
{
    llWindowSlider * widget;
    int16_t x,y;
    widget=(llWindowSlider*)info.receiver;

    if(llClickGetPoint(&x,&y)==true)
    {
        widget->clickPoint.x=x;
        widget->clickPoint.y=y;
        return true;
    }
    return false;
}

bool slotWindowSliderReleased(llConnectInfo info)
{
    llWindowSlider * widget;
    uint8_t itemNumMod,minShowItemCount;

    widget=(llWindowSlider*)info.receiver;

    if(widget->isPageMove)
    {
        if((widget->moveOffset>30)||(widget->moveOffset<-30))
        {
            //下一页
            if(widget->isHorizontalScroll)//水平
            {
                if(widget->moveOffset<0)
                {
                    if(widget->page<(widget->pageMax-1))
                    {
                        widget->page++;
                    }
                }
                else
                {
                    if(widget->page>0)
                    {
                        widget->page--;
                    }
                }
                widget->moveX=-widget->page*(widget->geometry.width+widget->pageSpacing);
            }
            else
            {
                if(widget->moveOffset<0)
                {
                    if(widget->page<(widget->pageMax-1))
                    {
                        widget->page++;
                    }
                }
                else
                {
                    if(widget->page>0)
                    {
                        widget->page--;
                    }
                }
                widget->moveY=-widget->page*(widget->geometry.height+widget->pageSpacing);
            }
        }
        else
        {
            //还原
            if(widget->isHorizontalScroll)//水平
            {
                widget->moveX=-widget->page*(widget->geometry.width+widget->pageSpacing);
            }
            else
            {
                widget->moveY=-widget->page*(widget->geometry.height+widget->pageSpacing);
            }
        }
    }
    else
    {
        if((widget->moveOffset>30)||(widget->moveOffset<-30))
        {
            if(widget->isHorizontalScroll)//水平
            {
                if(widget->moveOffset<0)//向左拖动
                {
                    widget->itemNum=(-widget->moveX)/widget->itemWidth;
                    itemNumMod=(-widget->moveX)%widget->itemWidth;
                    if(itemNumMod>(widget->itemWidth>>1))
                    {
                        widget->itemNum++;
                    }

                    //计算能显示几个图片
                    minShowItemCount=widget->geometry.width/widget->itemWidth;

                    if(widget->imageCount>minShowItemCount)
                    {
                        if(widget->itemNum>=(widget->imageCount-minShowItemCount))
                        {
                            widget->itemNum=widget->imageCount-minShowItemCount;
                        }
                        widget->moveX=-widget->itemNum*widget->itemWidth;
                    }
                    else
                    {
                        widget->moveX=0;
                        widget->itemNum=0;
                    }
                }
                else//向右拖动
                {
                    if(widget->moveX>0)
                    {
                        widget->moveX=0;
                        widget->itemNum=0;
                    }
                    else
                    {
                    widget->itemNum=(-widget->moveX)/widget->itemWidth;
                    itemNumMod=(-widget->moveX)%widget->itemWidth;
                    if(itemNumMod>(widget->itemWidth>>1))
                    {
                        widget->itemNum++;
                    }
                    if(widget->itemNum>=widget->imageCount)
                    {
                        widget->itemNum=widget->imageCount-1;
                    }

                    widget->moveX=-widget->itemNum*widget->itemWidth;
                    }
                }
            }
            else
            {
                if(widget->moveOffset<0)//向上拖动
                {
                    widget->itemNum=(-widget->moveY)/widget->itemHeight;
                    itemNumMod=(-widget->moveY)%widget->itemHeight;
                    if(itemNumMod>(widget->itemHeight>>1))
                    {
                        widget->itemNum++;
                    }

                    //计算能显示几个图片
                    minShowItemCount=widget->geometry.height/widget->itemHeight;

                    if(widget->imageCount>minShowItemCount)
                    {
                        if(widget->itemNum>=(widget->imageCount-minShowItemCount))
                        {
                            widget->itemNum=widget->imageCount-minShowItemCount;
                        }
                        widget->moveY=-widget->itemNum*widget->itemHeight;
                    }
                    else
                    {
                        widget->moveY=0;
                        widget->itemNum=0;
                    }
                }
                else//向下拖动
                {
                    if(widget->moveY>0)
                    {
                        widget->moveY=0;
                        widget->itemNum=0;
                    }
                    else
                    {
                    widget->itemNum=(-widget->moveY)/widget->itemHeight;
                    itemNumMod=(-widget->moveY)%widget->itemHeight;
                    if(itemNumMod>(widget->itemHeight>>1))
                    {
                        widget->itemNum++;
                    }
                    if(widget->itemNum>=widget->imageCount)
                    {
                        widget->itemNum=widget->imageCount-1;
                    }

                    widget->moveY=-widget->itemNum*widget->itemHeight;
                    }
                }
            }
        }
        else
        {
            //还原
            if(widget->isHorizontalScroll)//水平
            {
                widget->moveX=-widget->itemNum*widget->itemWidth;
            }
            else
            {
                widget->moveY=-widget->itemNum*widget->itemHeight;
            }
        }
    }

    widget->clickPoint.x=0;
    widget->clickPoint.y=0;

    pWindowSliderRefresh(widget);

    return false;
}

bool slotWindowSliderMove(llConnectInfo info)
{
    llWindowSlider * widget;
    int16_t nowX,nowY;
    bool clickState;

    widget=(llWindowSlider*)info.receiver;

    clickState=llClickGetPoint(&nowX,&nowY);
    if(clickState==true)
    {
        if(widget->isPageMove)
        {
            if(widget->isHorizontalScroll)//水平
            {
                widget->moveOffset=nowX-widget->clickPoint.x;
                widget->moveX=widget->moveOffset-(widget->page*(widget->geometry.width+widget->pageSpacing));
                pWindowSliderRefresh(widget);
            }
            else
            {
                widget->moveOffset=nowY-widget->clickPoint.y;
                widget->moveY=widget->moveOffset-(widget->page*(widget->geometry.height+widget->pageSpacing));
                pWindowSliderRefresh(widget);
            }
        }
        else
        {
            if(widget->isHorizontalScroll)//水平
            {
                widget->moveOffset=nowX-widget->clickPoint.x;
                widget->moveX=widget->moveOffset-(widget->itemNum*widget->itemWidth);
                pWindowSliderRefresh(widget);
            }
            else
            {
                widget->moveOffset=nowY-widget->clickPoint.y;
                widget->moveY=widget->moveOffset-(widget->itemNum*widget->itemHeight);
                pWindowSliderRefresh(widget);
            }
        }
    }
    return false;
}

llWindowSlider *llWindowSliderQuickCreate(uint16_t nameId, uint16_t parentNameId,
                                          int16_t x, int16_t y, int16_t width, int16_t height,
                                          uint8_t rowCount,uint8_t columnCount,
                                          uint8_t itemWidth,uint8_t itemHeight,
                                          llHorizontalAlign hAlign,llVerticalAlign vAlign,
                                          uint8_t pageMax,uint8_t pageSpacing,bool isPageMove,
                                          bool isHorizontalScroll,bool isHidden)
{
    llWindowSlider * pNewWidget = NULL;
    llListWidgetInfo *parentInfo;
    uint32_t *imageAddrBuf;

    //检查父链表存在
    if(llList_GetInfoByName(&parentInfo,parentNameId)==true)
    {
        pNewWidget = LL_MALLOC_WIDGET_INFO(llWindowSlider);
        imageAddrBuf = llMalloc(sizeof (uint32_t)*rowCount*columnCount*pageMax);
        if((pNewWidget!=NULL)&&(imageAddrBuf!=NULL))
        {
            pNewWidget->nameId=nameId;

            pNewWidget->parentType=((llGeneral*)(parentInfo->widget))->widgetType;
            pNewWidget->parentWidget=parentInfo->widget;

            pNewWidget->widgetType=widgetTypeWindowSlider;
            pNewWidget->geometry.x=x;
            pNewWidget->geometry.y=y;
            pNewWidget->geometry.width=width;
            pNewWidget->geometry.height=height;

            pNewWidget->moveX=0;
            pNewWidget->moveY=0;

            pNewWidget->rowCount=rowCount;
            pNewWidget->columnCount=columnCount;
            pNewWidget->itemWidth=itemWidth;
            pNewWidget->itemHeight=itemHeight;
            pNewWidget->hAlign=hAlign;
            pNewWidget->vAlign=vAlign;

            pNewWidget->deleteFunc=nWindowSliderDelete;
            pNewWidget->actionFunc=llWindowSliderAction;
            pNewWidget->refreshFunc=nWindowSliderRefresh;
            pNewWidget->isHidden=isHidden;
            pNewWidget->isEnable=true;
            pNewWidget->pageMax=pageMax;
            pNewWidget->imageCount=0;
            pNewWidget->isHorizontalScroll=isHorizontalScroll;
            pNewWidget->page=0;
            pNewWidget->imageAddr=imageAddrBuf;
            pNewWidget->pageSpacing=pageSpacing;
            pNewWidget->isPageMove=isPageMove;
            //add linked list
            llListWidgetAdd(&(parentInfo->child_link),pNewWidget);

            //动作
            llConnectSignal(nameId,SIGNAL_CLICK_PRESSED,nameId,slotWindowSliderPressed);
            llConnectSignal(nameId,SIGNAL_CLICK_HOLD_MOVE,nameId,slotWindowSliderMove);
            llConnectSignal(nameId,SIGNAL_CLICK_RELEASED,nameId,slotWindowSliderReleased);
        }
        else
        {
            llFree(pNewWidget);
            llFree(imageAddrBuf);
        }
    }

    return pNewWidget;
}

llWindowSlider *llWindowSliderCreate(uint16_t nameId, uint16_t parentNameId,
                                     int16_t x, int16_t y, int16_t width, int16_t height,
                                     uint8_t rowCount,uint8_t columnCount,
                                     uint8_t pageMax,uint8_t pageSpacing,bool isPageMove,
                                     bool isHorizontalScroll,bool isHidden)
{
    return llWindowSliderQuickCreate(nameId,parentNameId,x,y,width,height,rowCount,columnCount,
                                     (uint8_t)(width/rowCount),(uint8_t)(height/columnCount),
                                     llAlignHCenter,llAlignVCenter,
                                     pageMax,pageSpacing,isPageMove,
                                     isHorizontalScroll,isHidden);
}


void llWindowSliderAction(void *widget,uint8_t touchSignal)
{
    if(((llWindowSlider*)widget)->isEnable)
    {
        switch(touchSignal)
        {
        case SIGNAL_CLICK_PRESSED:
        case SIGNAL_CLICK_RELEASED:
        case SIGNAL_CLICK_HOLD_MOVE:
        {
            llEmitSignal(widget,touchSignal);
            break;
        }
        default:
            break;
        }
    }
}

void pWindowSliderFree(llWindowSlider *widget)
{
    llFree(widget->imageAddr);
    llFree(widget);
}

void nWindowSliderDelete(uint16_t nameId)
{

}

void pWindowSliderRefresh(llWindowSlider *widget)
{
    llPoint globalPos;
    int16_t x,y;
    uint8_t page;
    uint8_t i;
    llGeometry imgGeometry,showGeometry;
    llSize imgSize;
    uint16_t skipWidth;

    if(widget->isHidden==false)
    {
        globalPos=llListGetGlobalPos((llGeneral*)widget);

        //空余区域填充
        if((widget->moveX>0)||(widget->moveY>0))//左侧，上方
        {
            imgGeometry.x=globalPos.x;
            imgGeometry.y=globalPos.y;
            if(widget->moveX>0)
            {
                imgGeometry.width=widget->moveX;
                imgGeometry.height=widget->geometry.height;

                if( llRectIntersect(widget->geometry, imgGeometry,&showGeometry))
                {
                    llGeneralWidgetParentRecover((llGeneral*)widget,showGeometry);
                }
            }
            else
            {
                imgGeometry.width=widget->geometry.width;
                imgGeometry.height=widget->moveY;
                if( llRectIntersect(widget->geometry, imgGeometry,&showGeometry))
                {
                    llGeneralWidgetParentRecover((llGeneral*)widget,showGeometry);
                }
            }
        }
        else//右侧，下方
        {
            if(widget->isHorizontalScroll)//水平
            {
                if(widget->isPageMove)
                {
                    imgGeometry.x=globalPos.x+widget->moveX+widget->geometry.width+(widget->pageMax-1)*(widget->geometry.width+widget->pageSpacing);
                }
                else
                {
                    imgGeometry.x=globalPos.x+widget->moveX+widget->itemWidth*widget->imageCount;
                }
                if(imgGeometry.x<(widget->geometry.x+widget->geometry.width))
                {
                    imgGeometry.y=globalPos.y;
                    imgGeometry.width=widget->geometry.x+widget->geometry.width-imgGeometry.x;
                    imgGeometry.height=widget->geometry.height;
                    if( llRectIntersect(widget->geometry, imgGeometry,&showGeometry))
                    {
                        llGeneralWidgetParentRecover((llGeneral*)widget,showGeometry);
                    }
                }
            }
            else
            {
                if(widget->isPageMove)
                {
                    imgGeometry.y=globalPos.y+widget->moveY+widget->geometry.height+(widget->pageMax-1)*(widget->geometry.height+widget->pageSpacing);
                }
                else
                {
                    imgGeometry.y=globalPos.y+widget->moveY+widget->itemHeight*widget->imageCount;
                }

                if(imgGeometry.y<(widget->geometry.y+widget->geometry.height))
                {
                    imgGeometry.x=globalPos.x;
                    imgGeometry.width=widget->geometry.width;
                    imgGeometry.height=widget->geometry.y+widget->geometry.height-imgGeometry.y;
                    if( llRectIntersect(widget->geometry, imgGeometry,&showGeometry))
                    {
                        llGeneralWidgetParentRecover((llGeneral*)widget,showGeometry);
                    }
                }
            }
        }

        //填充中间空余区域
        for(i=0;i<(widget->pageMax-1);i++)
        {
            if(widget->isHorizontalScroll)//水平
            {
                imgGeometry.x=globalPos.x+widget->moveX+widget->geometry.width+(i*(widget->geometry.width+widget->pageSpacing));
                imgGeometry.y=globalPos.y;
                imgGeometry.width=widget->pageSpacing;
                imgGeometry.height=widget->geometry.height;

                if( llRectIntersect(widget->geometry, imgGeometry,&showGeometry))
                {
                    llGeneralWidgetParentRecover((llGeneral*)widget,showGeometry);
                }
            }
            else
            {
                imgGeometry.x=globalPos.x;
                imgGeometry.y=globalPos.y+widget->moveY+widget->geometry.height+(i*(widget->geometry.height+widget->pageSpacing));
                imgGeometry.width=widget->geometry.width;
                imgGeometry.height=widget->pageSpacing;

                if( llRectIntersect(widget->geometry, imgGeometry,&showGeometry))
                {
                    llGeneralWidgetParentRecover((llGeneral*)widget,showGeometry);
                }
            }
        }

        for(i=0;i<(widget->rowCount*widget->columnCount*widget->pageMax);i++)
        {
            //计算坐标
            //页
            page=i/(widget->rowCount*widget->columnCount);

            //窗口内坐标
            x=(i%widget->columnCount)*widget->itemWidth;
            y=(i/widget->columnCount)*widget->itemHeight;

            if(widget->isPageMove)
            {
                skipWidth=widget->geometry.width;
            }
            else
            {
                skipWidth=widget->itemWidth;
            }

            //换页坐标
            if(widget->isHorizontalScroll)
            {
                x+=page*(skipWidth+widget->pageSpacing);
                y-=page*widget->geometry.height;
            }
            else
            {
                y+=page*widget->pageSpacing;
            }


            //全局坐标
            x+=globalPos.x+widget->moveX;
            y+=globalPos.y+widget->moveY;

            if(i<widget->imageCount)
            {
                imgGeometry.x=x;
                imgGeometry.y=y;

                imgSize=llGeneralGetImageSize(widget->imageAddr[i]);
                imgGeometry.width=imgSize.width;
                imgGeometry.height=imgSize.height;

                if( llRectIntersect(widget->geometry, imgGeometry,&showGeometry))
                {
                    llGeneralImageSpecificAreaShow(imgGeometry, showGeometry,widget->imageAddr[i]);
                }
            }
            else
            {
                imgGeometry.x=x;
                imgGeometry.y=y;
                imgGeometry.width=widget->itemWidth;
                imgGeometry.height=widget->itemHeight;
                if( llRectIntersect(widget->geometry, imgGeometry,&showGeometry))
                {
                    llGeneralWidgetParentRecover((llGeneral*)widget,showGeometry);
                }
            }
        }

    }
}

void nWindowSliderRefresh(uint16_t nameId)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeWindowSlider);

    if(widget!=NULL)
    {
        pWindowSliderRefresh((llWindowSlider*)widget);
    }
}

void pWindowSliderAddImage(llWindowSlider *widget,uint32_t imageAddr)
{
    if(widget->imageCount<(widget->rowCount*widget->columnCount*widget->pageMax))
    {
        widget->imageAddr[widget->imageCount++]=imageAddr;
    }
}

void nWindowSliderAddImage(uint16_t nameId,uint32_t imageAddr)
{
    void *widget;
    widget=llGeneralGetWidget(nameId,widgetTypeWindowSlider);
    if(widget!=NULL)
    {
        pWindowSliderAddImage((llWindowSlider*)widget,imageAddr);
    }
}
