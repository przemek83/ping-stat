#pragma once

#include "Pinger.h"

class PingerLinux : public Pinger
{
public:
    explicit PingerLinux(QObject* parent = nullptr);
    ~PingerLinux() = default;

protected:
    QString getPingCommand() const override;

    PingData extractPingData(QString pingOutput) const override;

private:
    std::pair<int, int> getPacketsInfo(const QString& line) const;

    std::tuple<int, int, int> getTimesInfo(const QString& line) const;
};
