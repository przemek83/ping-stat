#pragma once

#include <QDateTime>
#include <QWidget>

class Painter;

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
    void setupPainter(QPainter& painter);

    void drawScales(QPainter& painter);

    void drawItems(QPainter& painter);

    int getPlotItemWidth() const;

    QSize getPlotAreaSize() const;

    QVector<int> avgReturnTimes_;

    /// Container with time values.
    QVector<QDateTime> timeData_;

    int timeoutValue_{0};

    const int marginSize_{10};
    const int axisNameSize_{50};
    const int axisArrowSize_{5};
    const int maxPlotItems_{50};
    const int minPlotItemsToResize_{10};
};
