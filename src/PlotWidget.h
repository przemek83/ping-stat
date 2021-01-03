#pragma once

#include <QDateTime>
#include <QWidget>

class Painter;

class PlotWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlotWidget(QWidget* parent = nullptr);

    ~PlotWidget() override = default;

    void setTimeoutValue(int timeoutValue);

    void updatePlotWidget(int avgReturnTime, const QDateTime& time);

protected:
    void paintEvent(QPaintEvent* event) override;

    bool event(QEvent* event) override;

private:
    void setupPainter(QPainter& painter) const;

    void drawScales(QPainter& painter) const;

    void drawItems(QPainter& painter) const;

    int getPlotItemWidth() const;

    QSize getPlotAreaSize() const;

    void doublePenSize(QPainter& painter) const;

    QString buildToolTip(int item) const;

    int getMaxYAxisValue() const;

    QVector<std::pair<QDateTime, int>> data_;

    int timeoutValue_{0};

    const int marginSize_{10};
    const int axisNameSize_{50};
    const int maxPlotItems_{50};
    const int minPlotItemsToResize_{10};
};
