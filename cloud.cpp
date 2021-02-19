#include <QtWidgets>

#include "cloud.h"

//! [0]
cloud::cloud(QWidget *parent)
    : QLabel(parent)
{
   // this->setPixmap(QPixmap(":/images/cloud.png"));
    this->move(10, 10);
    this->show();
    this->setAttribute(Qt::WA_DeleteOnClose);

}
//! [0]
