#include <QtWidgets>
#include <QMenu>
#include <iostream>
#include "atmosphere.h"
#include "cloud.h"

//! [0]
atmosphere::atmosphere(QWidget *parent)
    : QFrame(parent)
{
    setMinimumSize(200, 200);
    setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
    setAcceptDrops(true);

    this->setStyleSheet("background-color: rgb(192,191,255)");

//    this->setContextMenuPolicy(Qt::CustomContextMenu);
//    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
//            this, SLOT(ShowContextMenu(const QPoint &)));
}
//! [0]
//!
//!
void atmosphere::add_cloud()
{
    QLabel *cloud = new QLabel(this);
    //cloud->
    cloud->setPixmap(QPixmap(":/images/cloud.svg"));
    cloud->move(10, 10);
    cloud->show();
    cloud->setAttribute(Qt::WA_DeleteOnClose);
    cloud->setStyleSheet("background-color: rgba(0,0,0,0);");

}

void atmosphere::remove_cloud()
{
    QLabel *child = static_cast<QLabel*>(childAt(this->last_click));
    if (!child)
        return;
    child->close();

}

void atmosphere::remove_all_clouds()
{
    QList<QObject*> clouds = this->children();

    for (int i=0; i<clouds.size(); ++i)
        static_cast<QLabel*>(clouds[i])->close();

}
void atmosphere::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu;
    menu.addAction(tr("Delete"));
    QAction*  selAct = menu.exec(event->globalPos());

    if (selAct!=0)
    {
        this->last_click = event->pos();
        std::cout<<this->last_click.x()<<" "<<this->last_click.y()<<std::endl;
        remove_cloud();
    }
    QWidget::contextMenuEvent(event);
}

void atmosphere::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_Delete:
        {
            Cloud *child = static_cast<Cloud*>(childAt(this->last_click));
            if (!child)
                return;
            child->close();
            break;
        }
    case Qt::Key_Backspace:
        {
            Cloud *child = static_cast<Cloud*>(childAt(this->last_click));
            if (!child)
                return;
            child->close();
            break;
        }
    }
}

void atmosphere::wheelEvent(QWheelEvent *event)
{
    QPoint mouse = QWidget::mapFromGlobal(QCursor::pos());
    const int spin = event->angleDelta().y();

    Cloud *child = static_cast<Cloud*>(childAt(mouse));

    if (!child)
        return;
    const int width_cur = child->width;
    const int height_cur = child->height;
    const int x = child->pos().x();
    const int y = child->pos().y();
    if (spin > 0)
    {
        child->width = int(std::ceil(child->width * 1.1));
        child->height = int(std::ceil(child->height * 1.1));
        child->update_image(child->name);
    }
    else if (spin < 0)
    {
        child->width = int(std::ceil(child->width / 1.1));
        child->height = int(std::ceil(child->height / 1.1));
        child->update_image(child->name);
    }
    child->move(x-(child->width-width_cur)/2, y-(child->height-height_cur)/2);
}

void pinchTriggered(QPinchGesture *gesture)
{
    QPinchGesture::ChangeFlags changeFlags = gesture->changeFlags();
    if (changeFlags & QPinchGesture::ScaleFactorChanged)
    {
        std::cout<<gesture->totalScaleFactor()<<std::endl;
    }
}

void atmosphere::dragEnterEvent(QDragEnterEvent *event)
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

void atmosphere::dragMoveEvent(QDragMoveEvent *event)
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


void atmosphere::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {

        QByteArray itemData = event->mimeData()->data("application/x-dnditemdata");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        QString name;
        QPoint offset;
        QPoint cur_size;
        dataStream >> name >> offset >> cur_size;

        Cloud *newIcon = new Cloud(this);


        if (offset == QPoint(666,999))
        {
            newIcon->update_image(name);
            offset = QPoint(newIcon->pixmap().size().width()/2,newIcon->pixmap().size().height()/2);
            newIcon->move(event->position().toPoint() - offset);
        }
        else
        {
            newIcon->width = cur_size.x();
            newIcon->height = cur_size.y();
            newIcon->update_image(name);
            newIcon->move(event->position().toPoint() - offset);
        }
        newIcon->show();


        newIcon->setAttribute(Qt::WA_DeleteOnClose);
        newIcon->setStyleSheet("background-color: rgba(0,0,0,0);");

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

//! [1]
void atmosphere::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        this->last_click = event->position().toPoint();
        Cloud *child = static_cast<Cloud*>(childAt(this->last_click));
        if (!child)
            return;

        QPixmap pixmap = child->pixmap(Qt::ReturnByValue);
        QString name = child->name;
        QByteArray itemData;
        QDataStream dataStream(&itemData, QIODevice::WriteOnly);
        QPoint cur_size(child->width, child->height);
        dataStream << name << QPoint(event->position().toPoint() - child->pos()) << cur_size;
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
        QPainter painter;
        painter.begin(&tempPixmap);
        painter.fillRect(pixmap.rect(), QColor(127, 127, 127, 127));
        painter.end();

        child->setPixmap(tempPixmap);
        if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction) {
            child->close();
        } else {
            child->show();
            child->setPixmap(pixmap);
        }
    }
}
