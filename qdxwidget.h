/*
================================
 dxwidget.h

 Created on: 16 Feb 2014
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#ifndef QDXWIDGET_H
#define QDXWIDGET_H

#include "d3dbase.h"
#include "config.h"

#include <QWidget>

class QDXWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QDXWidget(QWidget *parent = 0);
    virtual ~QDXWidget();

    virtual bool init();
    virtual void update();    

public slots:
    void updateNow();
    void updateLater();

protected:
    QScopedPointer<D3DBase> m_d3d;
    bool m_updatePending;

    bool event(QEvent *event);
    void resizeEvent(QResizeEvent *event);

    QPaintEngine *paintEngine() const { return 0; }
    virtual void paintEvent(QPaintEvent *e) { Q_UNUSED(e); }
};

#endif // QDXWIDGET_H
