#include "streambox.h"
#include <iostream>
#include <QHoverEvent>
#include <stdlib.h>
streambox::streambox(QWidget *parent)
    : QCheckBox(parent)
{
    this->setAttribute(Qt::WA_Hover, true);
}

void streambox::mousePressEvent(QMouseEvent *event)
{
    int xn = rand()/float(RAND_MAX) * 800;
    int yn = rand()/float(RAND_MAX) * 500;
    this->move(xn,yn);

}
