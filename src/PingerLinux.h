#pragma once

#include "Pinger.h"

class PingerLinux : public Pinger
{
    Q_OBJECT
public:
    explicit PingerLinux(QObject* parent = nullptr);
    ~PingerLinux() override = default;

protected:
    QString getPingCommand() const override;

    PingData extractPingData(QString pingOutput) const override;

private:
    std::pair<int, int> getPacketsInfo(const QString& line) const;

    std::tuple<int, int, int> getTimesInfo(const QString& line) const;
};
