#include "widget.h"
#include "ui_widget.h"
#include <math.h>
#define PI  3.1415926
#define WIDTH 200

extern "C" void* runCudaPart(void* _src);
extern "C" void cudaInit();
extern "C" void cudaExit();

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    glWidget = new MyGLDrawer(this);
    glWidget->show();
    waveForm = new WaveForm(this);
    waveForm->show();
    waveForm->setCurveColor(Qt::white);
    waveForm->setBackgoundColor(Qt::blue);
    waveForm->setDrawLength(1024);

    test = new float[32768];
    testResult = new float[32768];
    for(int i=0;i<32768;i++)
        test[i] = sin(2*PI*i*500/16000);
    for(int i=512;i<32768;i++)
        test[i] = rand() / (float)RAND_MAX;

    waveForm->setDataPointer(test,DATA_TYPE_FLOAT);
    cudaInit();
}

Widget::~Widget()
{
    cudaExit();
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    testResult = (float*)runCudaPart(test);
    ui->textBrowser->clear();
    for(int i=0;i<256;i++)
        ui->textBrowser->append(QString::number(i) + ' ' + QString::number(testResult[i]));

    waveForm->setDrawLength(8192);
    waveForm->setDataPointer(testResult,DATA_TYPE_FLOAT);
}

void Widget::resizeEvent(QResizeEvent *e)
{
    waveForm->resize(width()-WIDTH,height()>>1);
    waveForm->move(0,0);

    glWidget->resize(width()-WIDTH,height()>>1);
    glWidget->move(0,height()>>1);
    ui->textBrowser->resize(WIDTH,height()-20);
    ui->textBrowser->move(width()-WIDTH,20);
    ui->pushButton->move(width()-WIDTH,0);
}
