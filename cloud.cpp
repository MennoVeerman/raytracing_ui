#include "cloud.h"
#include <QtWidgets>
#include <iostream>

Cloud::Cloud(QWidget* parent)
    : QLabel(parent)
{

}

void Cloud::set_icon(std::string name)
{
    this->name = QString::fromStdString(name);
    std::string iconname_str = ":/images/"+name+"_icon.svg";
    const char *iconname = iconname_str.c_str();

    setPixmap(QPixmap(iconname));
}


void Cloud::update_image(QString name)
{
    this->name = name;
    QByteArray bname = name.toLocal8Bit();
    const char *cname = bname.data();
    std::string name_str = ":/images/"+std::string(cname)+".svg";
    const char *imagename = name_str.c_str();
    if (this->width == -1)
    {
        setPixmap(QPixmap(imagename));
        this->width = this->pixmap().size().width();
        this->height = this->pixmap().size().height();

    }
    else
    {
        this->resize(this->width, this->height);
        setPixmap(QPixmap(imagename).scaledToWidth(this->width, Qt::SmoothTransformation));
    }
}

