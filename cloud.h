#ifndef CLOUD_H
#define CLOUD_H

#include <QLabel>
#include <string>

class Cloud : public QLabel
{
public:
    explicit Cloud(QWidget *parent = nullptr);
    void set_icon(std::string obj_type);
    void update_image(QString name);

    QString name;
    int width = -1;
    int height = -1;


};

#endif // CLOUD_H
