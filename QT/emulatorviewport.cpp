#include "emulatorviewport.h"

#include <qtimer.h>
#include <qpainter.h>
#include <qevent.h>
#include <qrgb.h>

EmulatorViewport::EmulatorViewport(QWidget *pParent)
    : m_Chip8Vm()
    , m_MainLoopTimerStarted(false)
    , m_ClockspeedMultiplier(1)
{
    setFocusPolicy(Qt::FocusPolicy::StrongFocus);
}

void EmulatorViewport::SetRom(const QString &path)
{
    m_Chip8Vm.LoadRom(path.toStdString());

    if (m_MainLoopTimerStarted)
        return;

    QTimer* pTimer = new QTimer(this);
    connect(pTimer, &QTimer::timeout, this, &EmulatorViewport::UpdateEmulator);
    pTimer->start(1);
    m_MainLoopTimerStarted = true;
}

void EmulatorViewport::ReloadRom()
{
    m_Chip8Vm.ReloadRom();
}

void EmulatorViewport::SetClockspeedMultiplier(int multiplier)
{
    m_ClockspeedMultiplier = multiplier;
}

int EmulatorViewport::GetClockspeedMultiplier() const
{
    return m_ClockspeedMultiplier;
}

void EmulatorViewport::UpdateEmulator()
{
    for (int i = 0; i < m_ClockspeedMultiplier; ++i)
        m_Chip8Vm.Update();

    if (m_Chip8Vm.DidDisplayUpdate())
        repaint();
}

void EmulatorViewport::paintEvent(QPaintEvent *pEvent)
{
    // Generate image from chip display data
    const auto& data = m_Chip8Vm.GetDisplay();
    QImage image{ (uchar*)&data[0], 64, 32, QImage::Format::Format_Indexed8 };

    // Set image's colour palette to black and white
    image.setColorCount(2);
    image.setColorTable(QList<QRgb>{QColor("black").rgb(), QColor("white").rgb()});

    // Create texture from image
    const auto ChipDisplayTexture = QPixmap::fromImage(image);

    // Draw
    QPainter painter;
    painter.begin(this);
    painter.setRenderHints(QPainter::Antialiasing); // No AA

    painter.drawPixmap(0, 0, 64 * 10, 32 * 10, ChipDisplayTexture); // Draw virtual machine's display

    painter.end();
}

void EmulatorViewport::keyPressEvent(QKeyEvent* pEvent)
{
    m_Chip8Vm.KeyDown(pEvent->key());
    QWidget::keyPressEvent(pEvent);
}

void EmulatorViewport::keyReleaseEvent(QKeyEvent* pEvent)
{
    m_Chip8Vm.KeyUp(pEvent->key());
    QWidget::keyReleaseEvent(pEvent);
}
