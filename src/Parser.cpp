#include "Parser.h"

#include <QRegularExpression>

namespace
{
std::pair<int, int> getLinuxPacketsInfo(const QString& line)
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

std::tuple<int, int, int> getTimesInfo(const QString& line)
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

QStringList getWindowsPacketsInfo(const QString& pingOutput)
{
    const QString equalString{QStringLiteral(" = ")};
    const QRegularExpression regex(R"( = (\d+)(ms,|,|ms\r))");
    QStringList values;
    QRegularExpressionMatchIterator it{regex.globalMatch(pingOutput)};
    while (it.hasNext())
    {
        QRegularExpressionMatch match = it.next();
        values.push_back(match.captured(1));
    }
    return values;
}
}  // namespace

namespace parser
{
PingData getPingDataFromlinuxOutput(QString pingOutput)
{
    const QStringList outputLines{pingOutput.split('\n', Qt::SkipEmptyParts)};
    const qsizetype packetsLineIndex{outputLines.indexOf(
        QRegularExpression(QStringLiteral(".+packets transmitted.*")))};
    if (packetsLineIndex == -1)
        return {};

    const auto [packetsSent, packetsReceived] =
        getLinuxPacketsInfo(outputLines[packetsLineIndex]);
    const qsizetype timesIndex{
        outputLines.lastIndexOf(QRegularExpression(QStringLiteral(".+=.+")))};
    if (timesIndex == -1)
        return {QDateTime::currentDateTime(), packetsSent,
                packetsSent - packetsReceived};

    const auto [min, avg, max] = getTimesInfo(outputLines[timesIndex]);
    return {QDateTime::currentDateTime(),
            packetsSent,
            packetsSent - packetsReceived,
            avg,
            min,
            max};
}

PingData getPingDataFromWindowsOutput(QString pingOutput)
{
    PingData data{QDateTime::currentDateTime()};

    QStringList values{getWindowsPacketsInfo(pingOutput)};
    if (values.size() < 2)
        return data;

    data.packetsSent = values[0].toInt();
    const int packetsReceived{values[1].toInt()};
    data.packetsLost = data.packetsSent - packetsReceived;

    if (values.size() < 5)
        return data;

    data.min = values[2].toInt();
    data.max = values[3].toInt();
    data.avgReturnTime = values[4].toInt();

    return data;
}
};  // namespace parser