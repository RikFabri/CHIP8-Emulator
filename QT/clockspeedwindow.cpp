#include "clockspeedwindow.h"
#include "ui_clockspeedwindow.h"

#include "emulatorviewport.h"

ClockSpeedWindow::ClockSpeedWindow(EmulatorViewport* emulatorVp, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClockSpeedWindow),
    m_pEmulatorViewport(emulatorVp),
    m_InitialCpuMultiplier(emulatorVp->GetClockspeedMultiplier())
{
    ui->setupUi(this);
}

ClockSpeedWindow::~ClockSpeedWindow()
{
    delete ui;
}

void ClockSpeedWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->lcdNumber->display(value);
    m_CpuMultiplier = value;
    m_pEmulatorViewport->SetClockspeedMultiplier(m_CpuMultiplier);
}


void ClockSpeedWindow::on_buttonBox_accepted()
{
    m_pEmulatorViewport->SetClockspeedMultiplier(m_CpuMultiplier);
}

void ClockSpeedWindow::on_buttonBox_rejected()
{
    m_pEmulatorViewport->SetClockspeedMultiplier(m_InitialCpuMultiplier);
}
