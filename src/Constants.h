#pragma once

#include <QObject>

/**
 * @brief The Constants class with values defined for whole app.
 */
class Constants
{
public:
    static const char* dateFormat();

    static const char* logPrefix();

    static const char* logTimeFormat();

    static const char* displayTimeFormat();

    static int maxTimeout();

    static int maxPlotItems();

    static int minPlotItemsToResize();

private:
    Constants();
    virtual ~Constants();
    Q_DISABLE_COPY(Constants)

    /// Format of date for log file name.
    static const char* logDateFormat_;

    /// Prefix for log files name.
    static const char* logPrefix_;

    /// Time format used in log files.
    static const char* logTimeFormat_;

    /// Format of time used in display.
    static const char* displayTimeFormat_;

    /// Max timeout which can be set.
    static const int maxTimeout_;

    /// Max items in plot.
    static const int maxPlotItems_;

    /// Used for unfreezing plot (when number hit).
    static const int minPlotItemsToResize_;
};
