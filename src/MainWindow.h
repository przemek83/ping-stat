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

    void flipEditableFieldsEnablement();

    void stopPinging();

    void startPinging();

    void updatePingStatistics(const PingData& pingData);

    Ui::MainWindow* ui;

    HostChecker hostChecker_;

    PlotWidget plotWidget_;

private Q_SLOTS:
    void pingButtonClicked();

    void updatePingData(PingData pingData);
};
