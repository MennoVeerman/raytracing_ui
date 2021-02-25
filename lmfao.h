#ifndef LMFAO_H
#define LMFAO_H
#include <QCheckBox>
#include <iostream>

class lmfao : public QCheckBox
{
    public:
        explicit lmfao(QWidget *parent = nullptr);
    protected:
    void enterEvent(QEvent *event) ;
//        std::cout<<"X"<<std::endl;

//    }
        void mousePressEvent(QMouseEvent *event);

};

#endif // LMFAO_H
