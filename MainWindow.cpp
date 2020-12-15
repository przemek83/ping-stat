#include <QRegExpValidator>
#include <QIntValidator>

#include "Constants.h"
#include "PlotWidget.h"
#include "StatsDisplayWidget.h"
#include "HostChecker.h"
#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Set ip adress validator.
    QString ipRegexp("(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.)");
    ipRegexp.append("{3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)");
    QRegExpValidator* adressValidator =
        new QRegExpValidator(QRegExp(ipRegexp), ui->adressLineEdit);
    ui->adressLineEdit->setValidator(adressValidator);

    //Set interval validator.
    QIntValidator* intervalValidator =
        new QIntValidator(0, INT_MAX, ui->intervalLineEdit);
    ui->intervalLineEdit->setValidator(intervalValidator);

    //Set timeout validator.
    QIntValidator* timeoutValidator =
        new QIntValidator(0, Constants::maxTimeout(), ui->timeoutLineEdit);
    ui->timeoutLineEdit->setValidator(timeoutValidator);

    StatsDisplayWidget* stats = new StatsDisplayWidget(this);
    ui->verticalLayout->addWidget(stats);

    PlotWidget* plot = new PlotWidget(this);
    ui->verticalLayout->addWidget(plot);
    connect(this,
            SIGNAL(configUpdated(int)),
            plot,
            SLOT(configUpdated(int)));

    hostChecker_ = new HostChecker(this);

    connect(hostChecker_,
            SIGNAL(updatePlotWidget(int, QDateTime)),
            plot,
            SLOT(updatePlotWidget(int, QDateTime)));

    connect(hostChecker_,
            SIGNAL(updateStatDisplay(QDateTime,int,int,int,int,int)),
            stats,
            SLOT(updateStatDisplay(QDateTime,int,int,int,int,int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_checkAdressButton_clicked()
{
    bool checkerRunning = hostChecker_->isRunning();
    if(true == checkerRunning)
    {
        hostChecker_->stop();
        ui->checkAdressButton->setText(tr("Check adress"));
    }
    else
    {
        int timeoutValue = ui->timeoutLineEdit->text().toInt();
        QString host = ui->adressLineEdit->text();
        hostChecker_->start(ui->intervalLineEdit->text().toInt(),
                            timeoutValue,
                            host);
        emit configUpdated(timeoutValue);
        ui->checkAdressButton->setText(tr("Stop"));
    }

    ui->adressLineEdit->setEnabled(checkerRunning);
    ui->intervalLineEdit->setEnabled(checkerRunning);
    ui->timeoutLineEdit->setEnabled(checkerRunning);
}
