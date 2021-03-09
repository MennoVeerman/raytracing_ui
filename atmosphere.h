#ifndef ATMOSPHERE_H
#define ATMOSPHERE_H

#include <QFrame>
#include <QLabel>
#include <QPoint>
#include <QPinchGesture>

QT_BEGIN_NAMESPACE
class QDragEnterEvent;
class QDropEvent;
QT_END_NAMESPACE

//! [0]
class atmosphere : public QFrame
{
public:
    explicit atmosphere(QWidget *parent = nullptr);
    void add_cloud();
    void remove_cloud();
    void remove_all_clouds();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    QPoint last_click;
    void pinchTriggered(QPinchGesture*);
};
//! [0]

#endif // ATMOSPHERE_H
