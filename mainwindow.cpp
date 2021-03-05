#include <chrono>
#include <Windows.h>
#include <cstdlib>
#include <unistd.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "helper_functions.h"
#include "raytracer.h"
#include <QPainter>
#include <QFontDatabase>
#include <QSize>
#include <iostream>
#include <vector>
#include <cmath>
#include <QMessageBox>
#include <thread>
#include <compute.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QFont font(QFontDatabase::systemFont(QFontDatabase::SmallestReadableFont));
    QApplication::setFont(font);

    ui->zenithangle->setRange(0,89.9);
    Compute* thr = new Compute();

    connect(ui->add_cld, SIGNAL(clicked(bool)), this, SLOT(add_cloud_button()));
    connect(ui->rem_cld, SIGNAL(clicked(bool)), this, SLOT(rem_cloud_button()));
    connect(ui->compute, SIGNAL(clicked(bool)), this, SLOT(compute_button()));
    connect(ui->reset_btn, SIGNAL(clicked(bool)), this, SLOT(initialise()));
    connect(ui->showgrid, SIGNAL(clicked(bool)), this, SLOT(toggle_grid(bool)));
    connect(ui->zenithangle, SIGNAL(valueChanged(double)), this, SLOT(update_grid()));
    connect(ui->cmin, SIGNAL(valueChanged(double)), this, SLOT(paint_results()));
    connect(ui->cmax, SIGNAL(valueChanged(double)), this, SLOT(paint_results()));
    connect(ui->dirdiftot, SIGNAL(currentTextChanged(QString)), this, SLOT(compute_paint_results()));
    connect(ui->fullrange, SIGNAL(clicked(bool)), this, SLOT(compute_clims()));
    connect(ui->ln_global, SIGNAL(clicked(bool)), this, SLOT(paint_lines()));
    connect(ui->ln_direct, SIGNAL(clicked(bool)), this, SLOT(paint_lines()));
    connect(ui->ln_diffuse, SIGNAL(clicked(bool)), this, SLOT(paint_lines()));
    //connect(ui->streams, SIGNAL(clicked(bool)), this, SLOT(streams(bool)));
    //connect(ui->streams, SIGNAL(underMouse()), this, SLOT(streams()));

//    connect(ui->thread, SIGNAL(clicked(bool)), thr, SLOT(start()));
//    connect(thr, SIGNAL(lets_paint()), this, SLOT(streams2()));


    ui->gridlines->angle = ui->zenithangle->value();
    ui->gridlines->show();
    ui->lineplots->show();

    ui->dirdiftot->addItem(tr("Global"));
    ui->dirdiftot->addItem(tr("Direct"));
    ui->dirdiftot->addItem(tr("Diffuse"));
    ui->dirdiftot->setCurrentIndex(0);

    ui->ln_global->linecolor  = Qt::black;
    ui->ln_direct->linecolor  = QColor(69,106,216);
    ui->ln_diffuse->linecolor = QColor(242,212,62);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::add_cloud_button()
{
    ui->atm_frame->add_cloud();
}
void MainWindow::move_button()
{
    std::cout<<ui->streams->pos().x()<<std::endl;
    std::cout<<ui->streams->pos().y()<<std::endl;
}

