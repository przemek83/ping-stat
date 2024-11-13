#pragma once

#include <QTest>

class Test : public QObject
{
    Q_OBJECT

private slots:
    void testSomething() const;
};
