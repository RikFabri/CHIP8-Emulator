#ifndef EMULATORVIEWPORT_H
#define EMULATORVIEWPORT_H

#include <QOpenGLWidget>
#include <VM.h>

class EmulatorViewport : public QOpenGLWidget
{
public:
    explicit EmulatorViewport(QWidget* pParent = nullptr);

    void SetRom(const QString& path);
    void ReloadRom();
private:
    CHIP::VM m_Chip8Vm;

    void UpdateEmulator();
    void paintEvent(QPaintEvent *pEvent) override;

    void keyPressEvent(QKeyEvent* pEvent) override;
    void keyReleaseEvent(QKeyEvent* pEvent) override;
};

#endif // EMULATORVIEWPORT_H
