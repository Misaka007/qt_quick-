#ifndef RENDER_H
#define RENDER_H
#include <QOpenGLFunctions>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLFunctions>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QTimer>
#include <QPainter>
#include <QVector3D>
#include <QDebug>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/gl.h>
#include <GL/glu.h>
#include <QtGui/QMatrix4x4>
#include <math.h>
class glBasis : protected QOpenGLFunctions
{
public:
    struct  Camera{
        QVector3D eye = QVector3D(0, 20, 10);
        QVector3D center = QVector3D(0, 0, 0);
        QVector3D up = QVector3D(0, 1, 0);
        QVector3D view() { return eye - center; }
        void Rotate(QPoint mousePosition) {
            QVector3D L = view();
            double L_module = pow((pow(L.x(),2)+pow(L.z(),2)),0.5);
            QVector3D L_vertical_Y_1(-L.z()/L_module,0,L.x()/L_module);
            QMatrix4x4 mvp;
            mvp.rotate(mousePosition.y()*0.4,L_vertical_Y_1);
            L = mvp.inverted()*L;
            L_module = pow((pow(L.x(),2)+pow(L.z(),2)),0.5);
            if (fabs(L.y())/L_module<pow(3,0.5))
                eye =L+center;
            QMatrix4x4 mvp1;
            mvp1.rotate(-mousePosition.x()*0.4, QVector3D(0, 1, 0));
            eye = mvp1.inverted()*view()+center;
        }

        void Translation(QPoint mousePosition,float times=0.05) {
            QVector3D L =view().normalized();
            double L_module = pow((pow(L.x(),2)+pow(L.z(),2)),0.5);
            QVector3D L_vertical(L.z()/L_module*mousePosition.x()*times-L.x()/L_module*mousePosition.y()*times,0,
                                 -L.x()/L_module*mousePosition.x()*times+L.z()/L_module*mousePosition.y()*times);
            eye+=L_vertical; center+=L_vertical;
        }

        void Translation(float x,float y,float z){}

        void zoom(bool is) {
            if (is)
                eye+=view().normalized()*1.2;
            else
                eye-=view().normalized() *1.2;
        }
    }camera;
public:
    glBasis() { }
    virtual ~glBasis() { }



public:
    virtual void initializeGL();
    virtual void initializeShader();

    virtual void resizeGL(int w, int h);
    virtual void paintGL();

    void mousePressEvent(QMouseEvent *event) ;
    void mouseMoveEvent(QMouseEvent *event) ;
    void wheelEvent(QWheelEvent *event) ;

public:
    static QVector3D screen2World(QVector3D screen, int *viewport, float *projection, float *modelview);
    void getPickRay(const QPoint &mousePosition, QVector3D &origin, QVector3D &direction);
    QVector3D getIntersectionines(const QVector3D &rayOrigin, const QVector3D &rayDirection, const QVector3D &pointOnPlane, const QVector3D &planeNormal);
    QVector3D getMousePlaneIntersection(const QPoint &mousePosition);

protected:
    int Width ,Height;
    QPoint _mousePosition;
};

#endif // RENDER_H
