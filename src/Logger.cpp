#include <QDate>
#include <QTextStream>

#include "Constants.h"
#include "Logger.h"

Logger::Logger() : logDate_(QDate::currentDate())
{
    logFile_.setFileName(logPrefix_ + logDate_.toString(logDateFormat_));
}

Logger::~Logger() { logFile_.close(); }

bool Logger::logFileReady(QDate currentDate)
{
    // Change log file if new day.
    if (currentDate != logDate_)
    {
        logFile_.close();
        logFile_.setFileName(logPrefix_ + logDate_.toString(logDateFormat_));
        logDate_ = currentDate;
    }

    // Open if needed.
    if (!logFile_.isOpen())
        return logFile_.open(QIODevice::WriteOnly | QIODevice::Append);

    return true;
}

Logger* Logger::getInstance()
{
    static auto instance{new Logger()};
    return instance;
}

void Logger::log(const QDateTime& time, const QString& msg)
{
    if (logFileReady(time.date()))
    {
        static QTextStream outStream(&logFile_);
        outStream << msg;
    }
}
