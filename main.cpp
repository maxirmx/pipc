#include <iostream>

#include "iceoryx_posh/popo/publisher.hpp"
#include "iceoryx_posh/runtime/posh_runtime.hpp"

#include <phpcpp.h>

using namespace std;

Php::Value initialize_publisher(Php::Parameters &params)
{
    constexpr char APP_NAME[] = "pipc-publisher";
    iox::runtime::PoshRuntime::initRuntime(APP_NAME);

    return 0;
}

void send_message(Php::Parameters &params)
{
    cout << "Message: '" << params[0] << "'" << endl;
}

extern "C" {

    /**
     *  Function that is called by PHP right after the PHP process
     *  has started, and that returns an address of an internal PHP
     *  strucure with all the details and features of your extension
     *
     *  @return void*   a pointer to an address that is understood by PHP
     */
    PHPCPP_EXPORT void *get_module()
    {
        // static(!) Php::Extension object that should stay in memory
        // for the entire duration of the process (that's why it's static)
        static Php::Extension extension("pipc", "1.0");

        extension.add<initialize_publisher>("pipc_initialize_publisher", {
            Php::ByVal("channel", Php::Type::String)
        });

        extension.add<send_message>("pipc_send_message", {
            Php::ByRef("message", Php::Type::String)
        });

        return extension;
    }
}
