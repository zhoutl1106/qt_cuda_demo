#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "myglwidget.h"
#include "waveform.h"

namespace Ui {
    class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void resizeEvent(QResizeEvent *);

private slots:
    void on_pushButton_clicked();

private:
    Ui::Widget *ui;
    MyGLDrawer *glWidget;
    WaveForm *waveForm;
    float* test;
    float* testResult;
};

#endif // WIDGET_H
