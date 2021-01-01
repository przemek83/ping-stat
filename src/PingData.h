#pragma once

#include <QDateTime>

struct PingData
{
    QDateTime time;
    int packetsSent;
    int packetsLost;
    int avgReturnTime;
    int min;
    int max;
};
