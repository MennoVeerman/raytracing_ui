#ifndef PLOTAREA_H
#define PLOTAREA_H

#include <QWidget>
#include <QPainter>
#include <QCheckBox>
#include <vector>

class plotarea : public QWidget
{
public:
    explicit plotarea(QWidget *parent = nullptr);
    int nx = 0;
    int dx = 0;
    float* global;
    float* direct;
    float* diffuse;
    bool do_global  = true;
    bool do_direct  = true;
    bool do_diffuse = true;

protected:
    void paintEvent(QPaintEvent *event) override;
    const int ddy = 5;
    const int ddx = 30;
    int w;
    int h;
};

class checkbox_line : public QCheckBox
{
public:
    explicit checkbox_line(QWidget *parent = nullptr);
    QColor linecolor;
protected:
    void paintEvent(QPaintEvent *event) override;

};

#endif // PLOTAREA_H
