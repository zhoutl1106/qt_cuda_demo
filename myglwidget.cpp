#include "myglwidget.h"
#include <QWheelEvent>
#include <QDebug>
#include <QPainter>
#include "dc10.h"


MyGLDrawer::MyGLDrawer(QWidget *parent) :
    QGLWidget(parent)
{
    setMouseTracking(true);
    /*setAutoFillBackground(false);
    setAutoBufferSwap(false);*/
    camPos[0]=0.0f;
    camPos[1]=0.0f;
    camPos[2]=-4000.0f;
    mouseLeftButtonFlag = false;
    mouseRightButtonFlag = false;
    button = new QPushButton(this);
    button->move(0,0);
    button->show();
}

void MyGLDrawer::initializeGL()
{
    // Set up the rendering context, define display lists etc.:
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0F, 0.0F, 0.0F, 1.0F);
    glClearDepth(1.0F);
    setAutoBufferSwap(true);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glDisable(GL_DITHER);

    GLfloat ambientLight[] = { 0.1f, 0.1f, 0.1f, 1.0f};
    GLfloat diffuseLight[] = { 0.05f, 0.05f, 0.05f, 1.0f};
    GLfloat lightPos[]     = { 0.0f, 200.0f, 200.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat no_mat[] = { 0.5, 0.5, 0.8, 1.0 };
    glMaterialfv(GL_FRONT, GL_FRONT_AND_BACK, no_mat);
    model = GL3DS_initialize_dc10();
    qDebug()<<"model="<<model;
}

void MyGLDrawer::resizeGL(int w, int h)
{
    // setup viewport, projection etc.:
    glViewport(0, 0, (GLint)w, (GLint)h);
    /*...
     glFrustum(...);
     ...*/
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f,(GLfloat)width()/(GLfloat)height(),0.1f,100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void MyGLDrawer::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glBegin(GL_TRIANGLES);
        glColor3f(1.0f,0.0f,0.0f); glVertex3f(0.0f,1.0f,0.0f);
        glColor3f(0.0f,1.0f,0.0f); glVertex3f(-1.0f,-1.0f,0.0f);
        glColor3f(0.0f,0.0f,1.0f); glVertex3f(1.0f,-1.0f,0.0f);
    glEnd();

    /*float pos[1][6]={1.0,1.0,1.0,1.0,0.0,0.0};
    glDrawPixels(width(),height(),GL_RGB,GL_FLOAT,pos);*/

    glScalef(0.005f,0.005f,0.005f);
    glTranslatef(camPos[0],camPos[1],camPos[2]);
    //qDebug()<<camPos[0]<<' '<<camPos[1]<<' '<<camPos[2];
    glRotatef(camRot[0],1.0f,0.0f,0.0f);
    glRotatef(camRot[1],0.0f,1.0f,0.0f);
    glRotatef(camRot[2],0.0f,0.0f,1.0f);
    glCallList(model);
    glPopMatrix();
}

void MyGLDrawer::wheelEvent(QWheelEvent *e)
{
    camPos[2] += e->delta();
    update();
}

void MyGLDrawer::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
        mouseLeftButtonFlag = true;
    if(e->button() == Qt::RightButton)
        mouseRightButtonFlag = true;
}

void MyGLDrawer::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
        mouseLeftButtonFlag = false;
    if(e->button() == Qt::RightButton)
        mouseRightButtonFlag = false;
}

void MyGLDrawer::mouseMoveEvent(QMouseEvent *e)
{
    if(mouseLeftButtonFlag)
    {
        setRot(0,e->pos().y() - mousePrevPoint.y());
        setRot(1,e->pos().x() - mousePrevPoint.x());
        update();
    }
    if(mouseRightButtonFlag)
    {
        camPos[0] += (e->x() - mousePrevPoint.x())<<3;
        camPos[1] -= (e->y() - mousePrevPoint.y())<<3;
        update();
    }
    mousePrevPoint = e->pos();
}

void MyGLDrawer::setRot(int axis, int value)
{
    camRot[axis] += (camRot[axis] >=360) ? (-360 + value/2): value/2;
}

