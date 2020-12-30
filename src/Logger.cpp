#include "Logger.h"

#include <QTextStream>

Logger::Logger() : currentLogDate_(QDate::currentDate())
{
    logFile_.setFileName(getLogFileName());
}

bool Logger::isLogFileReady()
{
    if (isNewDay())
        switchLogFileToNewDay();

    if (logFile_.isOpen())
        return true;

    return logFile_.open(QIODevice::WriteOnly | QIODevice::Append);
}

QString Logger::getLogFileName() const
{
    return QStringLiteral("log_") +
           currentLogDate_.toString(QStringLiteral("yyyy-MM-dd"));
}

void Logger::switchLogFileToNewDay()
{
    logFile_.close();
    logFile_.setFileName(getLogFileName());
    currentLogDate_ = QDate::currentDate();
}

bool Logger::isNewDay() const
{
    return QDate::currentDate() != currentLogDate_;
}

Logger& Logger::getInstance()
{
    static Logger instance;
    return instance;
}

void Logger::log(const QString& msg)
{
    if (!isLogFileReady())
        return;

    QTextStream outStream(&logFile_);
    outStream << msg;
}
