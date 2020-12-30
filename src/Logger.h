#pragma once

#include <QDate>
#include <QFile>
#include <QObject>

/**
 * @brief The Logger class for logging ping results for given host.
 */
class Logger
{
public:
    Logger& operator=(const Logger& other) = delete;
    Logger(const Logger& other) = delete;
    Logger& operator=(Logger&& other) = delete;
    Logger(Logger&& other) = delete;

    static Logger& getInstance();

    /**
     * @brief log message in file.
     * @param time timestamp.
     * @param msg message to log.
     */
    void log(const QDateTime& time, const QString& msg);

private:
    Logger();
    ~Logger() = default;

    /**
     * @brief check if log file is current and open it if needed.
     * @param currentDate current date for log file.
     * @return true when file is opened.
     */
    bool logFileReady(QDate currentDate);

    void changeDay(const QDate& currentDate);

    /// Current log date.
    QDate logDate_;

    /// Log file.
    QFile logFile_;

    const QString logDateFormat_{QStringLiteral("yyyy-MM-dd")};

    const QString logPrefix_{QStringLiteral("log_")};
};
