#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "integrate.h"
#include "helper_functions.h"
#include "raytracer.h"
#include <QPainter>
#include <QFontDatabase>
#include <QSize>
#include <iostream>
#include <vector>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QFont font(QFontDatabase::systemFont(QFontDatabase::SmallestReadableFont));
    QApplication::setFont(font);

    ui->zenithangle->setRange(0,89.9);

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

    ui->gridlines->angle = ui->zenithangle->value();
    ui->gridlines->show();

    ui->dirdiftot->addItem(tr("Global"));
    ui->dirdiftot->addItem(tr("Direct"));
    ui->dirdiftot->addItem(tr("Diffuse"));
    ui->dirdiftot->setCurrentIndex(0);
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
void MainWindow::compute_paint_results()
{
    compute_results();
    paint_results();
}

void MainWindow::compute_results()
{
    if (not this->result.empty())
    {
        std::string ddt = ui->dirdiftot->currentText().toStdString();
        const int w_out = W/dx;
        if (ddt == "Global")
            for (int iw=0; iw<w_out;  ++iw)
                result[iw] = (sfc_dir[iw]+sfc_dif[iw])/photon_count*w_out;
        else if (ddt == "Direct")
            for (int iw=0; iw<w_out;  ++iw)
                result[iw] = (sfc_dir[iw])/photon_count*w_out;
        else if (ddt == "Diffuse")
            for (int iw=0; iw<w_out;  ++iw)
                result[iw] = (sfc_dif[iw])/photon_count*w_out;
    }
}

void MainWindow::paint_results()
{
    if (not this->result.empty())
    {
        const int w_out = W/dx;
        // show result
        QImage result_img(W, 10, QImage::Format_ARGB32);
        int rgb[3];
        float cmin = ui->cmin->value();
        float cmax = ui->cmax->value();
        for (int iw=0; iw<w_out; ++iw)
        {
            float result_norm = (result[iw]-cmin)/(cmax-cmin);
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
        if (result[i]<cmin)
            cmin = result[i];
        if (result[i]>cmax)
            cmax = result[i];
    }
    if (cmax == cmin)
        cmax = 1;
    ui->cmin->setValue(cmin);
    ui->cmax->setValue(cmax);
}

void MainWindow::compute_button()
{
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
    int n_photon = int(ui->nphoton->value());
    photon_count = float(n_photon);
    ui->gridlines->angle = ui->zenithangle->value();
    ui->gridlines->update();
    ui->dirdiftot->setCurrentIndex(0);
    QImage domain_img(W, H, QImage::Format_ARGB32);
    QPainter painter(&domain_img);
    ui->atm_frame->render(&painter);

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

    const int w_out = int(W/dx);
    sfc_dir.resize(w_out);
    sfc_dif.resize(w_out);
    std::fill(sfc_dir.begin(), sfc_dir.end(), 0);
    std::fill(sfc_dif.begin(), sfc_dif.end(), 0);

    trace_ray(tau.data(), ssa.data(), g, cld_mask.data(), size.data(), albedo, sza_rad, cloud_clear_frac, k_null, n_photon, sfc_dir, sfc_dif);
    result.resize(w_out);
    compute_results();
    compute_clims();

    paint_results();
}
