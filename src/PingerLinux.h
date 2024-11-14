#pragma once

#include "Pinger.h"

class PingerLinux : public Pinger
{
    Q_OBJECT
public:
    using Pinger::Pinger;

protected:
    QString getPingCommand() const override;

    PingData extractPingData(QString pingOutput) const override;
};
