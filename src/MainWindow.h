#pragma once

#include <QMainWindow>

#include "HostChecker.h"
#include "PlotWidget.h"
#include "StatsDisplayWidget.h"

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

    void setEditableFieldsEnabled(bool enable);

    Ui::MainWindow* ui;

    HostChecker hostChecker_;

    PlotWidget plotWidget_;

    StatsDisplayWidget statsDisplayWidget_;

private Q_SLOTS:
    void pingButtonClicked();
};
