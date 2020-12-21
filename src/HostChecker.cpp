#include <QDateTime>
#include <QDebug>

#include "Constants.h"
#include "HostChecker.h"
#include "Logger.h"

HostChecker::HostChecker(QObject* parent)
    : QObject(parent), timerId_(0), host_(""), timeout_(0)
{
}

HostChecker::~HostChecker() {}

void HostChecker::start(int interval, int timeout, QString host)
{
    // Stop if already running.
    if (true == isRunning())
    {
        stop();
    }
    host_ = host;
    timeout_ = timeout;

    // Start timer with given interval.
    timerId_ = startTimer(interval);
}

void HostChecker::stop()
{
    killTimer(timerId_);
    timerId_ = 0;
}

bool HostChecker::isRunning() { return (0 != timerId_); }

void HostChecker::timerEvent(QTimerEvent* /*event*/)
{
    if (false == isRunning())
    {
        return;
    }

    QProcess* pingProcess = new QProcess(this);
    connect(pingProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this,
            SLOT(pingFinished(int, QProcess::ExitStatus)));

    pingProcess->start(QString("ping.exe -w ") + QString::number(timeout_) +
                       " " + host_);
}

int HostChecker::getValue(QString& resultString, QString valueName,
                          int fromIndex, int& endIndex)
{
    int startIndex = resultString.indexOf(valueName, fromIndex);
    int length = valueName.length();
    endIndex = resultString.indexOf(QRegExp("\\D"), startIndex + length);
    return resultString.mid(startIndex + length, endIndex - startIndex - length)
        .toInt();
}

void HostChecker::pingFinished(int, QProcess::ExitStatus)
{
    QProcess* ping = qobject_cast<QProcess*>(sender());
    QDateTime time = QDateTime::currentDateTime();
    QString logMsg;
    QTextStream out(&logMsg);
    out << time.toString(Constants::logTimeFormat()) << ",";

    if (NULL == ping)
    {
        out << tr("Error: internal.") << "\n";
        Logger::getInstance()->log(time, logMsg);
        return;
    }

    // Read output and use it for info extraction.
    QString result = ping->readAllStandardOutput();
    int fromIndex = 0;
    int endIndex = 0;
    int packetsSent = getValue(result, " = ", fromIndex, endIndex);

    if (0 == packetsSent)
    {
        out << tr("Error: wrong return results.") << "\n";
        Logger::getInstance()->log(time, logMsg);
        return;
    }
    fromIndex = endIndex;
    getValue(result, " = ", fromIndex, endIndex);
    fromIndex = endIndex;
    int packetsLost = getValue(result, " = ", fromIndex, endIndex);
    fromIndex = endIndex;
    int min = getValue(result, " = ", fromIndex, endIndex);
    fromIndex = endIndex;
    int max = getValue(result, " = ", fromIndex, endIndex);
    fromIndex = endIndex;
    int avgReturnTime = getValue(result, " = ", fromIndex, endIndex);

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

    // Udpate GUI.
    emit updateStatDisplay(time, packetsSent, packetsLost, avgReturnTime, min,
                           max);
    emit updatePlotWidget(avgReturnTime, time);
}
