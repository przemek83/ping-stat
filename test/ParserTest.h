#pragma once

#include <QTest>

class ParserTest : public QObject
{
    Q_OBJECT

private slots:
    void testPingableHostOnLinux() const;
    void testNonPingableHostOnLinux() const;
    void testNoNetworkOnLinux() const;
    void testPacketsLostOnLinux() const;

    void testPingableHostOnWindows() const;
    void testNonPingableHostOnWindows() const;
    void testNoNetworkOnWindows() const;
    void testPacketsLostOnWindows() const;
};
