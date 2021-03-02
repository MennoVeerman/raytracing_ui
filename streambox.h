#ifndef STREAMBOX_H
#define STREAMBOX_H
#include <QCheckBox>
#include <iostream>

class streambox : public QCheckBox
{
    public:
        explicit streambox(QWidget *parent = nullptr);
    protected:
        void enterEvent(QEvent *event) ;
        void mousePressEvent(QMouseEvent *event);

};

#endif // STREAMBOX_H
