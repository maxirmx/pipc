#include <iostream>

#include "iceoryx_posh/popo/publisher.hpp"
#include "iceoryx_posh/runtime/posh_runtime.hpp"

#include <phpcpp.h>

#include <pipc/common.h>
#include <pipc/message.h>

using namespace std;

namespace pipc {
class PipcPublisher : public Php::Base
{
private:
    static bool runtime_initialized;
    static void init_runtime(void);
    iox::popo::Publisher<PipcMessage, PipcHeader> *publisher;
public:
    /**
     *  C++ constructor and destructor
     */
    PipcPublisher(void): publisher{nullptr} {
        init_runtime();
    };
    virtual ~PipcPublisher() {
        if (publisher)
            delete publisher;
    };

    void __construct(void)
    {
        publisher = new iox::popo::Publisher<PipcMessage, PipcHeader>({"Pipc", "Pipc", "Message"});
    }

    void send_message(Php::Parameters &params)
    {
        cout << "Sending message: '" << params[0] << "'" << endl;
        auto loan_result = publisher->loan();
        if (!loan_result.has_error()) {
            auto& sample = loan_result.value();
            strncpy(sample->data, params[0], sizeof(sample->data)/sizeof(sample->data[0]));
//            auto timestamp = std::chrono::steady_clock::now();
            sample.getUserHeader().timestamp = 42;
            sample.publish();
        }
        else {
            auto error = loan_result.get_error();
            throw Php::Exception(std::string("Unable to loan shared memory, error code: ") + iox::popo::asStringLiteral(error));
        }
    }
};

bool PipcPublisher::runtime_initialized = false;

void PipcPublisher::init_runtime(void)
{
    if (!runtime_initialized) {
        constexpr char app_name[] = "pipc-publisher";
        iox::runtime::PoshRuntime::initRuntime(app_name);
        runtime_initialized = true;
    }
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

        Php::Class<PipcPublisher> channel("PipcPublisher");
        channel.method<&PipcPublisher::__construct>("__construct");
        channel.method<&PipcPublisher::send_message>("send_message", {
            Php::ByRef("message", Php::Type::String)
        });
        extension.add(std::move(channel));

        return extension;
    }
}
}