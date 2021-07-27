#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H
#include "glBasis.h"
#include <QTime>
#include <QQuickItem>
#include <QBasicTimer>

class OpenGLItem : public QQuickItem
{
    Q_OBJECT

public:
    OpenGLItem();
    ~OpenGLItem();

public slots:
    void sync();
    void cleanup();

protected:
    void timerEvent(QTimerEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
private:
    QBasicTimer m_timer;
    glBasis *m_glBasis;
};

#endif // OPENGLWINDOW_H
