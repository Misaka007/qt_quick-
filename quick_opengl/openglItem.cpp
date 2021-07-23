#include "openglItem.h"
#include <QQuickWindow>
#include <iostream>

OpenGLItem::OpenGLItem()
    : m_glBasis(0)
{
    m_timer.start(12, this);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::AllButtons);
    setFlag(ItemAcceptsInputMethod, true);

    connect(this, &QQuickItem::windowChanged, this, [this](QQuickWindow *window)
    {
        if (window)
        {
            connect(window, &QQuickWindow::beforeSynchronizing, this, &OpenGLItem::sync,
                    Qt::DirectConnection);
            connect(window, &QQuickWindow::sceneGraphInvalidated, this, &OpenGLItem::cleanup,
                    Qt::DirectConnection);

            window->setClearBeforeRendering(false);
        }
        else return;
    });
}

OpenGLItem::~OpenGLItem()
{

}

void OpenGLItem::sync()
{
    if (!m_glBasis)
    {
        m_glBasis = new glBasis();
        m_glBasis->initializeGL();
        m_glBasis->resizeGL(window()->width(), window()->height());
        connect(window(), &QQuickWindow::beforeRendering, this, [this]()
        {
            //window()->resetOpenGLState();
            m_glBasis->paintGL();
        }, Qt::DirectConnection);
        connect(window(), &QQuickWindow::afterRendering, this, [this]()
        {
            //渲染后调用，计算fps
        }, Qt::DirectConnection);
        connect(window(), &QQuickWindow::widthChanged, this, [this]()
        {
            m_glBasis->resizeGL(window()->width(), window()->height());
        });
        connect(window(), &QQuickWindow::heightChanged, this, [this]()
        {
            m_glBasis->resizeGL(window()->width(), window()->height());
        });
    }
}

void OpenGLItem::cleanup()
{
    if (m_glBasis)
    {
        delete m_glBasis;
        m_glBasis = nullptr;
    }
}

void OpenGLItem::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    window()->update();
}

void OpenGLItem::mouseReleaseEvent(QMouseEvent *event) {
    QQuickItem::mouseReleaseEvent(event);
    event->accept();
}

void OpenGLItem::mousePressEvent(QMouseEvent *event) {
    m_glBasis->mousePressEvent(event);
    QQuickItem::mousePressEvent(event);
    event->accept();
}

void OpenGLItem::mouseMoveEvent(QMouseEvent *event) {
    m_glBasis->mouseMoveEvent(event);
    QQuickItem::mouseMoveEvent(event);
}

void OpenGLItem::wheelEvent(QWheelEvent *event) {
    m_glBasis->wheelEvent(event);
    QQuickItem::wheelEvent(event);
}
