#-------------------------------------------------
#
# Project created by QtCreator 2013-05-20T19:51:32
#
#-------------------------------------------------

QT       += core gui multimedia opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cuda_test
TEMPLATE = app

INCLUDEPATH *= /usr/local/cuda-5.0 /home/heu/NVIDIA_CUDA-5.0_Samples/common/inc
LIBS *= -L/usr/local/cuda-5.0/lib -lcudart -lcufft -lGLU


SOURCES += main.cpp\
        widget.cpp \
    myglwidget.cpp \
    waveform.cpp \
    hello.cu

HEADERS  += widget.h \
    myglwidget.h \
    waveform.h \
    submarinev3.h

FORMS    += widget.ui

OTHER_FILES += hello.cu










