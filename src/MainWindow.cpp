#include "MainWindow.h"

#include "PlotWidget.h"
#include "StatsDisplayWidget.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), hostChecker_(this)
{
    ui->setupUi(this);

    setupAdressValidator();
    setupStatsDisplay();
    setupPlotWidget();

    connect(ui->pingButton, &QPushButton::clicked, this,
            &MainWindow::pingButtonClicked);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::setupAdressValidator()
{
    QString ipRegexp(
        QStringLiteral("(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:"
                       "25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)"));
    auto adressValidator{
        new QRegExpValidator(QRegExp(ipRegexp), ui->adressLineEdit)};
    ui->adressLineEdit->setValidator(adressValidator);
}

void MainWindow::setupStatsDisplay()
{
    auto stats{new StatsDisplayWidget(this)};
    ui->verticalLayout->addWidget(stats);
    connect(&hostChecker_, &HostChecker::updateStatDisplay, stats,
            &StatsDisplayWidget::updateStatDisplay);
}

void MainWindow::setupPlotWidget()
{
    auto plot{new PlotWidget(this)};
    ui->verticalLayout->addWidget(plot);
    connect(this, &MainWindow::configUpdated, plot, &PlotWidget::configUpdated);
    connect(&hostChecker_, &HostChecker::updatePlotWidget, plot,
            &PlotWidget::updatePlotWidget);
}

void MainWindow::pingButtonClicked()
{
    bool checkerRunning{hostChecker_.isRunning()};
    if (checkerRunning)
    {
        hostChecker_.stop();
        ui->pingButton->setText(tr("Ping"));
    }
    else
    {
        int timeoutValue{ui->timeoutSpin->value()};
        QString host{ui->adressLineEdit->text()};
        hostChecker_.start(ui->intervalSpin->value(), timeoutValue, host);
        Q_EMIT configUpdated(timeoutValue);
        ui->pingButton->setText(tr("Stop"));
    }

    ui->adressLineEdit->setEnabled(checkerRunning);
    ui->intervalSpin->setEnabled(checkerRunning);
    ui->timeoutSpin->setEnabled(checkerRunning);
}
