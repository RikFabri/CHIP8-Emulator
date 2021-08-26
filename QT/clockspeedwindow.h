#ifndef CLOCKSPEEDWINDOW_H
#define CLOCKSPEEDWINDOW_H

#include <QDialog>

class EmulatorViewport;

namespace Ui {
class ClockSpeedWindow;
}

class ClockSpeedWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ClockSpeedWindow(EmulatorViewport* emulatorVp, QWidget *parent = nullptr);
    ~ClockSpeedWindow();

private slots:
    // Changes cpu speed multiplier according to slider and displays it
    void on_horizontalSlider_valueChanged(int value);

    // Closes window and saves new speed multiplier
    void on_buttonBox_accepted();
    // Closes window and discards new speed multiplier
    void on_buttonBox_rejected();

private:
    Ui::ClockSpeedWindow *ui;

    int m_CpuMultiplier;
    const int m_InitialCpuMultiplier;

    EmulatorViewport* m_pEmulatorViewport;
};

#endif // CLOCKSPEEDWINDOW_H
