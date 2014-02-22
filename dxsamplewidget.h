/*
================================
 dxsamplewidget.h

 Created on: 18 Feb 2014
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2014.
================================
*/

#ifndef DXSAMPLEWIDGET_H
#define DXSAMPLEWIDGET_H

#include "qdxwidget.h"

class Camera;
class Model;
class Shader;
class Light;
class QElapsedTimer;

class DXSampleWidget : public QDXWidget
{
    Q_OBJECT
public:
    explicit DXSampleWidget(QWidget *parent = 0);
    ~DXSampleWidget();

    virtual bool init();
    virtual void update();

    void getGPUInfo(std::string& gpuName, int& memory) const;

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    // GUI handlers
    void toggleAnimation(bool animating);
    void rotateX(float angle);
    void rotateY(float angle);
    void rotateZ(float angle);
    void setAmbientR(int r);
    void setAmbientG(int g);
    void setAmbientB(int b);
    void setDiffuseR(int r);
    void setDiffuseG(int g);
    void setDiffuseB(int b);
    void setSpecularR(int r);
    void setSpecularG(int g);
    void setSpecularB(int b);
    void setSpecularPower(int power);
    void setSpecularIntensity(int intensity);

signals:
    void fps(int fps);

public slots:

private:
    QScopedPointer<Camera> m_camera;
    QScopedPointer<Model> m_model;
    QScopedPointer<Shader> m_shader;
    QScopedPointer<Light> m_light;
    QScopedPointer<QElapsedTimer> m_timer;
    qint64 m_frameTime;
    qint64 m_elapsedTime;
    int m_frames;
    bool m_animating;

    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);

    void render();
};

#endif // DXSAMPLEWIDGET_H
