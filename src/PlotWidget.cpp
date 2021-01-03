#include "PlotWidget.h"

#include <cmath>

#include <QHelpEvent>
#include <QPainter>
#include <QToolTip>

#include "Constants.h"

PlotWidget::PlotWidget(QWidget* parent) : QWidget(parent)
{
    setMinimumHeight(100);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void PlotWidget::setupPainter(QPainter& painter) const
{
    painter.setRenderHint(QPainter::Antialiasing);
    const QColor color(Qt::darkBlue);
    const QBrush brush{QBrush(color)};
    QPen pen;
    pen.setColor(color);
    pen.setWidth(1);
    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);
    painter.setBrush(brush);
}

void PlotWidget::paintEvent([[maybe_unused]] QPaintEvent* event)
{
    const int dataSize{data_.size()};
    if (dataSize == 0)
        return;

    QPainter painter(this);
    setupPainter(painter);

    drawItems(painter);
    drawScales(painter);
}

void PlotWidget::drawScales(QPainter& painter) const
{
    const int xFrom{marginSize_};
    const int yFrom{height() - marginSize_};
    const int xTo{width() - marginSize_};
    const int yTo{marginSize_};
    const int arrowSize{marginSize_ / 2};

    doublePenSize(painter);

    // Draw X axis.
    painter.drawLine(xFrom, yFrom, xTo, yFrom);
    painter.drawLine(xTo, yFrom, xTo - arrowSize, yFrom - arrowSize);
    painter.drawLine(xTo, yFrom, xTo - arrowSize, yFrom + arrowSize);

    // Draw Y axis.
    painter.drawLine(xFrom, yFrom, xFrom, yTo);
    painter.drawLine(xFrom, yTo, xFrom - arrowSize, yTo + arrowSize);
    painter.drawLine(xFrom, yTo, xFrom + arrowSize, yTo + arrowSize);

    // place "0" and axis names.
    painter.drawText(0, height(), tr("0"));
    painter.drawText(width() - axisNameSize_, height(), tr("time"));
    painter.rotate(270);
    painter.drawText(-axisNameSize_, marginSize_ - 2, tr("ping"));
}

void PlotWidget::drawItems(QPainter& painter) const
{
    const int itemWidth{getPlotItemWidth()};
    const int startY{height() - marginSize_};

    int counter{0};
    for (const auto& [time, value] : data_)
    {
        const int startX{marginSize_ + itemWidth * counter++};
        const float sizeFactor{static_cast<float>(value) /
                               static_cast<float>(getMaxYAxisValue())};
        const int itemHeight{static_cast<int>(std::round(
            sizeFactor * static_cast<float>(getPlotAreaSize().height())))};
        painter.drawRect(startX, startY, itemWidth, -itemHeight);
    }
}

int PlotWidget::getPlotItemWidth() const
{
    const int dataSize{data_.size()};
    const int plotAreaWidth{getPlotAreaSize().width()};
    return (dataSize > minPlotItemsToResize_
                ? (plotAreaWidth) / dataSize
                : (plotAreaWidth) / minPlotItemsToResize_);
}

QSize PlotWidget::getPlotAreaSize() const
{
    // One margin size for scale, one for distance between first plotItem
    // and scale, one for distance from last item to end.
    return {width() - 3 * marginSize_, height() - 3 * marginSize_};
}

void PlotWidget::doublePenSize(QPainter& painter) const
{
    QPen pen{painter.pen()};
    pen.setWidth(pen.width() * 2);
    painter.setPen(pen);
}

QString PlotWidget::buildToolTip(int item) const
{
    QString tooltip(tr("Average return time: "));
    tooltip.append(QString::number(data_[item].second));
    tooltip.append(QStringLiteral("\n"));
    tooltip.append(tr("Time: "));
    tooltip.append(
        data_[item].first.toString(Constants::getDisplayTimeFormat()));
    return tooltip;
}

int PlotWidget::getMaxYAxisValue() const
{
    auto comparator{[](const std::pair<QDateTime, int>& left,
                       const std::pair<QDateTime, int>& right) {
        return left.second < right.second;
    }};
    const std::pair<QDateTime, int> maxDataValue{
        *std::max_element(data_.constBegin(), data_.constEnd(), comparator)};
    return std::max(timeoutValue_, maxDataValue.second);
}

void PlotWidget::updatePlotWidget(int avgReturnTime, const QDateTime& time)
{
    if (data_.size() >= maxPlotItems_)
        data_.pop_front();
    data_.push_back({time, avgReturnTime});
    update();
}

void PlotWidget::setTimeoutValue(int timeoutValue)
{
    timeoutValue_ = timeoutValue;
}

bool PlotWidget::event(QEvent* event)
{
    if (event->type() != QEvent::ToolTip)
        return QWidget::event(event);

    auto helpEvent{dynamic_cast<QHelpEvent*>(event)};
    const int item{(helpEvent->pos().x() - marginSize_) / getPlotItemWidth()};
    if (item < data_.size() && item >= 0)
    {
        QString tooltipText(buildToolTip(item));
        QToolTip::hideText();
        QToolTip::showText(helpEvent->globalPos(), tooltipText);
        event->accept();
    }
    else
    {
        QToolTip::hideText();
        event->ignore();
    }
    return true;
}
