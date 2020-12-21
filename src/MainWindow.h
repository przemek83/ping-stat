#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
    explicit MainWindow(QWidget* parent = 0);

    virtual ~MainWindow();

private:
    Ui::MainWindow* ui;

    /// Host checker object.
    HostChecker* hostChecker_;

private slots:
    /**
     * @brief on start/stop button clicked.
     */
    void on_checkAdressButton_clicked();

signals:
    /**
     * @brief propagate new configuration value.
     */
    void configUpdated(int);
};

#endif  // MAINWINDOW_H
