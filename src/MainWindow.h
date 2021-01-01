#pragma once

#include <QMainWindow>

#include "HostChecker.h"
#include "PlotWidget.h"

class HostChecker;

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

    ~MainWindow();

private:
    void setupAdressValidator();

    void setupStatsDisplay();

    void setupPlotWidget();

    void flipEditableFieldsEnablement();

    void stopPinging();

    void startPinging();

    Ui::MainWindow* ui;

    HostChecker hostChecker_;

    PlotWidget plotWidget_;

private Q_SLOTS:
    void pingButtonClicked();

    void updateStatDisplay(const QDateTime& time, int packetsSent,
                           int packetsLost, int avgReturnTime, int min,
                           int max);
};
