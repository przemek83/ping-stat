#include "PingerLinux.h"

#include <cmath>

PingerLinux::PingerLinux(QObject* parent) : Pinger(parent) {}

QString PingerLinux::getPingCommand() const
{
    return QStringLiteral("ping -c 4 -q ") + getHost();
}

PingData PingerLinux::extractPingData(QString pingOutput) const
{
    QStringList outputLines{pingOutput.split('\n', QString::SkipEmptyParts)};
    int packetsLineIndex{
        outputLines.indexOf(QRegExp(".+packets transmitted.*"))};
    if (packetsLineIndex == -1)
        return {};

    auto [packetsSent, packetsReceived] =
        getPacketsInfo(outputLines[packetsLineIndex]);
    int timesIndex{outputLines.lastIndexOf(QRegExp(".+=.+"))};
    if (timesIndex == -1)
        return {QDateTime::currentDateTime(),
                packetsSent,
                packetsSent - packetsReceived,
                0,
                0,
                0};

    auto [min, avg, max] = getTimesInfo(outputLines[timesIndex]);
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
    int packetsSent{packetsLine.midRef(0, endIndex).toInt()};
    int startIndex{packetsLine.indexOf(", ")};
    endIndex = packetsLine.indexOf(" received", startIndex + 2);
    int packetsReceived{
        packetsLine.midRef(startIndex + 2, endIndex - startIndex - 2).toInt()};
    return {packetsSent, packetsReceived};
}

std::tuple<int, int, int> PingerLinux::getTimesInfo(
    const QString& timesLine) const
{
    int startIndex{timesLine.indexOf(" = ")};
    int endIndex{timesLine.lastIndexOf(" ms")};
    QString timesString{
        timesLine.mid(startIndex + 3, endIndex - startIndex - 3)};
    QStringList times{timesString.split('/')};
    if (times.size() < 3)
        return {0, 0, 0};

    return {std::round(times[0].toFloat()), std::round(times[1].toFloat()),
            std::round(times[2].toFloat())};
}
