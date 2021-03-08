#include <QtWidgets>
#include <iostream>
#include "cloudmenu.h"
#include "cloud.h"

//! [0]
Cloudmenu::Cloudmenu(QWidget *parent)
    : QFrame(parent)
{
    setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
    setAcceptDrops(true);

    Cloud *cloud = new Cloud(this);
    cloud->set_icon("cloud");
    cloud->move(10,10);
    cloud->show();
    cloud->setAttribute(Qt::WA_DeleteOnClose);
    cloud->setStyleSheet("background-color: rgba(0,0,0,0);");
    this->setStyleSheet("background-color: rgb(255,255,255)");
}

void Cloudmenu::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}

void Cloudmenu::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}

void Cloudmenu::dropEvent(QDropEvent *event)
{
    event->ignore();
}

//! [1]
void Cloudmenu::mousePressEvent(QMouseEvent *event)
{
    Cloud *child = static_cast<Cloud*>(childAt(event->position().toPoint()));

    if (!child)
        return;


    QPixmap pixmap = child->pixmap(Qt::ReturnByValue);
    QString name = child->name;
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << name << QPoint(666,999) << QPoint(666,999);
//    dataStream << name << QPoint(event->position().toPoint() - child->pos());
//! [1]

//! [2]
    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/x-dnditemdata", itemData);
//! [2]

//! [3]
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(pixmap);
    drag->setHotSpot(event->position().toPoint() - child->pos());
//! [3]

    QPixmap tempPixmap = pixmap;

    child->setPixmap(tempPixmap);
    if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction) {
        child->close();
    } else {
        child->show();
        child->setPixmap(pixmap);
    }
}
