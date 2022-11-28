/*
 *   Copyright (c) 2022 Maxim Samsonov. All rights reserved.
 *   This file is a part of pipc
 */

#include <uuid/uuid.h>
#include <iostream>
#include <iceoryx_posh/runtime/posh_runtime.hpp>

#include <object.h>

using namespace std;
using iox::cxx::TruncateToCapacity;

namespace pipc {

bool PipcObject::runtime_initialized = false;
std::string PipcObject::app_name = "Uninitialized";

void PipcObject::init_runtime(const string& app_class)
{
    if (!runtime_initialized) {
        uuid_t uuidObj;
        char app_uuid[iox::MAX_RUNTIME_NAME_LENGTH];
        uuid_generate(uuidObj);
        uuid_unparse(uuidObj, app_uuid);
        app_name = app_class + "-" + app_uuid;
        iox::log::Logger::init(iox::log::logLevelFromEnvOr(iox::log::LogLevel::OFF));
        iox::runtime::PoshRuntime::initRuntime({ TruncateToCapacity, app_name });
        runtime_initialized = true;
    }
}

}
