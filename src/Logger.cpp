#include <QDate>
#include <QTextStream>

#include "Constants.h"
#include "Logger.h"

Logger::Logger() : logDate_(QDate::currentDate())
{
    logFile_.setFileName(logPrefix_ + logDate_.toString(logDateFormat_));
}

bool Logger::logFileReady(QDate currentDate)
{
    if (currentDate != logDate_)
        changeDay(currentDate);

    if (!logFile_.isOpen())
        return logFile_.open(QIODevice::WriteOnly | QIODevice::Append);

    return true;
}

void Logger::changeDay(const QDate& currentDate)
{
    logFile_.close();
    logFile_.setFileName(logPrefix_ + logDate_.toString(logDateFormat_));
    logDate_ = currentDate;
}

Logger& Logger::getInstance()
{
    static Logger instance;
    return instance;
}

void Logger::log(const QDateTime& time, const QString& msg)
{
    if (logFileReady(time.date()))
    {
        QTextStream outStream(&logFile_);
        outStream << msg;
    }
}
