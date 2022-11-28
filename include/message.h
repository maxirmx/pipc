/*
 *   Copyright (c) 2022 Maxim Samsonov. All rights reserved.
 *   This file is a part of pipc
 */

#pragma once

#include <cstdint>
#include <chrono>

static constexpr uint32_t ONE_KILOBYTE = 1024U;
static constexpr uint32_t ONE_MEGABYTE = 1024U * 1024;

struct PipcMessage {
    char data[64 * ONE_KILOBYTE];
};

struct PipcHeader {
    std::chrono::time_point<std::chrono::_V2::steady_clock, std::chrono::duration<long int, std::ratio<1, 1000000000> > > timestamp;
};