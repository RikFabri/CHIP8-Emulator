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
    // Load new rom
    void on_actionLoad_triggered();
    // Reload current rom
    void on_actionReload_triggered();

    // Show about page
    void on_actionAbout_triggered();
    // Open window to change clock speed
    void on_actionClock_speed_triggered();

private:
    Ui::EmulatorWindow *ui;
};
#endif // EMULATORWINDOW_H
