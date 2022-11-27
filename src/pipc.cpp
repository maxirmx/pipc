/*
 *   Copyright (c) 2022 Maxim Samsonov. All rights reserved.
 *   This file is a part of pipc
 */

#include <iostream>

#include "iceoryx_posh/popo/publisher.hpp"
#include "iceoryx_posh/popo/subscriber.hpp"

#include <phpcpp.h>

#include <message.h>
#include <object.h>

using namespace std;
using iox::cxx::TruncateToCapacity;

namespace pipc {

class PipcSubscriber : public Php::Base, public PipcObject
{
private:
    iox::popo::Subscriber<PipcMessage, PipcHeader> *subscriber;
public:
    PipcSubscriber(void): PipcObject("subscriber"), subscriber{nullptr} {
    };
    virtual ~PipcSubscriber() {
        if (subscriber)
            delete subscriber;
    };

    void __construct(void)
    {
        subscriber = new iox::popo::Subscriber<PipcMessage, PipcHeader>({
            "Pipc",
            "Pipc",
            "default"
        });
    }

    void receive_message(Php::Parameters &params)
    {
        while(true) {
            auto takeResult = subscriber->take();
            if (!takeResult.has_error())
            {
                std::cout << std::endl << app_name << " got message: " << takeResult.value()->data << std::endl;
                params[0] =  (char*) takeResult.value()->data;
                return;
            }
            else
            {
                if (takeResult.get_error() == iox::popo::ChunkReceiveResult::NO_CHUNK_AVAILABLE)
                {
                    cout << ".";
                    cout.flush();
                }
                else
                {
                    std::cout << "Error receiving chunk." << std::endl;
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
};

class PipcPublisher : public Php::Base, public PipcObject
{
private:
    iox::popo::Publisher<PipcMessage, PipcHeader> *publisher;
public:
    PipcPublisher(void): PipcObject("publisher"), publisher{nullptr} {
    };
    virtual ~PipcPublisher() {
        if (publisher)
            delete publisher;
    };

    void __construct(void)
    {
        publisher = new iox::popo::Publisher<PipcMessage, PipcHeader>({
            "Pipc",
            "Pipc",
            "default"
        });
    }

    void send_message(Php::Parameters &params)
    {
        auto loan_result = publisher->loan();
        if (!loan_result.has_error()) {
            auto& sample = loan_result.value();
            strncpy(sample->data, static_cast<string>(params[0]).c_str(), sizeof(sample->data)/sizeof(sample->data[0]));
//            auto timestamp = std::chrono::steady_clock::now();
            sample.getUserHeader().timestamp = 42;
            cout << "Sending message: '" << sample->data << "'" << endl;
            sample.publish();
        }
        else {
            auto error = loan_result.get_error();
            throw Php::Exception(std::string("Unable to loan shared memory, error code: ") + iox::popo::asStringLiteral(error));
        }
    }
};


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

        Php::Class<PipcPublisher> publisher("PipcPublisher");
        publisher.method<&PipcPublisher::__construct>("__construct");
        publisher.method<&PipcPublisher::send_message>("send_message", {
            Php::ByRef("message", Php::Type::String)
        });
        extension.add(std::move(publisher));

        Php::Class<PipcSubscriber> subscriber("PipcSubscriber");
        subscriber.method<&PipcSubscriber::__construct>("__construct");
        subscriber.method<&PipcSubscriber::receive_message>("receive_message", {
            Php::ByRef("message", Php::Type::String)
        });
        extension.add(std::move(subscriber));


        return extension;
    }
}
}