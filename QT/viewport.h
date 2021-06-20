#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QWidget>
#include <QOpenGLWidget>

#include <QOpenGLFunctions>

class QOpenGLShaderProgram;

namespace chip
{
    class Emulator;
}

class ViewPort : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit ViewPort(QWidget* pParent = nullptr);
    

protected:
    void paintEvent(QPaintEvent* pEvent) override;

private:
    void EmulationUpdate();

    QScopedPointer<chip::Emulator> m_Emulator;
};

#endif // VIEWPORT_H
