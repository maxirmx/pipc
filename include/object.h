#pragma once

#include <string>
#include <cstdint>

namespace pipc {
class PipcObject
{
private:
    static bool runtime_initialized;
    static void init_runtime(const std::string& app_class);
protected:
    static std::string app_name;
public:
    PipcObject(const std::string& app_class) { init_runtime(app_class); };
    ~PipcObject(void) = default;
};
}
