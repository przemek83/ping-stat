#include <algorithm>
#include <cmath>

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

void PlotWidget::setupPainter(QPainter& painter)
{
    painter.setRenderHint(QPainter::Antialiasing);
    QColor color(Qt::darkBlue);
    QBrush brush{QBrush(color)};
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

void PlotWidget::drawItems(QPainter& painter)
{
    const int itemWidth{getPlotItemWidth()};
    const int startY{height() - marginSize_};

    int counter{0};
    for (const auto [time, value] : data_)
    {
        const int startX{marginSize_ + itemWidth * counter++};
        const float sizeFactor{static_cast<float>(value) /
                               static_cast<float>(timeoutValue_)};
        const int itemHeight{
            static_cast<int>(round(sizeFactor * getPlotAreaSize().height()))};
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
    if (event->type() == QEvent::ToolTip)
    {
        auto helpEvent{dynamic_cast<QHelpEvent*>(event)};

        const int plotItemWidth{getPlotItemWidth()};
        int item{(helpEvent->pos().x() - 2 * marginSize_) / plotItemWidth};

        if (item <= data_.size() && item >= 0)
        {
            QString tooltip(tr("Average return time: "));
            tooltip.append(QString::number(data_[item].second));
            tooltip.append(QStringLiteral("\n"));
            tooltip.append(tr("Time: "));
            tooltip.append(
                data_[item].first.toString(Constants::getDisplayTimeFormat()));

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
