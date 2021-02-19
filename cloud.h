#ifndef CLOUD_H
#define CLOUD_H

#include <QLabel>

QT_BEGIN_NAMESPACE
class QDragEnterEvent;
class QDropEvent;
QT_END_NAMESPACE

//! [0]
class cloud : public QLabel
{
public:
    explicit cloud(QWidget *parent = nullptr);

};
//! [0]

#endif // CLOUD_H
