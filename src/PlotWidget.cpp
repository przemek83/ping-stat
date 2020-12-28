#include <QApplication>
#include <QDebug>
#include <QHelpEvent>
#include <QPainter>
#include <QTimer>
#include <QToolTip>

#include "Constants.h"
#include "PlotWidget.h"

const int PlotWidget::marginSize_ = 10;

const int PlotWidget::axisNameSize_ = 50;

const int PlotWidget::axisArrowSize_ = 5;

PlotWidget::PlotWidget(QWidget* parent) : QWidget(parent), timeoutValue_(0)
{
    setMinimumHeight(100);
}

void PlotWidget::paintEvent([[maybe_unused]] QPaintEvent* event)
{
    int dataSize{data_.size()};
    if (dataSize == 0)
        return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    static bool initialized = false;

    // Set color, pen and brush style.
    QColor color(127, 0, 127);
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

    // Used 3 * scale because need space for scale, margin from scale to plot
    // and
    int plotAreaHeight = height() - 3 * marginSize_;

    int itemWidth = getItemWidth();

    // Calculate x and y where items should start.
    int itemStartX = 2 * marginSize_;
    int itemStartY = marginSize_ + plotAreaHeight;

    // Set items spacing.
    int itemSpacing = marginSize_ * 2;

    int min = getMin();

    // Draw items.
    for (int i = 0; i < dataSize; ++i)
    {
        int x = itemStartX + itemWidth * i;
        int width = itemWidth - itemSpacing / 2;
        int itemHeight = plotAreaHeight;
        int height =
            itemHeight * (1 - (data_[i] - min) * 1.0 / (timeoutValue_ - min));
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

int PlotWidget::getMin() const
{
    int dataSize = data_.size();
    if (dataSize < 1)
    {
        return 0;
    }

    int currentMin = data_[0];
    for (int i = 1; i < dataSize; ++i)
    {
        if (data_[i] < currentMin)
        {
            currentMin = data_[i];
        }
    }

    return currentMin;
}

int PlotWidget::getItemWidth()
{
    int dataSize = data_.size();

    // Used 3 * scale because need space for scale, margin from scale to plot
    // and margin from plot to end.
    int plotAreaWidth = width() - 3 * marginSize_;

    return (dataSize > minPlotItemsToResize_
                ? (plotAreaWidth) / dataSize
                : (plotAreaWidth) / minPlotItemsToResize_);
}

void PlotWidget::updatePlotWidget(int value, const QDateTime& time)
{
    if (data_.size() >= maxPlotItems_)
    {
        data_.pop_front();
        timeData_.pop_front();
    }

    data_.push_back(value);
    timeData_.push_back(time);

    update();
}

void PlotWidget::configUpdated(int newTimeoutValue)
{
    timeoutValue_ = newTimeoutValue;
}

bool PlotWidget::event(QEvent* event)
{
    if (event->type() == QEvent::ToolTip)
    {
        QHelpEvent* helpEvent = static_cast<QHelpEvent*>(event);

        int itemWidth = getItemWidth();
        int item = (helpEvent->pos().x() - 2 * marginSize_) / itemWidth;

        if (item <= data_.size() && item >= 0)
        {
            QString tooltip(tr("Average return time: "));
            tooltip.append(QString::number(data_[item]));
            tooltip.append("\n");
            tooltip.append(tr("Time: "));
            tooltip.append(
                timeData_[item].toString(Constants::getDisplayTimeFormat()));

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
