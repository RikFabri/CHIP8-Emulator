#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QWidget>
#include <QOpenGLWidget>

#include <QOpenGLFunctions>

class QOpenGLShaderProgram;

class ViewPort : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit ViewPort(QWidget* pParent = nullptr);

    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void resizeGL(int w, int h) override;

private:
    QOpenGLShaderProgram* m_pShaderProgram;
};

#endif // VIEWPORT_H
