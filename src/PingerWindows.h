#pragma once

#include "Pinger.h"

class PingerWindows : public Pinger
{
public:
    explicit PingerWindows(QObject* parent = nullptr);
    ~PingerWindows() = default;

protected:
    QString getPingCommand() const override;

    PingData extractPingData(QString pingOutput) const override;

private:
    /**
     * @brief Get value from ping results  string.
     * @param resultString output of ping.
     * @param valueName substring to find.
     * @param fromIndex search result from index.
     * @param endIndex remember last end index.
     * @return extracted value or 0 if not found.
     */
    int getValue(QString& resultString, const QString& valueName, int fromIndex,
                 int& endIndex) const;
};
