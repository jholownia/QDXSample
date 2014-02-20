#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    bool result = ui->widget->init();
    if (!result)
    {
        qCritical() << "Error initializing QDXWidget!";
    }


    showGPUInfo();
    connectSignals();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateFps(int fps)
{
    ui->lcdFps->display(fps);
}

void MainWindow::on_vsRotationX_valueChanged(int value)
{

}

void MainWindow::connectSignals()
{
    connect(ui->widget, SIGNAL(fps(int)), this, SLOT(updateFps(int)));
}

void MainWindow::showGPUInfo()
{
    std::string name;
    int memory;
    ui->widget->getGPUInfo(name, memory);
    ui->txtGPU->setText(QString::fromStdString(name));
    ui->txtMemory->setText(QString::number(memory).append(" MB"));
}

void MainWindow::on_btnQuit_clicked()
{
    this->close();
}
