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
    std::cout<<"Z"<<std::endl;
}


void Cloud::update_image(QString name)
{
    QByteArray bname = name.toLocal8Bit();
    const char *cname = bname.data();
    std::string name_str = ":/images/"+std::string(cname)+".svg";
    const char *imagename = name_str.c_str();

//    QByteArray bname = image_name.toLocal8Bit();
//    const char *name = bname.data();
    setPixmap(QPixmap(imagename));


}
