[![Build & test](https://github.com/przemek83/ping-stat/actions/workflows/buld-and-test.yml/badge.svg)](https://github.com/przemek83/ping-stat/actions/workflows/buld-and-test.yml)
[![CodeQL analysis](https://github.com/przemek83/ping-stat/actions/workflows/codeql.yml/badge.svg)](https://github.com/przemek83/ping-stat/actions/workflows/codeql.yml)
[![codecov](https://codecov.io/github/przemek83/ping-stat/graph/badge.svg?token=EP1UNV6W35)](https://codecov.io/github/przemek83/ping-stat)

[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=przemek83_ping-stat&metric=alert_status)](https://sonarcloud.io/summary/new_code?id=przemek83_ping-stat)
[![Bugs](https://sonarcloud.io/api/project_badges/measure?project=przemek83_ping-stat&metric=bugs)](https://sonarcloud.io/summary/new_code?id=przemek83_ping-stat)
[![Code Smells](https://sonarcloud.io/api/project_badges/measure?project=przemek83_ping-stat&metric=code_smells)](https://sonarcloud.io/summary/new_code?id=przemek83_ping-stat)
[![Coverage](https://sonarcloud.io/api/project_badges/measure?project=przemek83_ping-stat&metric=coverage)](https://sonarcloud.io/summary/new_code?id=przemek83_ping-stat)
[![Duplicated Lines (%)](https://sonarcloud.io/api/project_badges/measure?project=przemek83_ping-stat&metric=duplicated_lines_density)](https://sonarcloud.io/summary/new_code?id=przemek83_ping-stat)

## Table of contents
- [About](#about)
- [Problem description](#problem-description)
- [Getting Started](#getting-started)
   * [Prerequisites](#prerequisites)
   * [Building](#building)
   * [Built with](#built-with)
- [Usage](#usage)
- [Testing](#testing)
- [License](#license)

## About
Program drawing given server response time on chart. Pinging of the hosts is done using the call of the `ping` command from the operating system and parsing output. Example screenshot:
![](screen.png?raw=true "")

## Problem description
Create a small application that will ping a given address, draw a plot with responses times over time, and will log data to a file (one log file per day).  
Given as an offline interview task.

**Input**:  
Editable fields:
- host IP - IP of host to ping
- timeout value - max time which ping command should wait for answer in milliseconds,
- interval value - frequency of pings in seconds.

**Output**:  
Plot with ping response statistics over time.

## Getting Started
This section describes briefly how to setup the environment and build the project.  

### Prerequisites
Qt in version 6.5 or greater, a C++ compiler with C++17 support as a minimum, and CMake 3.16+. 

### Building
Clone and use CMake directly or via any IDE supporting it. CMake should:
- configure everything automatically,
- compile and create binaries.

As a result of compilation, binary for simulations and binary for testing should be created.

### Built with
| Tool |  Windows | Ubuntu |
| --- | --- | --- |
| OS version | 10 22H2 | 22.04 |
| GCC | 13.1.0 | 13.2.0 |
| CMake | 3.30.2 | 3.28.3 |
| Git | 2.46.0 | 2.43.0 |
| Qt | 6.5.2 | 6.5.2 |
| Qt Creator | 10.0.2 | 10.0.2 |
| VS Code | 1.92.0 | 1.94.2 |

## Usage
Pick one host and get its IP (you can use the `ping -a <name>` command in console). Copy IP address into application field `Address`. Optionally change the `Timeout` field value to adjust the time at which the ping command will wait for an answer from the address. Also, optionally modify the `Interval` field value to change the frequency of calling the system ping command. If all fields are as intended, then hit the ping button to start pinging. Click it again to stop.  
There might be a few seconds of delay between clicking the ping/stop button and reaction. It is a result of the system `ping` command, which needs time to perform its job.

## Testing
For testing purposes, the QTest framework is used. Build the project first. Make sure that the `ping-stat-tests` target is built. Modern IDEs supporting CMake also support running tests with monitoring of failures. But in case you would like to run it manually, go to the `build/tests` directory, where the⁣ binary `ping-stat-tests` should be available. Launching it should produce the following output on Linux:
Example run:
```
$ ./ping-stat-tests
********* Start testing of ParserTest *********
Config: Using QtTest library 6.5.2, Qt 6.5.2 (x86_64-little_endian-lp64 shared (dynamic) release build; by GCC 10.3.1 20210422 (Red Hat 10.3.1-1)), ubuntu 24.04
PASS   : ParserTest::initTestCase()
PASS   : ParserTest::testPingableHostOnLinux()
PASS   : ParserTest::testNonPingableHostOnLinux()
PASS   : ParserTest::testNoNetworkOnLinux()
PASS   : ParserTest::testPacketsLostOnLinux()
PASS   : ParserTest::testPingableHostOnWindows()
PASS   : ParserTest::testNonPingableHostOnWindows()
PASS   : ParserTest::testNoNetworkOnWindows()
PASS   : ParserTest::testPacketsLostOnWindows()
PASS   : ParserTest::cleanupTestCase()
Totals: 10 passed, 0 failed, 0 skipped, 0 blacklisted, 1ms
********* Finished testing of ParserTest *********

```
As an alternative, CTest can be used to run tests from the `build/tests` directory:
```
$ ctest
Test project <path>/ping-stat/build/tests
    Start 1: ping-stat-tests
1/1 Test #1: ping-stat-tests ...................   Passed    0.01 sec

100% tests passed, 0 tests failed out of 1

Total Test time (real) =   0.01 sec
```

## License
Project is distributed under the MIT License. See `LICENSE` for more information.

The project uses the following software:
| Name | License | Home | Description |
| --- | --- | --- | --- |
| Qt | LGPLv3 | https://www.qt.io/| cross-platform application development framework |
