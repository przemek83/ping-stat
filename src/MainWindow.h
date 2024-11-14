#pragma once

#include <QValidator>
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

    ~MainWindow() override;

private:
    void setupAdressValidator();

    void flipEditableFieldsEnablement();

    void stopPinging();

    void startPinging();

    void updatePingStatistics(const PingData& pingData);

    std::unique_ptr<Ui::MainWindow> ui_;

    std::unique_ptr<Pinger> pinger_{nullptr};

    PlotWidget plotWidget_;

    QRegularExpressionValidator validator_;

private Q_SLOTS:
    void pingButtonClicked();

    void updatePingData(const PingData& pingData);
};
