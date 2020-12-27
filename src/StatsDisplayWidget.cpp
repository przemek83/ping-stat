#include <QDateTime>

#include "Constants.h"
#include "StatsDisplayWidget.h"
#include "ui_StatsDisplayWidget.h"

StatsDisplayWidget::StatsDisplayWidget(QWidget* parent)
    : QWidget(parent), ui(new Ui::StatsDisplayWidget)
{
    ui->setupUi(this);
}

StatsDisplayWidget::~StatsDisplayWidget() { delete ui; }

void StatsDisplayWidget::updateStatDisplay(QDateTime time, int packetsSent,
                                           int packetsLost, int avgReturnTime,
                                           int min, int max)
{
    ui->timeLabelValue->setText(
        time.toString(Constants::getDisplayTimeFormat()));
    ui->packetsSentValue->setText(QString::number(packetsSent));
    ui->packetsLostValue->setText(QString::number(packetsLost));
    ui->avgTimeValue->setText(QString::number(avgReturnTime) + tr("ms"));
    ui->minTimeValue->setText(QString::number(min) + tr("ms"));
    ui->maxTimeValue->setText(QString::number(max) + tr("ms"));
}
