#include "PingerWindows.h"

#include <QRegularExpression>

PingerWindows::PingerWindows(QObject* parent) : Pinger(parent) {}

QString PingerWindows::getPingCommand() const
{
    return QStringLiteral("ping.exe -w ") + QString::number(getTimeout()) +
           " -n 4 " + getHost();
}

PingData PingerWindows::extractPingData(QString pingOutput) const
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
    const int min{getValue(pingOutput, equalString, fromIndex, endIndex)};
    fromIndex = endIndex;
    const int max{getValue(pingOutput, equalString, fromIndex, endIndex)};
    fromIndex = endIndex;
    const int avgReturnTime{
        getValue(pingOutput, equalString, fromIndex, endIndex)};
    const QDateTime time{QDateTime::currentDateTime()};

    return {time, packetsSent, packetsLost, avgReturnTime, min, max};
}

int PingerWindows::getValue(QString& resultString, const QString& valueName,
                            int fromIndex, int& endIndex) const
{
    qsizetype startIndex{resultString.indexOf(valueName, fromIndex)};
    qsizetype length{valueName.length()};
    endIndex = resultString.indexOf(QRegularExpression(QStringLiteral("\\D")),
                                    startIndex + length);
    return resultString.mid(startIndex + length, endIndex - startIndex - length)
        .toInt();
}
