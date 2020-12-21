#include <QDate>
#include <QTextStream>

#include "Constants.h"
#include "Logger.h"

Logger::Logger() : logDate_(QDate::currentDate())
{
    logFile_.setFileName(Constants::logPrefix() +
                         logDate_.toString(Constants::dateFormat()));
}

Logger::~Logger() { logFile_.close(); }

bool Logger::logFileReady(QDate currentDate)
{
    // Change log file if new day.
    if (currentDate != logDate_)
    {
        logFile_.close();
        logFile_.setFileName(Constants::logPrefix() +
                             logDate_.toString(Constants::dateFormat()));
        logDate_ = currentDate;
    }

    // Open if needed.
    if (false == logFile_.isOpen())
    {
        return logFile_.open(QIODevice::WriteOnly | QIODevice::Append);
    }

    return true;
}

Logger* Logger::getInstance()
{
    static Logger* instance = new Logger();
    return instance;
}

void Logger::log(QDateTime time, QString msg)
{
    if (true == logFileReady(time.date()))
    {
        static QTextStream outStream(&logFile_);
        outStream << msg;
    }
}
