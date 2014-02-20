#include "dxsamplewidget.h"
#include "camera.h"
#include "model.h"
#include "shader.h"
#include "light.h"

#include <QDebug>
#include <QWheelEvent>
#include <QElapsedTimer>

DXSampleWidget::DXSampleWidget(QWidget *parent) :
    QDXWidget(parent),
    m_camera(nullptr),
    m_model(nullptr),
    m_shader(nullptr),
    m_light(nullptr),
    m_timer(nullptr),
    m_frameTime(0),
    m_elapsedTime(0),
    m_frames(0)
{

}

DXSampleWidget::~DXSampleWidget()
{

}

bool DXSampleWidget::init()
{
    // Initialize DirectX
    m_d3d.reset(new D3DBase);
    bool result = m_d3d->init(width(), height(), VSYNC_ENABLED, (HWND)winId(), FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);

    if (!result)
    {
        qCritical() << "Error initializing Direct3D!";
    }

    // Create a camera
    m_camera.reset(new Camera);
    m_camera->setPosition(0.0f, 0.0f, -5.0f);

    // Create and initialize a model
    m_model.reset(new Model);
    result = m_model->init(m_d3d->getDevice(), "Data/ball.obj", "Data/earth_4k.jpg", "Data/earth_normal.png", "Data/earth_specular.jpg");

    if (!result)
    {
        qCritical() << "Error initializing model!";
    }

    // Create and initalize shader class
    m_shader.reset(new Shader);
    m_shader->init(m_d3d->getDevice(), (HWND)winId());

    if (!result)
    {
        qCritical() << "Error initializing shader!";
    }

    // Create and setup light
    m_light.reset(new Light);
    m_light->setAmbientColor(0.1f, 0.1f, 0.1f, 1.0f);
    m_light->setDiffuseColor(1.0f, 1.0f, 0.8f, 1.0f);
    // light_->setDirection(-1.0f, 0.7f, 1.5f);

    m_light->setDirection(0.0f, 0.0f, 1.0f);
    // light_->setDirection(-0.8f, -0.5f, 1.0f);
    m_light->setLookAt(0.0f, 0.0f, 0.0f);
    m_light->generateProjectionMatrix(SCREEN_DEPTH, SCREEN_NEAR);

    m_light->setSpecularColor(1.0f, 1.0f, 0.7f, 1.0f);
    m_light->setSpecularPower(26.0f);
    m_light->setSpecularIntensity(0.6f);

    // Timer
    m_timer.reset(new QElapsedTimer);
    m_timer->start();

    return true;
}

void DXSampleWidget::update()
{
    // Get time since last frame
    m_frameTime = m_timer->restart();

    // Calculate frames per second
    ++m_frames;
    m_elapsedTime += m_frameTime;
    if (m_elapsedTime >= 1000)
    {
        emit fps(m_frames);
        m_frames = 0;
        m_elapsedTime = 0;
    }

    // Render the scene
    render();
}

void DXSampleWidget::getGPUInfo(std::string &gpuName, int &memory) const
{
    if (m_d3d)
    {
        m_d3d->getGpuInfo(gpuName, memory);
    }
}

QSize DXSampleWidget::minimumSizeHint() const
{
    return QSize(300, 300);
}

QSize DXSampleWidget::sizeHint() const
{
    return QSize(800, 600);
}

void DXSampleWidget::wheelEvent(QWheelEvent *event)
{
    D3DXVECTOR3 camPosition = m_camera->getPosition();
    camPosition.z += event->delta() * 0.0012f;
    m_camera->setPosition(camPosition.x, camPosition.y, camPosition.z);
}

void DXSampleWidget::mousePressEvent(QMouseEvent *event)
{
    D3DXVECTOR3 lightDirection;
    lightDirection = m_light->getDirection();
    lightDirection.x = (2.0f * (float)event->x()) / (float)width() - 1.0f;
    lightDirection.y = ((2.0f * (float)event->y()) / (float)height() - 1.0f) * -1.0f;

    float x = lightDirection.x;
    float y = lightDirection.y;

    D3DXMATRIX projectionMatrix;
    m_d3d->getProjectionMatrix(projectionMatrix);
    x = x / projectionMatrix._11;
    y = y / projectionMatrix._22;

    D3DXMATRIX viewMatrix, inverseViewMatrix;
    m_camera->getViewMatrix(viewMatrix);
    D3DXMatrixInverse(&inverseViewMatrix, NULL, &viewMatrix);

    lightDirection.x = (x * inverseViewMatrix._11) + (y * inverseViewMatrix._21) + inverseViewMatrix._31;
    lightDirection.y = (x * inverseViewMatrix._12) + (y * inverseViewMatrix._22) + inverseViewMatrix._32;
    lightDirection.z = (x * inverseViewMatrix._13) + (y * inverseViewMatrix._23) + inverseViewMatrix._33;

    D3DXMATRIX worldMatrix, inverseWorldMatrix;
    m_d3d->getWorldMatrix(worldMatrix);
    D3DXMatrixInverse(&inverseWorldMatrix, NULL, &worldMatrix);
    D3DXVec3TransformCoord(&lightDirection, &lightDirection, &inverseWorldMatrix);

    qDebug() << lightDirection.x << ", " << lightDirection.y;
    m_light->setDirection(lightDirection.x, lightDirection.y, lightDirection.z);
}

void DXSampleWidget::render()
{
    // Begin scene
    m_d3d->beginScene(0.0f, 0.0f, 0.0f, 1.0f);

    // Render the camera
    m_camera->render();

    // Get matrices
    D3DXMATRIX viewMatrix, projectionMatrix, orthoMatrix;

    m_camera->getViewMatrix(viewMatrix);
    m_d3d->getProjectionMatrix(projectionMatrix);
    m_d3d->getOrthoMatrix(orthoMatrix);

    // Move the model?
    static float rotation = 0.0f;

    D3DXMATRIX rotationMatrix;
    D3DXMatrixRotationY(&rotationMatrix, -rotation);
    rotation += 0.001f * m_frameTime;
    if (rotation >= 360.0f) rotation -= 360.0f;

    //D3DXMatrixRotationY(&rotationMatrix, model_->getRotation().y);
    //D3DXMATRIX translationMatrix;
    //D3DXMatrixTranslation(&translationMatrix, tiger_->getPosition().x, tiger_->getPosition().y, tiger_->getPosition().z);

    // Render the model

    D3DXMATRIX worldMatrix;
    m_d3d->getWorldMatrix(worldMatrix);

    // Scale?
    /*D3DXMATRIX scaleMatrix;
    D3DXMatrixScaling(&scaleMatrix, 0.2f, 0.2f, 0.2f);
    D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &scaleMatrix);*/

    D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &rotationMatrix);

    m_model->render(m_d3d->getDeviceContext());
    m_shader->render(m_d3d->getDeviceContext(), m_model->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_model->getTextureArray(), m_light->getDirection(), m_light->getAmbientColor(), m_light->getDiffuseColor(), m_camera->getPosition(), m_light->getSpecularColor(), m_light->getSpecularPower(), m_light->getSpecularIntensity());

    // End scene
    m_d3d->endScene();
}