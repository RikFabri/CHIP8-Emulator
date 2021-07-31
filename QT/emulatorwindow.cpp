#include "emulatorwindow.h"
#include "./ui_emulatorwindow.h"

#include <QFileDialog>
#include <QMessageBox>

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
    ui->openGLWidget->ReloadRom();
}


void EmulatorWindow::on_actionLoad_triggered()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Open rom"), "");

    if(!file.isEmpty())
        ui->openGLWidget->SetRom(file);
}


void EmulatorWindow::on_actionAbout_triggered()
{
    QMessageBox box{};
    box.setWindowTitle("About");
    box.setText("A chip-8 emulator made by Rik Fabri");
    box.exec();
}

