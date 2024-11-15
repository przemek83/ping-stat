[![Build & test](https://github.com/przemek83/ping-stat/actions/workflows/buld-and-test.yml/badge.svg)](https://github.com/przemek83/ping-stat/actions/workflows/buld-and-test.yml)
[![CodeQL analysis](https://github.com/przemek83/ping-stat/actions/workflows/codeql.yml/badge.svg)](https://github.com/przemek83/ping-stat/actions/workflows/codeql.yml)
[![codecov](https://codecov.io/github/przemek83/ping-stat/graph/badge.svg?token=EP1UNV6W35)](https://codecov.io/github/przemek83/ping-stat)

[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=przemek83_ping-stat&metric=alert_status)](https://sonarcloud.io/summary/new_code?id=przemek83_ping-stat)
[![Bugs](https://sonarcloud.io/api/project_badges/measure?project=przemek83_ping-stat&metric=bugs)](https://sonarcloud.io/summary/new_code?id=przemek83_ping-stat)
[![Code Smells](https://sonarcloud.io/api/project_badges/measure?project=przemek83_ping-stat&metric=code_smells)](https://sonarcloud.io/summary/new_code?id=przemek83_ping-stat)
[![Coverage](https://sonarcloud.io/api/project_badges/measure?project=przemek83_ping-stat&metric=coverage)](https://sonarcloud.io/summary/new_code?id=przemek83_ping-stat)
[![Duplicated Lines (%)](https://sonarcloud.io/api/project_badges/measure?project=przemek83_ping-stat&metric=duplicated_lines_density)](https://sonarcloud.io/summary/new_code?id=przemek83_ping-stat)

## Ping statistics on chart
Program drawing given server response time on chart. Pinging of the hosts is done using call of `ping` command from operating system and parsing output.   
![](screen.png?raw=true "")

## Table of content
- [Problem description](#problem-description)
- [Getting Started](#getting-started)
  * [Prerequisites](#prerequisites)
  * [Building](#building)
- [Built with](#built-with)
- [Usage](#usage)
- [License](#license)

## Problem description
Create small application which will ping given address, draw plot with responses times over time and will log data to file (one log file per day).  
Given as offline interview task.

**Input**:  
Editable fields:
- host IP - IP of host to ping
- timeout value - max time which ping command should wait for answer in milliseconds,
- interval value - frequency of pings in seconds.

**Output**:  
Plot with ping response statistics over time.

## Getting Started
This section describes briefly how to setup environment and build project.

### Prerequisites
C++ compiler with C++17 support as a minimum and CMake 3.8+. As application is based on Qt you will also needed it to build this application.

### Building
Use QtCreator or Cmake.  

## Built with
| Tool |  Windows 10 | Lubuntu 20.04 |
| --- | --- | --- |
| GCC | 7.3.0 | 9.3.0 |
| Cmake | 3.14 | 3.17 |
| Git | 2.20 | 2.25 |
| QtCreator | 4.12.0 | 4.13.0 |

## Usage
Pick one host and get its IP (you can use `ping -a <name>` command in console). Copy IP address into application field `Address`. Optionally change `Timeout` field value to adjust time which ping command will wait for answer from address. Also optionally modify `Interval` field value to change frequency of calling system ping command. if all fields are as intended, than hit ping button to start pinging. Click it again to stop.  
There might be few seconds of delay between clicking ping/stop button and reaction. It is a result of system `ping` command which needs time to perform its job.

## Testing
For testing purposes, the QTest framework is used. Build the project first. Make sure that the `ping-stat-test` target is built. Modern IDEs supporting CMake also support running tests with monitoring of failures. But in case you would like to run it manually, go to the `build/test` directory, where the⁣ binary `ping-stat-test` should be available. Launching it should produce the following output on Linux:
Example run:
```
$ ./ping-stat-test
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
As an alternative, CTest can be used to run tests from the `build/test` directory:
```
$ ctest
Test project <path>/ping-stat/build/test
    Start 1: ping-stat-test
1/1 Test #1: ping-stat-test ...................   Passed    0.01 sec

100% tests passed, 0 tests failed out of 1

Total Test time (real) =   0.01 sec
```

## License
Project is distributed under the MIT License. See `LICENSE` for more information.

The project uses the following software:
| Name | License | Home | Description |
| --- | --- | --- | --- |
| Qt | LGPLv3 | https://www.qt.io/| cross-platform application development framework |
