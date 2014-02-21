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
    updateAmbientLabel();
    updateDiffuseLabel();
    updateSpecularLabel();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateFps(int fps)
{
    ui->lcdFps->display(fps);
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

void MainWindow::updateAmbientLabel()
{
    QString rgbString = QString::number(ui->vsAr->value()) + ", " + QString::number(ui->vsAg->value()) + ", " + QString::number(ui->vsAb->value());
    ui->lblAmbient->setStyleSheet("QLabel { background-color: rgba(" + rgbString + ");}");
}

void MainWindow::updateDiffuseLabel()
{
    QString rgbString = QString::number(ui->vsDr->value()) + ", " + QString::number(ui->vsDg->value()) + ", " + QString::number(ui->vsDb->value());
    ui->lblDiffuse->setStyleSheet("QLabel { background-color: rgba(" + rgbString + ");}");
}

void MainWindow::updateSpecularLabel()
{
    QString rgbString = QString::number(ui->vsSr->value()) + ", " + QString::number(ui->vsSg->value()) + ", " + QString::number(ui->vsSb->value());
    ui->lblSpecular->setStyleSheet("QLabel { background-color: rgba(" + rgbString + ");}");
}

void MainWindow::on_btnQuit_clicked()
{
    this->close();
}

void MainWindow::on_btnAnimation_toggled(bool checked)
{
    ui->widget->toggleAnimation(checked);
}

void MainWindow::on_vsRotationX_valueChanged(int value)
{
    ui->widget->rotateX(value);
}

void MainWindow::on_vsRotationY_valueChanged(int value)
{
    ui->widget->rotateY(value);
}

void MainWindow::on_vsRotationZ_valueChanged(int value)
{
    ui->widget->rotateZ(value);
}

void MainWindow::on_vsAr_valueChanged(int value)
{
    ui->widget->setAmbientR(value);
    updateAmbientLabel();
}

void MainWindow::on_vsAg_valueChanged(int value)
{
    ui->widget->setAmbientG(value);
    updateAmbientLabel();
}

void MainWindow::on_vsAb_valueChanged(int value)
{
    ui->widget->setAmbientB(value);
    updateAmbientLabel();
}

void MainWindow::on_vsDr_valueChanged(int value)
{
    ui->widget->setDiffuseR(value);
    updateDiffuseLabel();
}

void MainWindow::on_vsDg_valueChanged(int value)
{
    ui->widget->setDiffuseG(value);
    updateDiffuseLabel();
}

void MainWindow::on_vsDb_valueChanged(int value)
{
    ui->widget->setDiffuseB(value);
    updateDiffuseLabel();
}

void MainWindow::on_vsSr_valueChanged(int value)
{
    ui->widget->setSpecularR(value);
    updateSpecularLabel();
}

void MainWindow::on_vsSg_valueChanged(int value)
{
    ui->widget->setSpecularG(value);
    updateSpecularLabel();
}

void MainWindow::on_vsSb_valueChanged(int value)
{
    ui->widget->setSpecularB(value);
    updateSpecularLabel();
}

void MainWindow::on_vsSpower_valueChanged(int value)
{
    ui->widget->setSpecularPower(value);
}

void MainWindow::on_vsSintensity_valueChanged(int value)
{
    ui->widget->setSpecularIntensity(value);
}
