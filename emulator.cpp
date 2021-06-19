#include "emulator.h"
#include "./ui_emulator.h"

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

