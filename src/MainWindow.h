#pragma once

#include <QMainWindow>

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
    Ui::MainWindow* ui;

    /// Host checker object.
    HostChecker* hostChecker_;

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
