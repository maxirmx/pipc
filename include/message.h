/*
 *   Copyright (c) 2022 Maxim Samsonov. All rights reserved.
 *   This file is a part of pipc
 */

#pragma once

#include <cstdint>
#include <chrono>

static constexpr uint32_t ONE_KILOBYTE = 1024U;
static constexpr uint32_t ONE_MEGABYTE = 1024U * 1024;

namespace pipc {

struct PipcMessage {
    int64_t timestamp;
    char data[64 * ONE_KILOBYTE - sizeof(int64_t)];
};

struct PipcHeader {
    int64_t timestamp;
};
}
