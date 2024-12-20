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

    auto* pingProcess{new QProcess(this)};
    connect(pingProcess, &QProcess::finished, this, &Pinger::pingFinished);

    pingProcess->startCommand(getPingCommand());
}

void Pinger::logPingData(const PingData& pingData) const
{
    QString logMsg;
    QTextStream out(&logMsg);
    out << pingData.time_.toString(logTimeFormat_) << ",";
    out << host_ << "," << pingData.packetsSent_ << "," << pingData.packetsLost_
        << "," << pingData.min_ << "," << pingData.max_ << ","
        << pingData.avgReturnTime_ << "\n";
    Logger::getInstance().info(logMsg);
}

void Pinger::logError(const QString& errorMsg) const
{
    QDateTime dateTime{QDateTime::currentDateTime()};
    QString logMsg;
    QTextStream out(&logMsg);
    out << dateTime.toString(logTimeFormat_) << ",";
    out << errorMsg << "\n";
    Logger::getInstance().info(logMsg);
}

void Pinger::fillFieldsWithTimeout(PingData& pingData) const
{
    pingData.avgReturnTime_ = timeout_;
    pingData.min_ = timeout_;
    pingData.max_ = timeout_;
}

void Pinger::pingFinished([[maybe_unused]] int exitCode,
                          [[maybe_unused]] QProcess::ExitStatus exitStatus)
{
    auto* ping{::qobject_cast<QProcess*>(sender())};
    if (ping == nullptr)
    {
        logError(QStringLiteral("Error: internal."));
        return;
    }

    QString result{QString::fromLatin1(ping->readAllStandardOutput())};
    PingData pingData{extractPingData(result)};
    if (pingData.packetsSent_ == 0)
    {
        logError(QStringLiteral("Error: wrong return results."));
        return;
    }

    if (pingData.packetsSent_ == pingData.packetsLost_)
        fillFieldsWithTimeout(pingData);

    logPingData(pingData);

    Q_EMIT newPingData(pingData);
}
