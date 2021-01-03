#pragma once

#include <QObject>
#include <QProcess>

#include "PingData.h"

class QTimerEvent;

class Pinger : public QObject
{
    Q_OBJECT
public:
    explicit Pinger(QObject* parent = nullptr);

    ~Pinger() = default;

    /**
     * @brief start host checker using giving parameters.
     * @param intervalInSeconds time between calls of ping.
     * @param timeout maximum time ping wits for reply.
     * @param host IP of host.
     */
    void start(int intervalInSeconds, int timeout, const QString& host);

    void stop();

    bool isRunning();

protected:
    void timerEvent(QTimerEvent* event) override;

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

    QString getPingCommand() const;

    PingData extractPingData(QString pingOutput) const;

    void logPingData(const PingData& pingData) const;

    void logError(const QString& errorMsg) const;

    QString host_;

    int timerId_{0};

    int timeout_{0};

    const QString logTimeFormat_{QStringLiteral("HH:mm:ss")};

private Q_SLOTS:
    void pingFinished(int, QProcess::ExitStatus exitStatus);

Q_SIGNALS:
    void newPingData(PingData);
};
