#include "emulatorwindow.h"
#include "./ui_emulatorwindow.h"

#include <QFileDialog>
#include <QMessageBox>

#include "clockspeedwindow.h"

EmulatorWindow::EmulatorWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::EmulatorWindow)
{
    ui->setupUi(this);
}

EmulatorWindow::~EmulatorWindow()
{
    delete ui;
}

void EmulatorWindow::on_actionReload_triggered()
{
    ui->emulatorWidget->ReloadRom();
}


void EmulatorWindow::on_actionLoad_triggered()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Open rom"), "");

    if(!file.isEmpty())
        ui->emulatorWidget->SetRom(file);
}


void EmulatorWindow::on_actionAbout_triggered()
{
    QMessageBox box{};
    box.setWindowTitle("About");
    box.setText("A chip-8 emulator made by Rik Fabri");
    box.exec();
}

void EmulatorWindow::on_actionClock_speed_triggered()
{
    ClockSpeedWindow csw{ ui->emulatorWidget, this };
    csw.exec();
}

