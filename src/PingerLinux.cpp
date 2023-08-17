#include "PingerLinux.h"

#include <cmath>

#include <QRegularExpression>

PingerLinux::PingerLinux(QObject* parent) : Pinger(parent) {}

QString PingerLinux::getPingCommand() const
{
    return QStringLiteral("ping -c 4 -q ") + getHost();
}

PingData PingerLinux::extractPingData(QString pingOutput) const
{
    const QStringList outputLines{pingOutput.split('\n', Qt::SkipEmptyParts)};
    const qsizetype packetsLineIndex{outputLines.indexOf(
        QRegularExpression(QStringLiteral(".+packets transmitted.*")))};
    if (packetsLineIndex == -1)
        return {};

    const auto [packetsSent, packetsReceived] =
        getPacketsInfo(outputLines[packetsLineIndex]);
    const qsizetype timesIndex{
        outputLines.lastIndexOf(QRegularExpression(QStringLiteral(".+=.+")))};
    if (timesIndex == -1)
        return {QDateTime::currentDateTime(),
                packetsSent,
                packetsSent - packetsReceived,
                0,
                0,
                0};

    const auto [min, avg, max] = getTimesInfo(outputLines[timesIndex]);
    return {QDateTime::currentDateTime(),
            packetsSent,
            packetsSent - packetsReceived,
            avg,
            min,
            max};
}

std::pair<int, int> PingerLinux::getPacketsInfo(const QString& line) const
{
    qsizetype endIndex{line.indexOf(QStringLiteral(" "))};
    const int packetsSent{line.mid(0, endIndex).toInt()};
    const QString startSubString{QStringLiteral(", ")};
    const qsizetype startIndex{line.indexOf(startSubString)};
    endIndex = line.indexOf(QStringLiteral(" received"),
                            startIndex + startSubString.size());
    const int packetsReceived{
        line.mid(startIndex + startSubString.size(),
                 endIndex - startIndex - startSubString.size())
            .toInt()};
    return {packetsSent, packetsReceived};
}

std::tuple<int, int, int> PingerLinux::getTimesInfo(const QString& line) const
{
    const QString startSubString{QStringLiteral(" = ")};
    const qsizetype startIndex{line.indexOf(startSubString)};
    const qsizetype endIndex{line.lastIndexOf(QStringLiteral(" ms"))};
    const QString timesString{
        line.mid(startIndex + startSubString.size(),
                 endIndex - startIndex - startSubString.size())};
    const QStringList times{timesString.split('/')};
    if (times.size() < 3)
        return {0, 0, 0};

    return {std::round(times[0].toFloat()), std::round(times[1].toFloat()),
            std::round(times[2].toFloat())};
}
