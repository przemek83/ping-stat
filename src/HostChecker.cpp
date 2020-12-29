#include <QDateTime>
#include <QDebug>

#include "Constants.h"
#include "HostChecker.h"
#include "Logger.h"

HostChecker::HostChecker(QObject* parent) : QObject(parent) {}

void HostChecker::start(int interval, int timeout, const QString& host)
{
    if (isRunning())
        stop();
    host_ = host;
    timeout_ = timeout;
    timerId_ = startTimer(interval);
}

void HostChecker::stop()
{
    killTimer(timerId_);
    timerId_ = 0;
}

bool HostChecker::isRunning() { return timerId_ != 0; }

void HostChecker::timerEvent([[maybe_unused]] QTimerEvent* event)
{
    if (!isRunning())
        return;

    auto pingProcess{new QProcess(this)};
    connect(pingProcess,
            static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(
                &QProcess::finished),
            this, &HostChecker::pingFinished);

    pingProcess->start(QStringLiteral("ping.exe -w ") +
                       QString::number(timeout_) + " -n 4 " + host_);
}

int HostChecker::getValue(QString& resultString, const QString& valueName,
                          int fromIndex, int& endIndex)
{
    int startIndex{resultString.indexOf(valueName, fromIndex)};
    int length{valueName.length()};
    endIndex = resultString.indexOf(QRegExp(QStringLiteral("\\D")),
                                    startIndex + length);
    return resultString
        .midRef(startIndex + length, endIndex - startIndex - length)
        .toInt();
}

void HostChecker::pingFinished([[maybe_unused]] int exitCode,
                               [[maybe_unused]] QProcess::ExitStatus exitStatus)
{
    auto ping{qobject_cast<QProcess*>(sender())};
    QDateTime time{QDateTime::currentDateTime()};
    QString logMsg;
    QTextStream out(&logMsg);
    out << time.toString(logTimeFormat_) << ",";

    if (ping == nullptr)
    {
        out << tr("Error: internal.") << "\n";
        Logger::getInstance()->log(time, logMsg);
        return;
    }

    // Read output and use it for info extraction.
    QString result{QString::fromLatin1(ping->readAllStandardOutput())};
    int fromIndex{0};
    int endIndex{0};
    const QString equalString{QStringLiteral(" = ")};
    int packetsSent{getValue(result, equalString, fromIndex, endIndex)};

    if (packetsSent == 0)
    {
        out << tr("Error: wrong return results.") << "\n";
        Logger::getInstance()->log(time, logMsg);
        return;
    }
    fromIndex = endIndex;
    getValue(result, equalString, fromIndex, endIndex);
    fromIndex = endIndex;
    int packetsLost{getValue(result, equalString, fromIndex, endIndex)};
    fromIndex = endIndex;
    int min{getValue(result, equalString, fromIndex, endIndex)};
    fromIndex = endIndex;
    int max{getValue(result, equalString, fromIndex, endIndex)};
    fromIndex = endIndex;
    int avgReturnTime{getValue(result, equalString, fromIndex, endIndex)};

    // Log.
    out << host_ << "," << packetsSent << "," << packetsLost << "," << min
        << "," << max << "," << avgReturnTime << "\n";
    Logger::getInstance()->log(time, logMsg);

    // If host not found/host down.
    if (packetsSent == packetsLost)
    {
        avgReturnTime = timeout_;
        min = timeout_;
        max = timeout_;
    }

    // Update GUI.
    Q_EMIT updateStatDisplay(time, packetsSent, packetsLost, avgReturnTime, min,
                             max);
    Q_EMIT updatePlotWidget(avgReturnTime, time);
}
