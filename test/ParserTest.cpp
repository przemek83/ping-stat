#include "ParserTest.h"

#include <src/Parser.h>
#include <src/PingData.h>

namespace{
    void check(PingData current, PingData expected)
    {
        QCOMPARE(current.packetsSent, expected.packetsSent);
        QCOMPARE(current.packetsLost, expected.packetsLost);
        QCOMPARE(current.avgReturnTime, expected.avgReturnTime);
        QCOMPARE(current.min, expected.min);
        QCOMPARE(current.max, expected.max);
    }
};

void ParserTest::testPingableHostOnLinux() const
{
    QString pingOutput{
        "PING 212.77.98.9 (212.77.98.9) 56(84) bytes of data.\n\n--- "
        "212.77.98.9 ping statistics ---\n4 packets transmitted, 4 received, "
        "0% packet loss, time 3004ms\nrtt min/avg/max/mdev = "
        "17.234/19.302/22.909/2.153 ms\n"};

    PingData currentPingData{parser::getPingDataFromlinuxOutput(pingOutput)};
    check(currentPingData, {QDateTime::currentDateTime(), 4, 0, 19, 17, 23});
}

void ParserTest::testNonPingableHostOnLinux() const
{
    QString pingOutput{
        "PING 212.77.98.1 (212.77.98.1) 56(84) bytes of data.\n\n--- "
        "212.77.98.1 ping statistics ---\n4 packets transmitted, 0 received, "
        "100% packet loss, time 3109ms\n\n"};

    PingData currentPingData{parser::getPingDataFromlinuxOutput(pingOutput)};
    check(currentPingData, {QDateTime::currentDateTime(), 4, 4, 0, 0, 0});
}

void ParserTest::testNoNetworkOnLinux() const
{
    QString pingOutput{""};

    PingData currentPingData{parser::getPingDataFromlinuxOutput(pingOutput)};
    check(currentPingData, {QDateTime::currentDateTime(), 0, 0, 0, 0, 0});
}

void ParserTest::testPacketsLostOnLinux() const
{
    QString pingOutput{
        "PING 212.77.98.9 (212.77.98.9) 56(84) bytes of data.\n\n--- "
        "212.77.98.9 ping statistics ---\n4 packets transmitted, 1 received, "
        "75% packet loss, time 3030ms\nrtt min/avg/max/mdev = "
        "19.708/19.708/19.708/0.000 ms\n"};

    PingData currentPingData{parser::getPingDataFromlinuxOutput(pingOutput)};
    check(currentPingData, {QDateTime::currentDateTime(), 4, 3, 20, 20, 20});
}

void ParserTest::testPingableHostOnWindows() const
{
    QString pingOutput{
        "\r\nPinging 212.77.98.9 with 32 bytes of data:\r\nReply from 212.77.98.9: bytes=32 time=17ms TTL=53\r\nReply from 212.77.98.9: bytes=32 time=25ms TTL=53\r\nReply from 212.77.98.9: bytes=32 time=30ms TTL=53\r\nReply from 212.77.98.9: bytes=32 time=18ms TTL=53\r\n\r\nPing statistics for 212.77.98.9:\r\n    Packets: Sent = 4, Received = 4, Lost = 0 (0% loss),\r\nApproximate round trip times in milli-seconds:\r\n    Minimum = 17ms, Maximum = 30ms, Average = 22ms\r\n"};

    PingData currentPingData{parser::getPingDataFromWindowsOutput(pingOutput)};
    check(currentPingData, {QDateTime::currentDateTime(), 4, 0, 22, 17, 30});
}

void ParserTest::testNonPingableHostOnWindows() const
{
    QString pingOutput{
        "\r\nPinging 212.77.1.2 with 32 bytes of data:\r\nRequest timed out.\r\nRequest timed out.\r\nRequest timed out.\r\nRequest timed out.\r\n\r\nPing statistics for 212.77.1.2:\r\n    Packets: Sent = 4, Received = 0, Lost = 4 (100% loss),\r\n"};

    PingData currentPingData{parser::getPingDataFromWindowsOutput(pingOutput)};
    check(currentPingData, {QDateTime::currentDateTime(), 4, 4, 0, 0, 0});
}

void ParserTest::testNoNetworkOnWindows() const
{
    QString pingOutput{""};

    PingData currentPingData{parser::getPingDataFromWindowsOutput(pingOutput)};
    check(currentPingData, {QDateTime::currentDateTime(), 0, 0, 0, 0, 0});
}

void ParserTest::testPacketsLostOnWindows() const
{
    QString pingOutput{
"\r\nPinging 212.77.98.9 with 32 bytes of data:\r\nPING: transmit failed. General failure. \r\nPING: transmit failed. General failure. \r\nReply from 212.77.98.9: bytes=32 time=14ms TTL=53\r\nReply from 212.77.98.9: bytes=32 time=18ms TTL=53\r\n\r\nPing statistics for 212.77.98.9:\r\n    Packets: Sent = 4, Received = 2, Lost = 2 (50% loss),\r\nApproximate round trip times in milli-seconds:\r\n    Minimum = 14ms, Maximum = 18ms, Average = 16ms\r\n"};

    PingData currentPingData{parser::getPingDataFromWindowsOutput(pingOutput)};
    check(currentPingData, {QDateTime::currentDateTime(), 4, 2, 16, 14, 18});
}

QTEST_MAIN(ParserTest)
