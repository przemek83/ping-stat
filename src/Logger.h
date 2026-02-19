#pragma once

#include <QDate>
#include <QFile>

class Logger
{
public:
    Logger(const Logger& other) = delete;
    Logger(Logger&& other) = delete;

    Logger& operator=(const Logger& other) = delete;
    Logger& operator=(Logger&& other) = delete;

    static Logger& getInstance();

    void info(const QString& msg);

private:
    Logger();
    ~Logger() = default;

    bool isLogFileReady();

    QString getLogFileName() const;

    void switchLogFileToNewDay();

    bool isNewDay() const;

    QDate currentLogDate_{QDate::currentDate()};

    QFile logFile_;
};
