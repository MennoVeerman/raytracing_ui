#include "plotarea.h"
#include <iostream>
#include <QtCore/QtCore>

plotarea::plotarea(QWidget *parent) : QWidget(parent)
{
    this->setStyleSheet("background-color: rgba(255,255,255,255);");

    w = this->size().width();
    h = this->size().height();

}

void plotarea::paintEvent(QPaintEvent *event)
{

    w = this->size().width();
    h = this->size().height();

    QPainter txtpainter(this);
    txtpainter.setRenderHint(QPainter::Antialiasing);
    txtpainter.setPen(QPen(Qt::black, 1.5));
    txtpainter.translate(0,h);
    txtpainter.rotate(-90);
    txtpainter.drawText(3,5,h,10, Qt::AlignCenter,tr("#photons (sfc/toa)"));

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::gray, 1.5));
    painter.drawLine(ddx,ddy,ddx,h-ddy);
    painter.drawLine(ddx,h-ddy,w,h-ddy);//    painter.setPen(QPen(Qt::black, 1.5));
//    painter.drawText(ddx-18, h-ddy-10, 15, 15, Qt::AlignRight, tr("0"));
//    painter.drawText(ddx-18, 10, 15, 15, Qt::AlignRight,tr("1"));
    if (nx > 0)
    {
        float cmin = 0.;
        float cmax = global[0];
        for (int i=0; i<nx; ++i)
            cmax = std::max(std::max(std::max(cmax,global[i]),diffuse[i]),direct[i]);
        cmax = std::max(1.f, cmax);
//        const float tick_y = (h-ddy)-(h-2*ddy)/cmax;
//        painter.drawLine(ddx,tick_y,ddx+5,tick_y);


        // Global
        if (do_global)
        {
            QPointF *globalpoints = new QPointF[nx];
            for (int i=0; i<nx; ++i)
            {
                const int ix = (float(dx)/2) + ddx+float(i)/nx*(w-ddx);
                const int iy = h-ddy-(h-2*ddy)*(global[i]-cmin)/(cmax-cmin);
                globalpoints[i] = QPointF(ix,iy);
            }
            painter.setPen(QPen(Qt::black, 2));
            painter.drawPolyline(globalpoints,nx);
        }

        // Direct
        if (do_direct)
        {
            QPointF *directpoints = new QPointF[nx];
            for (int i=0; i<nx; ++i)
            {
                const int ix = (float(dx)/2) + ddx+float(i)/nx*(w-ddx);
                const int iy = h-ddy-(h-2*ddy)*(direct[i]-cmin)/(cmax-cmin);
                directpoints[i] = QPointF(ix,iy);
            }
            painter.setPen(QPen(QColor(69,106,216), 2));
            painter.drawPolyline(directpoints,nx);
        }

        // Diffuse
        if (do_diffuse)
        {
            QPointF *diffusepoints = new QPointF[nx];
            for (int i=0; i<nx; ++i)
            {
                const int ix = (float(dx)/2) + ddx+float(i)/nx*(w-ddx);
                const int iy = h-ddy-(h-2*ddy)*(diffuse[i]-cmin)/(cmax-cmin);
                diffusepoints[i] = QPointF(ix,iy);
            }
            painter.setPen(QPen(QColor(242,212,62), 2));
            painter.drawPolyline(diffusepoints,nx);
        }

    }
}

checkbox_line::checkbox_line(QWidget *parent) : QCheckBox(parent)
{
}
void checkbox_line::paintEvent(QPaintEvent *event)
{
    QCheckBox::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(this->linecolor, 3));
    painter.drawLine(60,9.5,75,9.5);
}
