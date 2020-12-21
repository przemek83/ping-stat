#pragma once

#include <QDateTime>
#include <QWidget>

namespace Ui
{
class StatsDisplayWidget;
}

class StatsDisplayWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StatsDisplayWidget(QWidget* parent = nullptr);

    virtual ~StatsDisplayWidget();

private:
    Q_DISABLE_COPY(StatsDisplayWidget)

    Ui::StatsDisplayWidget* ui;

private slots:
    /**
     * @brief update display widget using value.
     * @param time time of ping.
     * @param packetsSent number of sent packets.
     * @param packetsLost number of lost packets.
     * @param avgReturnTime Average ping return time.
     * @param min minimum reply time.
     * @param max maximum reply time.
     */
    void updateStatDisplay(QDateTime time, int packetsSent, int packetsLost,
                           int avgReturnTime, int min, int max);
};