void MainWindow::streams(bool checked)
{
    if (checked)
    {
        QMessageBox *test = new QMessageBox(this);
        test->setText("Are you sure?");
        test->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        test->setStyleSheet("QLabel{min-width: 100px;}");
        int ret = test->exec();
        switch (ret)
        {
            case QMessageBox::Yes:
            streams2();
            break;
        }
    }
}
void MainWindow::streams2()
{
    std::cout<<"---"<<std::endl;
    ui->albedo->setValue(0.5);
    //    QMessageBox *test = new QMessageBox(this);
//    test->setText("Do you really want to use an n-stream solver?");
//    test->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
//    test->setStyleSheet("QLabel{min-width: 300px;}");
//    int ret = test->exec();
//    switch (ret)
//    {
//        case QMessageBox::Yes:
//        streams3();
//        break;
//    }
}
void MainWindow::streams3()
{
    QMessageBox *test = new QMessageBox(this);
    test->setText("It's less accurate than ray tracing!");
    test->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    test->button(QMessageBox::Yes)->setText("Good point");
    test->button(QMessageBox::No)->setText("I don't care");
    test->setStyleSheet("QLabel{min-width: 200px;}");
    int ret = test->exec();
    switch (ret)
    {
        case QMessageBox::No:
        streams4();
        break;
    }
}
void MainWindow::streams4()
{
    QMessageBox *test = new QMessageBox(this);
    test->setText("Are you a bot perhaps?");
    test->setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::NoToAll);
    test->button(QMessageBox::NoToAll)->setText("I'm a dolphin!");
    test->setStyleSheet("QLabel{min-width: 120px;}");
    int ret = test->exec();
    switch (ret)
    {
        case QMessageBox::NoToAll:
        streams(true);
        break;
    }
}void MainWindow::rem_cloud_button()
{
    ui->atm_frame->remove_cloud();
}

void MainWindow::initialise()
{
    ui->zenithangle->setValue(zenith_0);
    ui->albedo->setValue(albedo_0);
    ui->kext_cloud->setValue(kext_cld_0);
    ui->kext_clear->setValue(kext_clr_0);
    ui->ssa_cloud->setValue(ssa_cld_0);
    ui->ssa_clear->setValue(ssa_clr_0);
    ui->g_cloud->setValue(g_cld_0);
    ui->nphoton->setValue(n_photon_0);
    ui->atm_frame->remove_all_clouds();
}

void MainWindow::update_grid()
{
    if (ui->showgrid->isChecked())
    {
        ui->gridlines->angle = ui->zenithangle->value();
        ui->gridlines->update();
    }
}
void MainWindow::toggle_grid(bool checked)
{
    if (checked)
    {
        ui->gridlines->angle = ui->zenithangle->value();
        ui->gridlines->show();
    }
    else
        ui->gridlines->hide();
}

void MainWindow::paint_lines()
{
    if (not this->res_global.empty())
    {
        ui->lineplots->do_global  = ui->ln_global->isChecked();
        ui->lineplots->do_direct  = ui->ln_direct->isChecked();
        ui->lineplots->do_diffuse = ui->ln_diffuse->isChecked();

        ui->lineplots->update();
    }
}

void MainWindow::compute_paint_results()
{
    std::string ddt = ui->dirdiftot->currentText().toStdString();
    if (ddt == "Global")
        result_cur = res_global.data();
    else if (ddt == "Direct")
        result_cur = res_direct.data();
    else if (ddt == "Diffuse")
        result_cur = res_diffuse.data();

    paint_results();
}

void MainWindow::compute_results()
{
    if (not this->res_global.empty())
    {
        const int w_out = W/dx;
        for (int iw=0; iw<w_out;  ++iw)
        {
            res_global[iw] = (sfc_dir[iw]+sfc_dif[iw])/photon_count*w_out;
            res_direct[iw] = (sfc_dir[iw])/photon_count*w_out;
            res_diffuse[iw] = (sfc_dif[iw])/photon_count*w_out;
        }
    }
}

void MainWindow::paint_results()
{
    if (not this->res_global.empty())
    {
        const int w_out = W/dx;
        // show result
        QImage result_img(W, 10, QImage::Format_ARGB32);
        int rgb[3];
        float cmin = ui->cmin->value();
        float cmax = ui->cmax->value();
        for (int iw=0; iw<w_out; ++iw)
        {
            float result_norm = (result_cur[iw]-cmin)/(cmax-cmin);
            result_norm = std::max(0.f,std::min(1.f,result_norm));
            get_color(result_norm, rgb);
            const QColor clr(rgb[0], rgb[1], rgb[2]);
            for (int i=0; i<dx; ++i)
                for (int j=0; j<10; ++j)
                    result_img.setPixelColor(i+iw*dx,j,clr);
        }
        ui->results->setPixmap(QPixmap::fromImage(result_img));
    }
}

