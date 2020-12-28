#include <QIntValidator>
#include <QRegExpValidator>

#include "Constants.h"
#include "HostChecker.h"
#include "MainWindow.h"
#include "PlotWidget.h"
#include "StatsDisplayWidget.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set IP address validator.
    QString ipRegexp(
        QStringLiteral("(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.)"));
    ipRegexp.append(
        QStringLiteral("{3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)"));
    auto adressValidator{
        new QRegExpValidator(QRegExp(ipRegexp), ui->adressLineEdit)};
    ui->adressLineEdit->setValidator(adressValidator);

    // Set interval validator.
    auto intervalValidator{new QIntValidator(0, INT_MAX, ui->intervalLineEdit)};
    ui->intervalLineEdit->setValidator(intervalValidator);

    // Set timeout validator.
    auto timeoutValidator{
        new QIntValidator(0, maxTimeout_, ui->timeoutLineEdit)};
    ui->timeoutLineEdit->setValidator(timeoutValidator);

    auto stats{new StatsDisplayWidget(this)};
    ui->verticalLayout->addWidget(stats);

    auto plot{new PlotWidget(this)};
    ui->verticalLayout->addWidget(plot);
    connect(this, &MainWindow::configUpdated, plot, &PlotWidget::configUpdated);

    hostChecker_ = new HostChecker(this);

    connect(hostChecker_, &HostChecker::updatePlotWidget, plot,
            &PlotWidget::updatePlotWidget);

    connect(hostChecker_, &HostChecker::updateStatDisplay, stats,
            &StatsDisplayWidget::updateStatDisplay);

    connect(ui->checkAdressButton, &QPushButton::clicked, this,
            &MainWindow::checkAdressButtonClicked);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::checkAdressButtonClicked()
{
    bool checkerRunning{hostChecker_->isRunning()};
    if (checkerRunning)
    {
        hostChecker_->stop();
        ui->checkAdressButton->setText(tr("Check address"));
    }
    else
    {
        int timeoutValue{ui->timeoutLineEdit->text().toInt()};
        QString host{ui->adressLineEdit->text()};
        hostChecker_->start(ui->intervalLineEdit->text().toInt(), timeoutValue,
                            host);
        Q_EMIT configUpdated(timeoutValue);
        ui->checkAdressButton->setText(tr("Stop"));
    }

    ui->adressLineEdit->setEnabled(checkerRunning);
    ui->intervalLineEdit->setEnabled(checkerRunning);
    ui->timeoutLineEdit->setEnabled(checkerRunning);
}
