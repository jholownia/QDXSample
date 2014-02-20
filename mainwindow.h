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

private:
    Ui::MainWindow *ui;

    void connectSignals();
    void showGPUInfo();
};

#endif // MAINWINDOW_H
