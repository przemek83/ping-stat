#include "Constants.h"

const char* Constants::logDateFormat_ = "yyyy-MM-dd";

const char* Constants::logPrefix_ = "log_";

const char* Constants::logTimeFormat_ = "HH:mm:ss";

const char* Constants::displayTimeFormat_ = "HH:mm:ss dd.MM.yyyy";

const int Constants::maxTimeout_ = 1000;

const int Constants::maxPlotItems_ = 50;

const int Constants::minPlotItemsToResize_ = 10;

Constants::Constants() {}

Constants::~Constants() {}
int Constants::minPlotItemsToResize() { return minPlotItemsToResize_; }

int Constants::maxPlotItems() { return maxPlotItems_; }

int Constants::maxTimeout() { return maxTimeout_; }

const char* Constants::displayTimeFormat() { return displayTimeFormat_; }

const char* Constants::logTimeFormat() { return logTimeFormat_; }

const char* Constants::logPrefix() { return logPrefix_; }

const char* Constants::dateFormat() { return logDateFormat_; }
