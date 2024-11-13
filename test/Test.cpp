#include "Test.h"

void Test::testSomething() const { QCOMPARE(true, true); }

QTEST_MAIN(Test)