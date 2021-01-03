## Ping statistics on chart
Program drawing pinging server response time on chart.  
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
Create small application which will ping given address, draw plot with responses times over time and logging data to log file (one per day).  
Given as offline interview task.

**Input**:  
On window it is possible to insert:
- host IP - IP of host to ping
- timeout value - max time which ping command should wait for answer in milliseconds,
- interval value - frequency of pings in seconds.

**Output**:  
Plot with ping response statistics over time.

## Getting Started
This section describes briefly how to setup environment and build project.

### Prerequisites
C++ compiler with C++17 support as a minimum and CMake 3.8+. As application is based on Qt you will also needed it to build application.

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
<TODO>

## License
Project is distributed under the MIT License. See `LICENSE` for more information.
