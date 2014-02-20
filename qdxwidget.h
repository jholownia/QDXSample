#ifndef QDXWIDGET_H
#define QDXWIDGET_H

#include <QWidget>
#include "d3dbase.h"
#include "config.h"

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
