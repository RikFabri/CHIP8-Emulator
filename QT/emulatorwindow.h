#ifndef EMULATORWINDOW_H
#define EMULATORWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class EmulatorWindow; }
QT_END_NAMESPACE

class EmulatorWindow : public QMainWindow
{
    Q_OBJECT

public:
    EmulatorWindow(QWidget *parent = nullptr);
    ~EmulatorWindow();

private slots:
    void on_actionLoad_triggered();
    void on_actionReload_triggered();

    void on_actionAbout_triggered();

private:
    Ui::EmulatorWindow *ui;
};
#endif // EMULATORWINDOW_H
