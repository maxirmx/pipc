#pragma once

#include "common.h"

struct PipcMessage {
    char data[64 * ONE_KILOBYTE];
};

struct PipcHeader {
    uint64_t timestamp;
};