#pragma once

#include <memory>

#include <QMainWindow>

#include "Pinger.h"
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

    std::unique_ptr<Pinger> pinger_{nullptr};

    PlotWidget plotWidget_;

private Q_SLOTS:
    void pingButtonClicked();

    void updatePingData(const PingData& pingData);
};
