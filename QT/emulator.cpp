#include "emulator.h"
#include "./ui_emulator.h"
#include <QMessageBox>

Emulator::Emulator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Emulator)
{
    ui->setupUi(this);
}

Emulator::~Emulator()
{
    delete ui;
}


void Emulator::on_actionLoad_triggered()
{
    QMessageBox msg;
    msg.setText("henlo");
    msg.show();
    msg.exec();
}

