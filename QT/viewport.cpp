#include "viewport.h"

#include <QMessageBox>
#include <qpainter.h>
#include <qtimer.h>

#include "Emulator/Emulator.h"

ViewPort::ViewPort(QWidget* pParent)
    : QOpenGLWidget(pParent)
{
    QTimer* pTimer = new QTimer(this);
    connect(pTimer, &QTimer::timeout, this, &ViewPort::EmulationUpdate);
    pTimer->start(1000);
}

void ViewPort::paintEvent(QPaintEvent* pEvent)
{

    QPixmap texture("Tomato.png");
    QPainter painter;
    painter.begin(this);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.drawPixmap(0, 0, 500, 500, texture);

    painter.end();

 
}

void ViewPort::EmulationUpdate()
{
    m_Emulator->Update();
}
