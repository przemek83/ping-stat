#include "MainWindow.h"

#include "Constants.h"
#include "PingerLinux.h"
#include "PingerWindows.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), plotWidget_(this)
{
#ifdef _WIN32
    pinger_ = std::make_unique<PingerWindows>(this);
#else
    pinger_ = std::make_unique<PingerLinux>(this);
#endif
    ui->setupUi(this);

    setupAdressValidator();

    ui->verticalLayout->addWidget(&plotWidget_);

    connect(ui->pingButton, &QPushButton::clicked, this,
            &MainWindow::pingButtonClicked);

    connect(&*pinger_, &Pinger::newPingData, this, &MainWindow::updatePingData);
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

void MainWindow::flipEditableFieldsEnablement()
{
    ui->adressLineEdit->setEnabled(!ui->adressLineEdit->isEnabled());
    ui->intervalSpin->setEnabled(!ui->intervalSpin->isEnabled());
    ui->timeoutSpin->setEnabled(!ui->timeoutSpin->isEnabled());
}

void MainWindow::stopPinging()
{
    pinger_->stop();
    ui->pingButton->setText(tr("Ping"));
}

void MainWindow::startPinging()
{
    const int timeoutValue{ui->timeoutSpin->value()};
    const QString host{ui->adressLineEdit->text()};
    pinger_->start(ui->intervalSpin->value(), timeoutValue, host);
    plotWidget_.setTimeoutValue(timeoutValue);
    ui->pingButton->setText(tr("Stop"));
}

void MainWindow::updatePingStatistics(const PingData& pingData)
{
    ui->timeLabelValue->setText(
        pingData.time.toString(Constants::getDisplayTimeFormat()));
    ui->packetsSentValue->setText(QString::number(pingData.packetsSent));
    ui->packetsLostValue->setText(QString::number(pingData.packetsLost));
    ui->avgTimeValue->setText(QString::number(pingData.avgReturnTime) +
                              tr("ms"));
    ui->minTimeValue->setText(QString::number(pingData.min) + tr("ms"));
    ui->maxTimeValue->setText(QString::number(pingData.max) + tr("ms"));
}

void MainWindow::pingButtonClicked()
{
    if (pinger_->isRunning())
        stopPinging();
    else
        startPinging();
    flipEditableFieldsEnablement();
}

void MainWindow::updatePingData(PingData pingData)
{
    plotWidget_.updatePlotWidget(pingData.avgReturnTime, pingData.time);
    updatePingStatistics(pingData);
}