void MainWindow::compute_clims()
{
    const int w_out = W/dx;
    float cmin = 666.;
    float cmax = 0;
    for (int i=0; i<w_out; ++i)
    {
        if (result_cur[i]<cmin)
            cmin = result_cur[i];
        if (result_cur[i]>cmax)
            cmax = result_cur[i];
    }
    if (cmax == cmin)
        cmax = 1;
    ui->cmin->setValue(cmin);
    ui->cmax->setValue(cmax);
}


void MainWindow::compute_button()
{
    std::cout<<"XXXXXXX"<<std::endl;
    H=ui->atm_frame->size().height();
    W=ui->atm_frame->size().width();
    dhw = ui->inputres->value();
    dx  = ui->outputres->value();
    h = H/dhw;
    w = W/dhw;

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
    int n_photon = int(ui->nphoton->value()*100000);
    photon_count = float(n_photon);
    ui->gridlines->angle = ui->zenithangle->value();
    ui->gridlines->update();
    ui->dirdiftot->setCurrentIndex(0);

    QImage domain_img(W, H, QImage::Format_ARGB32);
    QPainter painter(&domain_img);
    ui->atm_frame->render(&painter);

    const float tau_scale = dhw/10.f;
    for (int i=0; i<h; ++i)
        for (int j=0; j<w;  ++j)
        {
            const int idx = j+(h-1-i)*w;
            QRgb pixColor = domain_img.pixel(dhw*j,dhw*i);
            if (qRed(pixColor) != qBlue(pixColor))
            {
                tau[idx] = kext_clr * tau_scale; // clear sky pixel
                ssa[idx] = ssa_clr;
                cld_mask[idx] = 0;
            }
            else
            {
                tau[idx] = kext_cld * tau_scale; //cloudy pixel
                ssa[idx] = ssa_cld;
                cld_mask[idx] = 1;
            }
        }

    const int w_out = int(W/dx);
    sfc_dir.resize(w_out);
    sfc_dif.resize(w_out);
    std::fill(sfc_dir.begin(), sfc_dir.end(), 0);
    std::fill(sfc_dif.begin(), sfc_dif.end(), 0);
    std::cout<<"XXXXXXX"<<std::endl;
//trace_ray(tau.data(), ssa.data(), g, cld_mask.data(), size.data(), albedo, sza_rad, cloud_clear_frac, k_null, n_photon, sfc_dir, sfc_dif);
//    std::thread run_raytracing(trace_ray,tau.data(), ssa.data(), g, cld_mask.data(), size.data(), albedo, sza_rad, cloud_clear_frac, k_null, n_photon, sfc_dir.data(), sfc_dif.data(), sfc_dif.size());
    trace_ray(tau.data(), ssa.data(), g, cld_mask.data(), size.data(), albedo, sza_rad, cloud_clear_frac, k_null, n_photon, sfc_dir.data(), sfc_dif.data(), sfc_dif.size());
//    run_raytracing.join();
    res_global.resize(w_out);
    res_diffuse.resize(w_out);
    res_direct.resize(w_out);
    result_cur = res_global.data();

    ui->lineplots->global = res_global.data();
    ui->lineplots->direct = res_direct.data();
    ui->lineplots->diffuse = res_diffuse.data();
    ui->lineplots->nx = res_global.size();
    ui->lineplots->dx = dx;
    ui->ln_global->setChecked(1);
    ui->ln_direct->setChecked(1);
    ui->ln_diffuse->setChecked(1);

    compute_results();
    compute_clims();

    paint_results();
    paint_lines();
}
