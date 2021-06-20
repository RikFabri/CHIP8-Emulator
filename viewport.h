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
    ~ViewPort();

    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void resizeGL(int w, int h) override;

private:
    QOpenGLShaderProgram* m_pShaderProgram;

    unsigned int m_VerticesId, m_IndicesId;
    int m_TexLocation;

    float m_Vertices[20];
    unsigned int m_Indices[6];
};

#endif // VIEWPORT_H
