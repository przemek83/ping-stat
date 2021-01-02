#include <algorithm>

#include <QApplication>
#include <QDebug>
#include <QHelpEvent>
#include <QPainter>
#include <QTimer>
#include <QToolTip>

#include "Constants.h"
#include "PlotWidget.h"

PlotWidget::PlotWidget(QWidget* parent) : QWidget(parent)
{
    setMinimumHeight(100);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void PlotWidget::paintEvent([[maybe_unused]] QPaintEvent* event)
{
    const int dataSize{avgReturnTimes_.size()};
    if (dataSize == 0)
        return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    static bool initialized = false;

    // Set color, pen and brush style.
    QColor color(Qt::darkBlue);
    QBrush brush{QBrush(color)};
    QPen pen;
    if (!initialized)
    {
        pen.setColor(color);
        pen.setWidth(1);
        pen.setStyle(Qt::SolidLine);
        initialized = true;
    }

    painter.setPen(pen);
    painter.setBrush(brush);

    // Draw scales.
    drawScales(painter);

    const int plotAreaHeight{getPlotAreaSize().height()};

    int plotItemWidth{getPlotItemWidth()};

    // Calculate x and y where items should start.
    int itemStartX = 2 * marginSize_;
    int itemStartY = marginSize_ + plotAreaHeight;

    // Set items spacing.
    int itemSpacing = marginSize_ * 2;

    const int minAvgReturnTime{getMinAvgReturnTime()};

    // Draw items.
    for (int i = 0; i < dataSize; ++i)
    {
        int x{itemStartX + plotItemWidth * i};
        int width{plotItemWidth - itemSpacing / 2};
        int itemHeight{plotAreaHeight};
        float factor{
            1 - static_cast<float>(avgReturnTimes_.at(i) - minAvgReturnTime) /
                    static_cast<float>(timeoutValue_ - minAvgReturnTime)};
        int height{static_cast<int>(static_cast<float>(itemHeight) * factor)};
        painter.drawRect(x, itemStartY, width, -height);
    }
}

void PlotWidget::drawScales(QPainter& painter)
{
    // int dataSize = data_.size();
    int xFrom = marginSize_;
    int yFrom = height() - marginSize_;
    int xTo = width() - marginSize_;
    int yTo = marginSize_;

    // Draw X axis.
    painter.drawLine(xFrom, yFrom, xTo, yFrom);
    painter.drawLine(xTo, yFrom, xTo - axisArrowSize_, yFrom - axisArrowSize_);
    painter.drawLine(xTo, yFrom, xTo - axisArrowSize_, yFrom + axisArrowSize_);

    // Draw Y axis.
    painter.drawLine(xFrom, yFrom, xFrom, yTo);
    painter.drawLine(xFrom, yTo, xFrom - axisArrowSize_, yTo + axisArrowSize_);
    painter.drawLine(xFrom, yTo, xFrom + axisArrowSize_, yTo + axisArrowSize_);

    // Name "0" and axis.
    painter.drawText(0, height(), tr("0"));
    painter.drawText(width() - axisNameSize_, height(), tr("time"));
    painter.save();
    painter.rotate(270);  // or 270
    painter.drawText(-axisNameSize_, marginSize_ - 2, tr("ping"));
    painter.restore();
}

int PlotWidget::getMinAvgReturnTime() const
{
    if (avgReturnTimes_.isEmpty())
        return 0;
    return *std::min_element(avgReturnTimes_.constBegin(),
                             avgReturnTimes_.constEnd());
}

int PlotWidget::getPlotItemWidth() const
{
    const int dataSize{avgReturnTimes_.size()};
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

void PlotWidget::updatePlotWidget(int avgReturnTime, const QDateTime& time)
{
    if (avgReturnTimes_.size() >= maxPlotItems_)
    {
        avgReturnTimes_.pop_front();
        timeData_.pop_front();
    }

    avgReturnTimes_.push_back(avgReturnTime);
    timeData_.push_back(time);

    update();
}

void PlotWidget::setTimeoutValue(int timeoutValue)
{
    timeoutValue_ = timeoutValue;
}

bool PlotWidget::event(QEvent* event)
{
    if (event->type() == QEvent::ToolTip)
    {
        auto helpEvent{dynamic_cast<QHelpEvent*>(event)};

        const int plotItemWidth{getPlotItemWidth()};
        int item{(helpEvent->pos().x() - 2 * marginSize_) / plotItemWidth};

        if (item <= avgReturnTimes_.size() && item >= 0)
        {
            QString tooltip(tr("Average return time: "));
            tooltip.append(QString::number(avgReturnTimes_[item]));
            tooltip.append(QStringLiteral("\n"));
            tooltip.append(tr("Time: "));
            tooltip.append(
                timeData_.at(item).toString(Constants::getDisplayTimeFormat()));

            QToolTip::showText(helpEvent->globalPos(), tooltip);
        }
        else
        {
            QToolTip::hideText();
            event->ignore();
        }

        return true;
    }
    return QWidget::event(event);
}
