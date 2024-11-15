#pragma once

#include <QDateTime>

struct PingData
{
    QDateTime time;
    int packetsSent{0};
    int packetsLost{0};
    int avgReturnTime{0};
    int min{0};
    int max{0};
};
