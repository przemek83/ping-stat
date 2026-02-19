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

    data.packetsSent_ = values[0].toInt();
    const int packetsReceived{values[1].toInt()};
    data.packetsLost_ = data.packetsSent_ - packetsReceived;

    int const expectedValuesCount{5};
    if (values.size() < expectedValuesCount)
        return data;

    const int minIndex{2};
    const int maxIndex{3};
    const int avgIndex{4};
    data.min_ = values[minIndex].toInt();
    data.max_ = values[maxIndex].toInt();
    data.avgReturnTime_ = values[avgIndex].toInt();

    return data;
}
};  // namespace parser
