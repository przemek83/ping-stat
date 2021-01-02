#pragma once

#include <QDateTime>
#include <QWidget>

class PlotWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlotWidget(QWidget* parent = nullptr);

    ~PlotWidget() = default;

    void setTimeoutValue(int timeoutValue);

    void updatePlotWidget(int avgReturnTime, const QDateTime& time);

protected:
    void paintEvent(QPaintEvent* event) override;

    bool event(QEvent* event) override;

private:
    void drawScales(QPainter& painter);

    int getMinAvgReturnTime() const;

    int getPlotItemWidth() const;

    QSize getPlotAreaSize() const;

    QVector<int> avgReturnTimes_;

    /// Container with time values.
    QVector<QDateTime> timeData_;

    /// Margin size used for scale size and spacing.
    const int marginSize_{10};

    const int axisNameSize_{50};

    const int axisArrowSize_{5};

    const int maxPlotItems_{50};

    const int minPlotItemsToResize_{10};

    int timeoutValue_{0};
};
