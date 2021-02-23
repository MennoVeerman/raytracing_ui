#ifndef GRID_H
#define GRID_H

#include <QPainter>
#include <QWidget>

class grid : public QWidget
{
    public:
        explicit grid(QWidget *parent = nullptr);
        float angle;
    protected:
        void paintEvent(QPaintEvent *event) override;


};


#endif // GRID_H
