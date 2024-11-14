#pragma once

#include <QTest>

class ParserTest : public QObject
{
    Q_OBJECT

private slots:
    void testPingableHostOnLinux() const;
};
