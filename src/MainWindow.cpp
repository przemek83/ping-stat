#include "MainWindow.h"

#include "Constants.h"
#include "Pinger.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(std::unique_ptr<Pinger> pinger, QWidget* parent)
    : QMainWindow(parent),
      ui_{std::make_unique<Ui::MainWindow>()},
      pinger_{std::move(pinger)},
      plotWidget_(this)
{
    ui_->setupUi(this);

    setupAdressValidator();

    ui_->verticalLayout->addWidget(&plotWidget_);

    connect(ui_->pingButton, &QPushButton::clicked, this,
            &MainWindow::pingButtonClicked);

    connect(&*pinger_, &Pinger::newPingData, this, &MainWindow::updatePingData);
}

MainWindow::~MainWindow() = default;

void MainWindow::setupAdressValidator()
{
    const auto ipRegexp(
        QStringLiteral("(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:"
                       "25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)"));

    validator_.setRegularExpression(QRegularExpression(ipRegexp));
    ui_->adressLineEdit->setValidator(&validator_);
}

void MainWindow::flipEditableFieldsEnablement()
{
    ui_->adressLineEdit->setEnabled(!ui_->adressLineEdit->isEnabled());
    ui_->intervalSpin->setEnabled(!ui_->intervalSpin->isEnabled());
    ui_->timeoutSpin->setEnabled(!ui_->timeoutSpin->isEnabled());
}

void MainWindow::stopPinging()
{
    pinger_->stop();
    ui_->pingButton->setText(tr("Ping"));
}

void MainWindow::startPinging()
{
    const int timeoutValue{ui_->timeoutSpin->value()};
    const QString host{ui_->adressLineEdit->text()};
    pinger_->start(ui_->intervalSpin->value(), timeoutValue, host);
    plotWidget_.setTimeoutValue(timeoutValue);
    ui_->pingButton->setText(tr("Stop"));
}

void MainWindow::updatePingStatistics(const PingData& pingData)
{
    ui_->timeLabelValue->setText(
        pingData.time.toString(Constants::getDisplayTimeFormat()));
    ui_->packetsSentValue->setText(QString::number(pingData.packetsSent));
    ui_->packetsLostValue->setText(QString::number(pingData.packetsLost));
    ui_->avgTimeValue->setText(QString::number(pingData.avgReturnTime) +
                               tr("ms"));
    ui_->minTimeValue->setText(QString::number(pingData.min) + tr("ms"));
    ui_->maxTimeValue->setText(QString::number(pingData.max) + tr("ms"));
}

void MainWindow::pingButtonClicked()
{
    if (pinger_->isRunning())
        stopPinging();
    else
        startPinging();
    flipEditableFieldsEnablement();
}

void MainWindow::updatePingData(const PingData& pingData)
{
    plotWidget_.updatePlotWidget(pingData.avgReturnTime, pingData.time);
    updatePingStatistics(pingData);
}
