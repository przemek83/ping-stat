#include "PingData.h"

namespace parser
{
PingData getPingDataFromlinuxOutput(QString pingOutput);

PingData getPingDataFromWindowsOutput(QString pingOutput);
};  // namespace parser
