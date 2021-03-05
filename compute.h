#ifndef COMPUTE_H
#define COMPUTE_H

#include <QThread>

class Compute : public QThread
{
    Q_OBJECT
public:
    Compute();
protected:
    void run();
signals:
    void lets_paint();
};

#endif // COMPUTE_H

