#include "PingerLinux.h"

PingerLinux::PingerLinux(QObject* parent) : Pinger(parent) {}

QString PingerLinux::getPingCommand() const { return ""; }

PingData PingerLinux::extractPingData(QString pingOutput) const { return {}; }
