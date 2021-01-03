#include <QDateTime>
#include <QDebug>

#include "Constants.h"
#include "Logger.h"
#include "Pinger.h"

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

bool Pinger::isRunning() { return timerId_ != 0; }

void Pinger::timerEvent([[maybe_unused]] QTimerEvent* event)
{
    if (!isRunning())
        return;

    auto pingProcess{new QProcess(this)};
    connect(pingProcess,
            static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(
                &QProcess::finished),
            this, &Pinger::pingFinished);

    pingProcess->start(QStringLiteral("ping.exe -w ") +
                       QString::number(timeout_) + " -n 4 " + host_);
}

int Pinger::getValue(QString& resultString, const QString& valueName,
                     int fromIndex, int& endIndex) const
{
    int startIndex{resultString.indexOf(valueName, fromIndex)};
    int length{valueName.length()};
    endIndex = resultString.indexOf(QRegExp(QStringLiteral("\\D")),
                                    startIndex + length);
    return resultString
        .midRef(startIndex + length, endIndex - startIndex - length)
        .toInt();
}

PingData Pinger::extractPingData(QString pingOutput) const
{
    int fromIndex{0};
    int endIndex{0};
    const QString equalString{QStringLiteral(" = ")};
    int packetsSent{getValue(pingOutput, equalString, fromIndex, endIndex)};

    fromIndex = endIndex;
    getValue(pingOutput, equalString, fromIndex, endIndex);
    fromIndex = endIndex;
    int packetsLost{getValue(pingOutput, equalString, fromIndex, endIndex)};
    fromIndex = endIndex;
    int min{getValue(pingOutput, equalString, fromIndex, endIndex)};
    fromIndex = endIndex;
    int max{getValue(pingOutput, equalString, fromIndex, endIndex)};
    fromIndex = endIndex;
    int avgReturnTime{getValue(pingOutput, equalString, fromIndex, endIndex)};
    QDateTime time{QDateTime::currentDateTime()};

    return {time, packetsSent, packetsLost, avgReturnTime, min, max};
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

void Pinger::pingFinished([[maybe_unused]] int exitCode,
                          [[maybe_unused]] QProcess::ExitStatus exitStatus)
{
    auto ping{qobject_cast<QProcess*>(sender())};
    if (ping == nullptr)
    {
        logError("Error: internal.");
        return;
    }

    // Read output and use it for info extraction.
    QString result{QString::fromLatin1(ping->readAllStandardOutput())};
    PingData pingData{extractPingData(result)};
    if (pingData.packetsSent == 0)
    {
        logError("Error: wrong return results.");
        return;
    }

    // If host not found/host down.
    if (pingData.packetsSent == pingData.packetsLost)
    {
        pingData.avgReturnTime = timeout_;
        pingData.min = timeout_;
        pingData.max = timeout_;
    }

    logPingData(pingData);

    Q_EMIT newPingData(pingData);
}
