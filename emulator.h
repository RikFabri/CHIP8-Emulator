#ifndef EMULATOR_H
#define EMULATOR_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Emulator; }
QT_END_NAMESPACE

class Emulator : public QMainWindow
{
    Q_OBJECT

public:
    Emulator(QWidget *parent = nullptr);
    ~Emulator();

private slots:
    void on_actionLoad_triggered();

private:
    Ui::Emulator *ui;

    
};
#endif // EMULATOR_H
