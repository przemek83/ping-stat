#pragma once

#include <QDate>
#include <QFile>

/**
 * @brief The Logger class for logging ping results.
 */
class Logger
{
public:
    Logger& operator=(const Logger& other) = delete;
    Logger(const Logger& other) = delete;
    Logger& operator=(Logger&& other) = delete;
    Logger(Logger&& other) = delete;

    static Logger& getInstance();

    void log(const QString& msg);

private:
    Logger();
    ~Logger() = default;

    bool isLogFileReady();

    QString getLogFileName() const;

    void switchLogFileToNewDay();

    bool isNewDay() const;

    QDate currentLogDate_;

    QFile logFile_;
};
