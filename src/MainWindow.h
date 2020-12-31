#pragma once

#include <QMainWindow>

#include "HostChecker.h"

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

    virtual ~MainWindow();

private:
    void setupValidators();

    void setupAdressValidator();

    void setupIntervalValidator();

    void setupTimeoutValidator();

    void setupStatsDisplay();

    void setupPlotWidget();

    Ui::MainWindow* ui;

    HostChecker hostChecker_;

private Q_SLOTS:
    /**
     * @brief on start/stop button clicked.
     */
    void checkAdressButtonClicked();

Q_SIGNALS:
    /**
     * @brief propagate new configuration value.
     */
    void configUpdated(int);
};
