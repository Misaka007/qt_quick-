#include "glBasis.h"

void glBasis::initializeGL()
{
    initializeOpenGLFunctions();
//    初始化着色器
    initializeShader();

    //    启用smoothshading(阴影平滑)
    glShadeModel(GL_SMOOTH);
//    设置清除屏幕时所用的颜色
    glClearColor(255.0,255.0,255.0,0.0);
//    设置深度缓存
    glClearDepth(1.0);
//    启用深度测试。
    glEnable(GL_DEPTH_TEST);
//    所作深度测试的类型
    glDepthFunc(GL_LEQUAL);
//    透视修正
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
}

//初始化着色器
void glBasis::initializeShader()
{

}

void glBasis::resizeGL(int w, int h)
{
    Width =w; Height= h;
    glViewport(0, 0, w, h);
}

void glBasis::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
//    选择投影矩阵。
    glMatrixMode(GL_PROJECTION);
//    重置投影矩阵。
    glLoadIdentity();
//    建立透视投影矩阵。
    gluPerspective(45.0,(GLfloat)Width/(GLfloat)Height,0.1,1000.0);
    gluLookAt(camera.eye.x(),camera.eye.y(),camera.eye.z(),
              camera.center.x(),camera.center.y(),camera.center.z(),
              camera.up.x(),camera.up.y(),camera.up.z()
    );

    glBegin(GL_LINES); // 画线 ======
    glColor3f(0.5,0.5,0.5); //gray  灰色线条
    float a =0.5;
    float size =20;
    for(float i = -size; i <= size ; i++)
    {
       // xz 平面 垂直x轴直线  x轴上-10,-9,...,0,1,2,...,10 21条线，z方向范围， -10～10
       glVertex3f(i*a,-0.3,  size*a);
       glVertex3f(i*a, -0.3, -size*a);

       // xz 平面 垂直z轴直线z轴上 -10,-9,...,0,1,2,...,10  21条线，x方向范围， -10～10
       glVertex3f( size*a, -0.3, i*a);
       glVertex3f(-size*a, -0.3, i*a);
    }
    glEnd();
}

void glBasis::mousePressEvent(QMouseEvent *event)
{
    _mousePosition=event->pos();
}

void glBasis::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
        camera.Rotate(_mousePosition-event->pos());
    else if (event->buttons() & Qt::RightButton)
        camera.Translation(_mousePosition-event->pos());
    _mousePosition=event->pos(); //把此时的鼠标坐标作为旧值，为下一次计算增量做准备
}

void glBasis::wheelEvent(QWheelEvent *event)
{
    camera.zoom(event->delta() < 0);
}


QVector3D glBasis::screen2World(QVector3D screen, int *viewport, float *projection, float *modelview) {
    QMatrix4x4 P(projection);
    QMatrix4x4 M(modelview);
    P = P.transposed();
    M = M.transposed();
    double x = screen.x();
    double y = viewport[3] - screen.y();
    double z = screen.z();
    QVector4D in(
            2 * (x - viewport[0]) / viewport[2] - 1,
            2 * (y - viewport[1]) / viewport[3] - 1,
            2 *  z                              - 1,
            1
    );
    QVector4D out = (P * M).inverted() * in;
    if(out[3] == 0)
        throw std::runtime_error("QtOpenGLViewer::screen2World: Failed.");
    x = out[0] / out[3];
    y = out[1] / out[3];
    z = out[2] / out[3];
    return QVector3D(x, y, z);
}

void glBasis::getPickRay(const QPoint &mousePosition, QVector3D &origin, QVector3D &direction) {
//    makeCurrent();

    int viewport[4] = {0, 0, Width ,Height};
    float projection[16];
    float modelview[16];
    glGetFloatv(GL_PROJECTION_MATRIX, projection);
    glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
    origin = screen2World(QVector3D(mousePosition.x(), mousePosition.y(), 0), viewport, projection, modelview);
    direction = screen2World(QVector3D(mousePosition.x(), mousePosition.y(), 1), viewport, projection, modelview);
}

QVector3D glBasis::getIntersectionines(const QVector3D &line1P1, const QVector3D &line1P2,
                                          const QVector3D &line2P1, const QVector3D &line2P2) {
    QVector3D v1 = line1P2 - line1P1;
    QVector3D v2 = line2P2 - line2P1;
    QVector3D startPointSeg = line2P1 - line1P1;
    QVector3D vecS1 = QVector3D::crossProduct(v1, v2);            // 有向面积1
    QVector3D vecS2 = QVector3D::crossProduct(startPointSeg, v2); // 有向面积2
    float num = QVector3D::dotProduct(vecS2, vecS1) / vecS1.lengthSquared();
    return line1P1 + v1 * num;
}
//获取鼠标y=0平面交点
QVector3D glBasis::getMousePlaneIntersection(const QPoint &mousePosition){
    QVector3D pickOrigin, pickRay;
    getPickRay(mousePosition, pickOrigin, pickRay);
    return getIntersectionines(pickOrigin ,pickRay,
                               QVector3D(pickOrigin.x(),0,pickOrigin.z()),QVector3D(pickRay.x(),0,pickRay.z()));
}