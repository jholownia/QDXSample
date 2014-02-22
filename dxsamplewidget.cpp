/*
================================
 dxsamplewidget.cpp

 Created on: 18 Feb 2014
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2014.
================================
*/

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
    m_frames(0),
    m_animating(false)
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

    // Use this when disabling vsync
    // qDebug() << m_timer->nsecsElapsed();

    // Calculate frames per second
    ++m_frames;
    m_elapsedTime += m_frameTime;

    if (m_elapsedTime >= 1000)
    {        
        emit fps(m_frames);
        m_frames = 0;
        m_elapsedTime = 0;        
    }

    // Rotate the model?
    if (m_animating)
    {
        static float rotation = 0.0f;
        rotation += 0.001f * m_frameTime;
        if (rotation >= 2 * (float)D3DX_PI) rotation -= 2 * (float)D3DX_PI;
        m_model->setRotation(m_model->getRotation().x, rotation, m_model->getRotation().z);
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

void DXSampleWidget::toggleAnimation(bool animating)
{
    m_animating = animating;
}

void DXSampleWidget::rotateX(float angle)
{
    D3DXVECTOR3 rotation = m_model->getRotation();
    m_model->setRotation(angle * 0.0174532925f, rotation.y, rotation.z);
}

void DXSampleWidget::rotateY(float angle)
{
    D3DXVECTOR3 rotation = m_model->getRotation();
    m_model->setRotation(rotation.x, angle * D3DX_PI / 180.0f, rotation.z);
}

void DXSampleWidget::rotateZ(float angle)
{
    D3DXVECTOR3 rotation = m_model->getRotation();
    m_model->setRotation(rotation.x, rotation.y, angle * 0.0174532925f);
}

void DXSampleWidget::setAmbientR(int r)
{
    m_light->setAmbientColor(r / 255.0f, m_light->getAmbientColor().y, m_light->getAmbientColor().z, m_light->getAmbientColor().w);
}

void DXSampleWidget::setAmbientG(int g)
{
    m_light->setAmbientColor(m_light->getAmbientColor().x, g / 255.0f, m_light->getAmbientColor().z, m_light->getAmbientColor().w);
}

void DXSampleWidget::setAmbientB(int b)
{
    m_light->setAmbientColor(m_light->getAmbientColor().x, m_light->getAmbientColor().y, b / 255.0f, m_light->getAmbientColor().w);
}

void DXSampleWidget::setDiffuseR(int r)
{
    m_light->setDiffuseColor(r / 255.0f, m_light->getDiffuseColor().y, m_light->getDiffuseColor().z, m_light->getDiffuseColor().w);
}

void DXSampleWidget::setDiffuseG(int g)
{
    m_light->setDiffuseColor(m_light->getDiffuseColor().x, g / 255.0f, m_light->getDiffuseColor().z, m_light->getDiffuseColor().w);
}

void DXSampleWidget::setDiffuseB(int b)
{
    m_light->setDiffuseColor(m_light->getDiffuseColor().x, m_light->getDiffuseColor().y, b / 255.0f, m_light->getDiffuseColor().w);
}

void DXSampleWidget::setSpecularR(int r)
{
    m_light->setSpecularColor(r / 255.0f, m_light->getSpecularColor().y, m_light->getSpecularColor().z, m_light->getSpecularColor().w);
}

void DXSampleWidget::setSpecularG(int g)
{
    m_light->setSpecularColor(m_light->getSpecularColor().x, g / 255.0f, m_light->getSpecularColor().z, m_light->getSpecularColor().w);
}

void DXSampleWidget::setSpecularB(int b)
{
    m_light->setSpecularColor(m_light->getSpecularColor().x, m_light->getSpecularColor().y, b / 255.0f, m_light->getSpecularColor().w);
}

void DXSampleWidget::setSpecularPower(int power)
{
    m_light->setSpecularPower((float)power);
}

void DXSampleWidget::setSpecularIntensity(int intensity)
{
    m_light->setSpecularIntensity(intensity / 100.0f);
}

void DXSampleWidget::wheelEvent(QWheelEvent *event)
{
    D3DXVECTOR3 camPosition = m_camera->getPosition();
    camPosition.z += event->delta() * 0.0012f;
    m_camera->setPosition(camPosition.x, camPosition.y, camPosition.z);
}

void DXSampleWidget::mousePressEvent(QMouseEvent *event)
{    
    D3DXMATRIX projectionMatrix;
    m_d3d->getProjectionMatrix(projectionMatrix);

    float x = ((2.0f * (float)event->x()) / (float)width() - 1.0f) / projectionMatrix._11;
    float y = ((-2.0f * (float)event->y()) / (float)height() + 1.0f) / projectionMatrix._22;

    D3DXVECTOR3 lightDirection (x, y, 1.0f);

    D3DXMATRIX viewMatrix, inverseViewMatrix;
    m_camera->getViewMatrix(viewMatrix);
    D3DXMatrixInverse(&inverseViewMatrix, NULL, &viewMatrix);

    D3DXVec3TransformNormal(&lightDirection, &lightDirection, &inverseViewMatrix);
    lightDirection.x *= m_camera->getPosition().z;
    lightDirection.y *= m_camera->getPosition().z;

    D3DXVec3Normalize(&lightDirection, &lightDirection);

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

    // Rotate the model
    D3DXMATRIX rotationMatrix;
    D3DXVECTOR3 rotation = m_model->getRotation();
    D3DXMatrixRotationYawPitchRoll(&rotationMatrix, rotation.y, rotation.x, rotation.z);

    D3DXMATRIX worldMatrix;
    m_d3d->getWorldMatrix(worldMatrix);
    D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &rotationMatrix);;

    // Render the model
    m_model->render(m_d3d->getDeviceContext());
    m_shader->render(m_d3d->getDeviceContext(), m_model->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_model->getTextureArray(), m_light->getDirection(), m_light->getAmbientColor(), m_light->getDiffuseColor(), m_camera->getPosition(), m_light->getSpecularColor(), m_light->getSpecularPower(), m_light->getSpecularIntensity());

    // End scene
    m_d3d->endScene();
}
