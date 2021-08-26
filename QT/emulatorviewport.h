#ifndef EMULATORVIEWPORT_H
#define EMULATORVIEWPORT_H

#include <QOpenGLWidget>
#include <VM.h>

// A QT widget managing and rendering the actual CHIP-8 VM
class EmulatorViewport : public QOpenGLWidget
{
public:
    explicit EmulatorViewport(QWidget* pParent = nullptr);

    void SetRom(const QString& path);
    void ReloadRom();

    void SetClockspeedMultiplier(int multiplier);
    int GetClockspeedMultiplier() const;
private:
    CHIP::VM m_Chip8Vm;
    bool m_MainLoopTimerStarted;

    // The amount of times the cpu updates per tick, NOT the underlying timer.
    int m_ClockspeedMultiplier;

    void UpdateEmulator();
    void paintEvent(QPaintEvent *pEvent) override;

    void keyPressEvent(QKeyEvent* pEvent) override;
    void keyReleaseEvent(QKeyEvent* pEvent) override;
};

#endif // EMULATORVIEWPORT_H
