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

    ~Pinger() override = default;

    void start(int intervalInSeconds, int timeout, const QString& host);

    void stop();

    bool isRunning() const;

protected:
    void timerEvent(QTimerEvent* event) override;

    virtual QString getPingCommand() const = 0;

    virtual PingData extractPingData(QString pingOutput) const = 0;

    QString getHost() const;

    int getTimeout() const;

private:
    void logPingData(const PingData& pingData) const;

    void logError(const QString& errorMsg) const;

    void fillFieldsWithTimeout(PingData& pingData) const;

    QString host_;

    int timerId_{0};

    int timeout_{0};

    const QString logTimeFormat_{QStringLiteral("HH:mm:ss")};

private slots:
    void pingFinished(int, QProcess::ExitStatus exitStatus);

signals:
    void newPingData(PingData);
};
