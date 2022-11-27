#include <uuid/uuid.h>

#include <iostream>

#include "iceoryx_posh/runtime/posh_runtime.hpp"

#include <object.h>

using iox::cxx::TruncateToCapacity;

namespace pipc {
bool PipcObject::runtime_initialized = false;
std::string PipcObject::app_name = "Uninitialized";

void PipcObject::init_runtime(const std::string& app_class)
{
    if (!runtime_initialized) {
        uuid_t uuidObj;
        char app_uuid[iox::MAX_RUNTIME_NAME_LENGTH];
        uuid_generate(uuidObj);
        uuid_unparse(uuidObj, app_uuid);
        app_name = app_class + "-" + app_uuid;
        iox::runtime::PoshRuntime::initRuntime({ TruncateToCapacity, app_name });
        runtime_initialized = true;
        std::cout << "Initialized with app_name ='" << app_name << "'" << std::endl;
    }
}
}
