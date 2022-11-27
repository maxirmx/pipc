/*
 *   Copyright (c) 2022 Maxim Samsonov. All rights reserved.
 *   This file is a part of pipc
 */

#pragma once

#include <cstdint>

static constexpr uint32_t ONE_KILOBYTE = 1024U;
static constexpr uint32_t ONE_MEGABYTE = 1024U * 1024;

struct PipcMessage {
    char data[64 * ONE_KILOBYTE];
};

struct PipcHeader {
    uint64_t timestamp;
};