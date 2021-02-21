#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "integrate.h"
#include "helper_functions.h"
#include "raytracer.h"
#include <QPainter>
#include <QSize>
#include <iostream>
#include <vector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    #ifdef QT_OS_WIN32
    QFont font("verdana 9 pt");
    QApplication::setFont(font);
    #endif
    #ifdef QT_OS_OSX
    QFont font("verdana 6.76 pt");
    QApplication::setFont(font);
    #endif
    connect(ui->add_cld, SIGNAL(clicked(bool)), this, SLOT(add_cloud_button()));
    connect(ui->rem_cld, SIGNAL(clicked(bool)), this, SLOT(rem_cloud_button()));
    connect(ui->compute, SIGNAL(clicked(bool)), this, SLOT(compute_button()));

    ui->zenithangle->setRange(0,90);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::add_cloud_button()
{
    ui->atm_frame->add_cloud();
}

void MainWindow::rem_cloud_button()
{
    ui->atm_frame->remove_cloud();
}

void MainWindow::compute_button()
{
    const int H=ui->atm_frame->size().height();
    const int W=ui->atm_frame->size().width();
    const int dhw = 10;
    const int h = H/dhw;
    const int w = W/dhw;

    // setup input arrays for raytracing
    std::vector<float> tau(w*h);
    std::vector<float> ssa(w*h);
    std::vector<int> cld_mask(w*h);
    std::vector<int> size = {h,w};
    float g = ui->g_cloud->value();
    const float albedo = std::min(0.9999f, float(ui->albedo->value()));
    const float sza = std::max(0.0001f, float(ui->zenithangle->value()));
    const float sza_rad = sza/180.f * (4.f*atan(1.f));
    const float ssa_cld  = ui->ssa_cloud->value();
    const float ssa_clr  = ui->ssa_clear->value();
    const float kext_cld = ui->kext_cloud->value();
    const float kext_clr = ui->kext_clear->value();
    float cloud_clear_frac = kext_cld / (kext_cld+kext_clr);
    float k_null = kext_cld+kext_clr;
    int n_photon = int(1e6);
   // if (k_null>20)
   //     ui->kext_te_hoog
    std::cout<<sza<<" - "<<sza_rad<<std::endl;

    QImage domain_img(W, H, QImage::Format_ARGB32);
    QPainter painter(&domain_img);
    ui->atm_frame->render(&painter);
    // read image frame
    //float field[h * w];
    for (int i=0; i<h; ++i)
        for (int j=0; j<w;  ++j)
        {
            const int idx = j+(h-1-i)*w;
            QRgb pixColor = domain_img.pixel(dhw*j,dhw*i);
            if (qRed(pixColor) != qBlue(pixColor))
            {
                tau[idx] = kext_clr; // clear sky pixel
                ssa[idx] = ssa_clr;
                cld_mask[idx] = 0;
            }
            else
            {
                tau[idx] = kext_cld; //cloudy pixel
                ssa[idx] = ssa_cld;
                cld_mask[idx] = 1;
            }
        }


    std::vector<int> sfc_dir(w, 0.f);
    std::vector<int> sfc_dif(w, 0.f);
    trace_ray(tau.data(), ssa.data(), g, cld_mask.data(), size.data(), albedo, sza_rad, cloud_clear_frac, k_null, n_photon, sfc_dir.data(), sfc_dif.data());
    std::vector<float> result(w, 0.f);
    for (int iw=0; iw<w;  ++iw)
        result[iw] = (sfc_dir[iw]+sfc_dif[iw])/float(n_photon);
    //    integrate(field, w, h, result.data());
    float cmin = 666.;
    float cmax = 0;
    for (int i=0; i<w; ++i)
    {
        if (result[i]<cmin)
            cmin = result[i];
        if (result[i]>cmax)
            cmax = result[i];
    }
    if (cmax == cmin)
        cmax = 1;
//    for (int i=0; i<w; ++i)
//        std::cout<<result[i]<<std::endl;
//    std::cout<<cmin<<std::endl;
//    std::cout<<cmax<<std::endl;


    // show result
    QImage result_img(W, 10, QImage::Format_ARGB32);
    int rgb[3];
    for (int iw=0; iw<w; ++iw)
    {
        const float result_norm = (result[iw]-cmin)/(cmax-cmin);
        get_color(result_norm, rgb);
//        std::cout<<rgb[0]<<" "<<rgb[1]<<" "<<rgb[2]<<" "<<result_norm<<std::endl;
        const QColor clr(rgb[0], rgb[1], rgb[2]);
        for (int i=0; i<10; ++i)
            for (int j=0; j<10; ++j)
                result_img.setPixelColor(i+iw*10,j,clr);
       // std::cout<<result[i]<<std::endl;
    }
    //QPixmap result_pxm = QPixmap::fromImage(result_img)
    ui->results->setPixmap(QPixmap::fromImage(result_img));

}
