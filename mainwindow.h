#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSpinBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void add_cloud_button();
    void rem_cloud_button();
    void initialise();
    void toggle_grid(bool);
    void update_grid();
    void compute_button();
    void paint_results();
    void compute_results();
    void compute_paint_results();
    void compute_clims();
    void lmfao(bool);
    void lmfao2();
    void lmfao3();
    void lmfao4();

private:
    Ui::MainWindow *ui;
    const float zenith_0 = 0.f;
    const float albedo_0 = 0.f;
    const float kext_cld_0 = 0.8f;
    const float kext_clr_0 = 0.0003f;
    const float ssa_cld_0  = 0.9f;
    const float ssa_clr_0 = 0.5f;
    const float g_cld_0 = 0.85f;
    const int n_photon_0 = 1000000;

    float photon_count;
    std::vector<float> result;
    std::vector<int> sfc_dir;
    std::vector<int> sfc_dif;
    int w;
    int W;
    int h;
    int H;
    int dhw;
    int dx;
};


#endif // MAINWINDOW_H
