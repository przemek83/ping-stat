#pragma once

#include <QDateTime>

struct PingData
{
    QDateTime time_;
    int packetsSent_{0};
    int packetsLost_{0};
    int avgReturnTime_{0};
    int min_{0};
    int max_{0};
};
