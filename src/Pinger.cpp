#include "Pinger.h"

#include <QDateTime>
#include <QTextStream>

#include "Logger.h"

Pinger::Pinger(QObject* parent) : QObject(parent) {}

void Pinger::start(int intervalInSeconds, int timeout, const QString& host)
{
    if (isRunning())
        stop();
    host_ = host;
    timeout_ = timeout;
    const int milisecondsInSecond{1000};
    timerId_ = startTimer(intervalInSeconds * milisecondsInSecond);
}

void Pinger::stop()
{
    killTimer(timerId_);
    timerId_ = 0;
}

bool Pinger::isRunning() const { return timerId_ != 0; }

QString Pinger::getHost() const { return host_; }

int Pinger::getTimeout() const { return timeout_; }

void Pinger::timerEvent([[maybe_unused]] QTimerEvent* event)
{
    if (!isRunning())
        return;

    auto pingProcess{new QProcess(this)};
    connect(pingProcess, &QProcess::finished, this, &Pinger::pingFinished);

    pingProcess->startCommand(getPingCommand());
}

void Pinger::logPingData(const PingData& pingData) const
{
    QString logMsg;
    QTextStream out(&logMsg);
    out << pingData.time.toString(logTimeFormat_) << ",";
    out << host_ << "," << pingData.packetsSent << "," << pingData.packetsLost
        << "," << pingData.min << "," << pingData.max << ","
        << pingData.avgReturnTime << "\n";
    Logger::getInstance().log(logMsg);
}

void Pinger::logError(const QString& errorMsg) const
{
    QDateTime time{QDateTime::currentDateTime()};
    QString logMsg;
    QTextStream out(&logMsg);
    out << time.toString(logTimeFormat_) << ",";
    out << errorMsg << "\n";
    Logger::getInstance().log(logMsg);
}

void Pinger::fillFieldsWithTimeout(PingData& pingData) const
{
    pingData.avgReturnTime = timeout_;
    pingData.min = timeout_;
    pingData.max = timeout_;
}

void Pinger::pingFinished([[maybe_unused]] int exitCode,
                          [[maybe_unused]] QProcess::ExitStatus exitStatus)
{
    auto ping{qobject_cast<QProcess*>(sender())};
    if (ping == nullptr)
    {
        logError(QStringLiteral("Error: internal."));
        return;
    }

    QString result{QString::fromLatin1(ping->readAllStandardOutput())};
    PingData pingData{extractPingData(result)};
    if (pingData.packetsSent == 0)
    {
        logError(QStringLiteral("Error: wrong return results."));
        return;
    }

    if (pingData.packetsSent == pingData.packetsLost)
        fillFieldsWithTimeout(pingData);

    logPingData(pingData);

    Q_EMIT newPingData(pingData);
}
