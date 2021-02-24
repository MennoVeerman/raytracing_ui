#include <QtWidgets>
#include "grid.h"
#include <iostream>
#include <cmath>
grid::grid(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(200, 200);
    this->setStyleSheet("background-color: rgba(0,0,0,0)");
    this->setAttribute(Qt::WA_TransparentForMouseEvents);
}

void grid::paintEvent(QPaintEvent *event)
{
    float sza = this->angle/180*4*atan(1);

    float w = float(this->size().width());
    float h = float(this->size().height());
    float ds = 100.f;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::gray, 1));

    float dx = ds/cos(sza);
    float ix = 0;
    while (ix < w)
    {
        painter.drawLine(ix, 0, ix+h*tan(sza), h);
        ix += dx;
    }

    float dy = ds/sin(sza);
    float iy = dy;
    while (iy < h)
    {
        painter.drawLine(0, iy, w, iy+w/tan(sza));
        iy += dy;
    }
}

