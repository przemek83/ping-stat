#pragma once

#include <QDateTime>
#include <QObject>
#include <QProcess>

class QTimerEvent;

/**
 * @brief The HostChecker class used for pinging given host.
 */
class HostChecker : public QObject
{
    Q_OBJECT
public:
    explicit HostChecker(QObject* parent = nullptr);

    ~HostChecker() = default;

    /**
     * @brief start host checker using giving parameters.
     * @param intervalInSeconds time between calls of ping.
     * @param timeout maximum time ping wits for reply.
     * @param host IP of host.
     */
    void start(int intervalInSeconds, int timeout, const QString& host);

    /**
     * @brief stop host checker.
     */
    void stop();

    /**
     * @brief Check state of host checker.
     * @return true if host checker is running.
     */
    bool isRunning();

protected:
    /**
     * @brief called every interval by timer.
     * @param event event.
     */
    void timerEvent(QTimerEvent* event);

private:
    Q_DISABLE_COPY(HostChecker)

    /**
     * @brief Get value from ping results  string.
     * @param resultString output of ping.
     * @param valueName substring to find.
     * @param fromIndex search result from index.
     * @param endIndex remember last end index.
     * @return extracted value or 0 if not found.
     */
    int getValue(QString& resultString, const QString& valueName, int fromIndex,
                 int& endIndex);

    /// Host IP.
    QString host_;

    /// Timer id.
    int timerId_{0};

    /// Timeout value.
    int timeout_{0};

    const QString logTimeFormat_{QStringLiteral("HH:mm:ss")};

private Q_SLOTS:
    /**
     * @brief called when ping is finished. Uses output of ping.
     */
    void pingFinished(int, QProcess::ExitStatus exitStatus);

Q_SIGNALS:
    /**
     * @brief Signal used to update display with new values.
     */
    void updateStatDisplay(QDateTime, int, int, int, int, int);

    /**
     * @brief signal used to update plot using average value and time.
     */
    void updatePlotWidget(int, QDateTime);
};
