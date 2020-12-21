#ifndef LOGGER_H
#define LOGGER_H

#include <QDate>
#include <QFile>
#include <QObject>

/**
 * @brief The Logger class for logging ping results for given host.
 */
class Logger
{
public:
    /**
     * @brief get singleton of class.
     * @return singleton object of Logger.
     */
    static Logger* getInstance();

    /**
     * @brief log message in file.
     * @param time timestamp.
     * @param msg message to log.
     */
    void log(QDateTime time, QString msg);

private:
    Logger();
    virtual ~Logger();
    Q_DISABLE_COPY(Logger)

    /**
     * @brief check if log file is current and open it if needed.
     * @param currentDate current date for log file.
     * @return true when file is opened.
     */
    bool logFileReady(QDate currentDate);

    /// Current log date.
    QDate logDate_;

    /// Log file.
    QFile logFile_;
};

#endif  // LOGGER_H
