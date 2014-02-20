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

    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);

    void render();
};

#endif // DXSAMPLEWIDGET_H
