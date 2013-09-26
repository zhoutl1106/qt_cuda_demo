#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QGLWidget>
#include <GL/glut.h>
#include <QPoint>
#include <QPushButton>

class MyGLDrawer : public QGLWidget
{
    Q_OBJECT        // must include this if you use Qt signals/slots

public:
    MyGLDrawer(QWidget *parent);
    void wheelEvent(QWheelEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

protected:

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

private:

    void setRot(int axis, int value);
    int model;

    QPoint mousePrevPoint;
    float camRot[3];
    float camPos[3];
    bool mouseLeftButtonFlag;
    bool mouseRightButtonFlag;
    QPushButton *button;
};

#endif // MYGLWIDGET_H
