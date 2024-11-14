#include "ParserTest.h"

#include <src/Parser.h>
#include <src/PingData.h>

void ParserTest::testPingableHostOnLinux() const
{
    QString pingOutput{
        "PING 212.77.98.9 (212.77.98.9) 56(84) bytes of data.\n\n--- "
        "212.77.98.9 ping statistics ---\n4 packets transmitted, 4 received, "
        "0% packet loss, time 3004ms\nrtt min/avg/max/mdev = "
        "17.234/19.302/22.909/2.153 ms\n"};

    PingData currentPingData{parser::getPingDataFromlinuxOutput(pingOutput)};
    QCOMPARE(currentPingData.packetsSent, 4);
    QCOMPARE(currentPingData.packetsLost, 0);
    QCOMPARE(currentPingData.avgReturnTime, 19);
    QCOMPARE(currentPingData.min, 17);
    QCOMPARE(currentPingData.max, 23);
}

void ParserTest::testNonPingableHostOnLinux() const
{
    QString pingOutput{
        "PING 212.77.98.1 (212.77.98.1) 56(84) bytes of data.\n\n--- "
        "212.77.98.1 ping statistics ---\n4 packets transmitted, 0 received, "
        "100% packet loss, time 3109ms\n\n"};

    PingData currentPingData{parser::getPingDataFromlinuxOutput(pingOutput)};
    QCOMPARE(currentPingData.packetsSent, 4);
    QCOMPARE(currentPingData.packetsLost, 4);
    QCOMPARE(currentPingData.avgReturnTime, 0);
    QCOMPARE(currentPingData.min, 0);
    QCOMPARE(currentPingData.max, 0);
}

void ParserTest::testNoNetworkOnLinux() const
{
    QString pingOutput{""};

    PingData currentPingData{parser::getPingDataFromlinuxOutput(pingOutput)};
    QCOMPARE(currentPingData.packetsSent, 0);
    QCOMPARE(currentPingData.packetsLost, 0);
    QCOMPARE(currentPingData.avgReturnTime, 0);
    QCOMPARE(currentPingData.min, 0);
    QCOMPARE(currentPingData.max, 0);
}

QTEST_MAIN(ParserTest)
