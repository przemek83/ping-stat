#include "MainWindow.h"

#include "Constants.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      hostChecker_(this),
      plotWidget_(this)
{
    ui->setupUi(this);

    setupAdressValidator();
    setupPlotWidget();

    connect(ui->pingButton, &QPushButton::clicked, this,
            &MainWindow::pingButtonClicked);

    connect(&hostChecker_, &HostChecker::newPingData, this,
            &MainWindow::updatePingData);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::setupAdressValidator()
{
    const QString ipRegexp(
        QStringLiteral("(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:"
                       "25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)"));
    auto adressValidator{
        new QRegExpValidator(QRegExp(ipRegexp), ui->adressLineEdit)};
    ui->adressLineEdit->setValidator(adressValidator);
}

void MainWindow::setupPlotWidget()
{
    ui->verticalLayout->addWidget(&plotWidget_);
}

void MainWindow::flipEditableFieldsEnablement()
{
    ui->adressLineEdit->setEnabled(!ui->adressLineEdit->isEnabled());
    ui->intervalSpin->setEnabled(!ui->intervalSpin->isEnabled());
    ui->timeoutSpin->setEnabled(!ui->timeoutSpin->isEnabled());
}

void MainWindow::stopPinging()
{
    hostChecker_.stop();
    ui->pingButton->setText(tr("Ping"));
}

void MainWindow::startPinging()
{
    const int timeoutValue{ui->timeoutSpin->value()};
    const QString host{ui->adressLineEdit->text()};
    hostChecker_.start(ui->intervalSpin->value(), timeoutValue, host);
    plotWidget_.setTimeoutValue(timeoutValue);
    ui->pingButton->setText(tr("Stop"));
}

void MainWindow::pingButtonClicked()
{
    if (hostChecker_.isRunning())
        stopPinging();
    else
        startPinging();
    flipEditableFieldsEnablement();
}

void MainWindow::updatePingData(PingData pingData)
{
    plotWidget_.updatePlotWidget(pingData.avgReturnTime, pingData.time);

    ui->timeLabelValue->setText(
        pingData.time.toString(Constants::getDisplayTimeFormat()));
    ui->packetsSentValue->setText(QString::number(pingData.packetsSent));
    ui->packetsLostValue->setText(QString::number(pingData.packetsLost));
    ui->avgTimeValue->setText(QString::number(pingData.avgReturnTime) +
                              tr("ms"));
    ui->minTimeValue->setText(QString::number(pingData.min) + tr("ms"));
    ui->maxTimeValue->setText(QString::number(pingData.max) + tr("ms"));
}
