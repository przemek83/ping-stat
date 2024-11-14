#include "PingerLinux.h"

#include "Parser.h"

QString PingerLinux::getPingCommand() const
{
    return QStringLiteral("ping -c 4 -q ") + getHost();
}

PingData PingerLinux::extractPingData(QString pingOutput) const
{
    return parser::getPingDataFromlinuxOutput(pingOutput);
}
