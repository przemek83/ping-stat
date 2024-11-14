#include "Parser.h"

#include <QRegularExpression>

namespace
{
std::pair<int, int> getPacketsInfo(const QString& line)
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

/**
 * @brief Get value from ping results  string.
 * @param resultString output of ping.
 * @param valueName substring to find.
 * @param fromIndex search result from index.
 * @param endIndex remember last end index.
 * @return extracted value or 0 if not found.
 */
int getValue(QString& resultString, const QString& valueName, int fromIndex,
             int& endIndex)
{
    qsizetype startIndex{resultString.indexOf(valueName, fromIndex)};
    qsizetype length{valueName.length()};
    endIndex = resultString.indexOf(QRegularExpression(QStringLiteral("\\D")),
                                    startIndex + length);
    return resultString.mid(startIndex + length, endIndex - startIndex - length)
        .toInt();
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

PingData getPingDataFromWindowsOutput(QString pingOutput)
{
    int fromIndex{0};
    int endIndex{0};
    const QString equalString{QStringLiteral(" = ")};
    const int packetsSent{
        getValue(pingOutput, equalString, fromIndex, endIndex)};

    fromIndex = endIndex;
    getValue(pingOutput, equalString, fromIndex, endIndex);
    fromIndex = endIndex;
    const int packetsLost{
        getValue(pingOutput, equalString, fromIndex, endIndex)};
    fromIndex = endIndex;

    int min{0};
    int max{0};
    int avgReturnTime{0};

    if(pingOutput.count(equalString) > 3)
    {
        min = getValue(pingOutput, equalString, fromIndex, endIndex);
        fromIndex = endIndex;
        max = getValue(pingOutput, equalString, fromIndex, endIndex);
        fromIndex = endIndex;
        avgReturnTime = getValue(pingOutput, equalString, fromIndex, endIndex);
    }

    const QDateTime time{QDateTime::currentDateTime()};

    return {time, packetsSent, packetsLost, avgReturnTime, min, max};
}
};  // namespace parser