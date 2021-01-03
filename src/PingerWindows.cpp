#include "PingerWindows.h"

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
    int packetsSent{getValue(pingOutput, equalString, fromIndex, endIndex)};

    fromIndex = endIndex;
    getValue(pingOutput, equalString, fromIndex, endIndex);
    fromIndex = endIndex;
    int packetsLost{getValue(pingOutput, equalString, fromIndex, endIndex)};
    fromIndex = endIndex;
    int min{getValue(pingOutput, equalString, fromIndex, endIndex)};
    fromIndex = endIndex;
    int max{getValue(pingOutput, equalString, fromIndex, endIndex)};
    fromIndex = endIndex;
    int avgReturnTime{getValue(pingOutput, equalString, fromIndex, endIndex)};
    QDateTime time{QDateTime::currentDateTime()};

    return {time, packetsSent, packetsLost, avgReturnTime, min, max};
}

int PingerWindows::getValue(QString& resultString, const QString& valueName,
                            int fromIndex, int& endIndex) const
{
    int startIndex{resultString.indexOf(valueName, fromIndex)};
    int length{valueName.length()};
    endIndex = resultString.indexOf(QRegExp(QStringLiteral("\\D")),
                                    startIndex + length);
    return resultString
        .midRef(startIndex + length, endIndex - startIndex - length)
        .toInt();
}
