#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void updateFps(int fps);

private slots:
    void on_vsRotationX_valueChanged(int value);

    void on_btnQuit_clicked();

    void on_btnAnimation_toggled(bool checked);

    void on_vsRotationY_valueChanged(int value);

    void on_vsRotationZ_valueChanged(int value);

    void on_vsAr_valueChanged(int value);

    void on_vsAg_valueChanged(int value);

    void on_vsAb_valueChanged(int value);

    void on_vsDr_valueChanged(int value);

    void on_vsDg_valueChanged(int value);

    void on_vsDb_valueChanged(int value);

    void on_vsSr_valueChanged(int value);

    void on_vsSg_valueChanged(int value);

    void on_vsSb_valueChanged(int value);

    void on_vsSpower_valueChanged(int value);

    void on_vsSintensity_valueChanged(int value);

private:
    Ui::MainWindow *ui;

    void connectSignals();
    void showGPUInfo();
    void updateAmbientLabel();
    void updateDiffuseLabel();
    void updateSpecularLabel();
};

#endif // MAINWINDOW_H
