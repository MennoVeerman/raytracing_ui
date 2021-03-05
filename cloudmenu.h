#ifndef CLOUDMENU_H
#define CLOUDMENU_H

#include <QFrame>
#include <QLabel>
#include <QPoint>

QT_BEGIN_NAMESPACE
class QDragEnterEvent;
class QDropEvent;
QT_END_NAMESPACE

class Cloudmenu : public QFrame
{
public:
    explicit Cloudmenu(QWidget *parent = nullptr);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

};



#endif // CLOUDMENU_H
