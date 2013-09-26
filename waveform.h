#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <QWidget>

#define DATA_TYPE_CHAR      0
#define DATA_TYPE_SHORT     1
#define DATA_TYPE_FLOAT     2
#define DATA_TYPE_DOUBLE    3

class WaveForm : public QWidget
{
    Q_OBJECT

public:
    explicit WaveForm(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);
    void setBackgoundColor(QColor clr);
    void setCurveColor(QColor clr);
    void setDataPointer(void *dat,int type);
    void setDrawLength(int length);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *event);
    ~WaveForm();

private:
    QColor m_bgColor;
    QColor m_curveColor;
    void *m_dat;
    double m_gain;
    bool enableFlag;
    int length;
    int m_dataType;
    QPoint mouseClickPos;
};

#endif // WAVEFORM_H
