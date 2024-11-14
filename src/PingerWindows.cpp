#include "PingerWindows.h"

#include "Parser.h"

QString PingerWindows::getPingCommand() const
{
    return QStringLiteral("ping.exe -w ") + QString::number(getTimeout()) +
           " -n 4 " + getHost();
}

PingData PingerWindows::extractPingData(QString pingOutput) const
{
    return parser::getPingDataFromWindowsOutput(pingOutput);
}
