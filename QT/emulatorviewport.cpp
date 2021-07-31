#include "emulatorviewport.h"

#include <qtimer.h>
#include <qpainter.h>
#include <qevent.h>
#include <qrgb.h>

EmulatorViewport::EmulatorViewport(QWidget *pParent)
    : m_Chip8Vm()
{
    setFocusPolicy(Qt::FocusPolicy::StrongFocus);
}

void EmulatorViewport::SetRom(const QString &path)
{
    m_Chip8Vm.LoadRom(path.toStdString());


    QTimer* pTimer = new QTimer(this);
    connect(pTimer, &QTimer::timeout, this, &EmulatorViewport::UpdateEmulator);
    pTimer->start(1);
}

void EmulatorViewport::ReloadRom()
{
    m_Chip8Vm.ReloadRom();
}

void EmulatorViewport::UpdateEmulator()
{
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
    painter.fillRect(0, 0, 64 * 10, 32 * 10, QColor(0,0,0)); // Clear display

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
