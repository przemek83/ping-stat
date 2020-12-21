#pragma once

#include <QDateTime>
#include <QWidget>

class PlotWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlotWidget(QWidget* parent = 0);

    virtual ~PlotWidget();

protected:
    /**
     * @brief overridden paint method for drawing.
     * @param event event
     */
    virtual void paintEvent(QPaintEvent* event);

    /**
     * @brief overridden event method to use custom tooltip.
     * @param event event
     * @return true if event recognized and accepted.
     */
    virtual bool event(QEvent* event);

private:
    Q_DISABLE_COPY(PlotWidget)

    /**
     * @brief draw scales for plot.
     * @param painter painter.
     */
    void drawScales(QPainter& painter);

    /**
     * @brief get minimum value from average return type.
     * @return minimum value.
     */
    int getMin() const;

    /// Container with average values.
    QVector<int> data_;

    /// Container with time values.
    QVector<QDateTime> timeData_;

    /// Margin size used for scale size and spacing.
    const static int marginSize_;

    /// Size used for axis names.
    const static int axisNameSize_;

    /// Size used for axis arrow.
    const static int axisArrowSize_;

    /// Current timeout value.
    int timeoutValue_;

    /**
     * @brief Calculate item width.
     * @return item width.
     */
    int getItemWidth();

private slots:
    /**
     * @brief add new value for plotting.
     * @param value new average value.
     * @param time new average value.
     */
    void updatePlotWidget(int value, QDateTime time);

    /**
     * @brief update timeout.
     * @param new timeout.
     */
    void configUpdated(int newTimeoutValue);
};
