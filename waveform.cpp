#include "waveform.h"
#include <QPainter>
#include <QMessageBox>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPainterPath>
#include <stdio.h>
#include <QTextCodec>
#include <QDebug>

WaveForm::WaveForm(QWidget *parent) :
    QWidget(parent)
{
    m_dat = NULL;
    m_gain = 1.0;
    enableFlag = 1;
    length = 1;
}

WaveForm::~WaveForm()
{
}

void WaveForm::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    if(enableFlag)
    {
        /************     Draw Background     ********************/
        p.fillRect(QRectF(QPointF(0,0),this->size()),QBrush(m_bgColor,Qt::SolidPattern));


        /*********** Draw Notation ******************/
        p.setPen(Qt::white);
        p.drawLine(mouseClickPos.x(),0,mouseClickPos.x(),height());
        switch(m_dataType)
        {
        case DATA_TYPE_CHAR:
            p.drawText(width()-150,20,QString::number(mouseClickPos.x()*length/width())+" | "+QString::number(((char*)m_dat)[mouseClickPos.x()*length/width()]));
            break;
        case DATA_TYPE_SHORT:
            p.drawText(width()-150,20,QString::number(mouseClickPos.x()*length/width())+" | "+QString::number(((short*)m_dat)[mouseClickPos.x()*length/width()]));
            break;
        case DATA_TYPE_FLOAT:
            p.drawText(width()-150,20,QString::number(mouseClickPos.x()*length/width())+" | "+QString::number(((float*)m_dat)[mouseClickPos.x()*length/width()]));
            break;
        case DATA_TYPE_DOUBLE:
            p.drawText(width()-150,20,QString::number(mouseClickPos.x()*length/width())+" | "+QString::number(((double*)m_dat)[mouseClickPos.x()*length/width()]));
            break;
        }


        /***********     Draw Curve       ************/
        if(m_dat)
        {
            //calc draw point
            double interval = (double)this->size().width()/length;
            double pos_v_mid = this->size().height()/2;
            //draw curve
            p.setPen(m_curveColor);
            QPainterPath path;
            path.moveTo(0,pos_v_mid);

            switch(m_dataType)
            {
            case DATA_TYPE_CHAR:
                for(int j = 0; j< length; j++)
                {
                    path.lineTo(j*interval,pos_v_mid - ((char*)m_dat)[j] * m_gain);
                };
                break;
            case DATA_TYPE_SHORT:
                for(int j = 0; j< length; j++)
                {
                    path.lineTo(j*interval,pos_v_mid - ((short*)m_dat)[j] * m_gain);
                };
                break;
            case DATA_TYPE_FLOAT:
                for(int j = 0; j< length; j++)
                {
                    path.lineTo(j*interval,pos_v_mid - ((float*)m_dat)[j] * m_gain);
                };
                break;
            case DATA_TYPE_DOUBLE:
                for(int j = 0; j< length; j++)
                {
                    path.lineTo(j*interval,pos_v_mid - ((double*)m_dat)[j] * m_gain);
                };
                break;
            }


            p.drawPath(path);
        }
    }
    else
    {
        p.fillRect(QRectF(QPointF(0,0),this->size()),QBrush(QColor(128,128,128),Qt::SolidPattern));
        p.setFont(QFont("",50));
        p.drawText(100,100,tr("暂停"));
    }
}

void WaveForm::setBackgoundColor(QColor clr)
{
    m_bgColor = clr;
}

void WaveForm::setCurveColor(QColor clr)
{
    m_curveColor = clr;
}

void WaveForm::setDataPointer(void *dat,int type)
{
    m_dat = dat;
    m_dataType = type;
}

void WaveForm::mouseReleaseEvent(QMouseEvent *event)
{
    //QMessageBox::warning(this,"Mouse Clicked",QString::number(event->x()));
    if(event->button() == Qt::RightButton)
        mouseClickPos = event->pos();
    update();
}

void WaveForm::wheelEvent(QWheelEvent *event)
{
    //QMessageBox::warning(this,"Mouse Wheel",QString::number(event->delta()));
    if(enableFlag)
    {
        if(event->delta() > 0)
            m_gain *= 2;
        else
            m_gain /= 2;
        this->update();
    }
}

void WaveForm::mouseDoubleClickEvent(QMouseEvent *event)
{
    enableFlag = !enableFlag;
    update();
}

void WaveForm::setDrawLength(int l)
{
    length = l;
    update();
}
