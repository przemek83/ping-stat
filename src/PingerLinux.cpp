#include "PingerLinux.h"

#include <cmath>

PingerLinux::PingerLinux(QObject* parent) : Pinger(parent) {}

QString PingerLinux::getPingCommand() const
{
    return QStringLiteral("ping -c 4 -q ") + getHost();
}

PingData PingerLinux::extractPingData(QString pingOutput) const
{
    const QStringList outputLines{
        pingOutput.split('\n', QString::SkipEmptyParts)};
    const int packetsLineIndex{
        outputLines.indexOf(QRegExp(".+packets transmitted.*"))};
    if (packetsLineIndex == -1)
        return {};

    const auto [packetsSent, packetsReceived] =
        getPacketsInfo(outputLines[packetsLineIndex]);
    const int timesIndex{outputLines.lastIndexOf(QRegExp(".+=.+"))};
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

std::pair<int, int> PingerLinux::getPacketsInfo(
    const QString& packetsLine) const
{
    int endIndex{packetsLine.indexOf(" ")};
    const int packetsSent{packetsLine.midRef(0, endIndex).toInt()};
    const QString startSubString{", "};
    const int startIndex{packetsLine.indexOf(startSubString)};
    endIndex =
        packetsLine.indexOf(" received", startIndex + startSubString.size());
    const int packetsReceived{
        packetsLine
            .midRef(startIndex + startSubString.size(),
                    endIndex - startIndex - startSubString.size())
            .toInt()};
    return {packetsSent, packetsReceived};
}

std::tuple<int, int, int> PingerLinux::getTimesInfo(
    const QString& timesLine) const
{
    const int startIndex{timesLine.indexOf(" = ")};
    const int endIndex{timesLine.lastIndexOf(" ms")};
    const QString timesString{
        timesLine.mid(startIndex + 3, endIndex - startIndex - 3)};
    const QStringList times{timesString.split('/')};
    if (times.size() < 3)
        return {0, 0, 0};

    return {std::round(times[0].toFloat()), std::round(times[1].toFloat()),
            std::round(times[2].toFloat())};
}
