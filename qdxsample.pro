#-------------------------------------------------
#
# Project created by QtCreator 2014-02-19T16:34:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qdxsample
TEMPLATE = app
DESTDIR = $$PWD

SOURCES += main.cpp\
        mainwindow.cpp \
    camera.cpp \
    light.cpp \
    model.cpp \
    moveable.cpp \
    objloader.cpp \
    shader.cpp \
    texturearray.cpp \
    d3dbase.cpp \
    qdxwidget.cpp \
    dxsamplewidget.cpp

HEADERS  += mainwindow.h \
    camera.h \
    config.h \
    light.h \
    mesh.h \
    model.h \
    moveable.h \
    objloader.h \    
    shader.h \
    texturearray.h \
    d3dbase.h \
    qdxwidget.h \
    dxsamplewidget.h


FORMS    += mainwindow.ui

OTHER_FILES += \
    cbuffers.hlsli \
    pixelshader.hlsl \
    vertexshader.hlsl

win32:CONFIG(release, debug|release): LIBS += -L"C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86/" -ld3dx11
else:win32:CONFIG(debug, debug|release): LIBS += -L"C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86/" -ld3dx11d

INCLUDEPATH += "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include"
DEPENDPATH += "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include"
