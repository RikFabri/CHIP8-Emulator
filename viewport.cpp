#include "viewport.h"
#include <QOpenGLShaderProgram>

#include <QMessageBox>

ViewPort::ViewPort(QWidget* pParent)
    : QOpenGLWidget(pParent)
{
}

void ViewPort::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(1, 0.5f, 1, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
}

void ViewPort::paintGL()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void ViewPort::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}