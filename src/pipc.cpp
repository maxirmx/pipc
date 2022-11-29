/*
 *   Copyright (c) 2022 Maxim Samsonov. All rights reserved.
 *   This file is a part of pipc
 */

#include <message.h>
#include <publisher.h>
#include <subscriber.h>

using namespace std;

namespace pipc {
extern "C" {
   /**
    *  PHP extension module definition
    */
    PHPCPP_EXPORT void *get_module()
    {
        static Php::Extension extension("pipc", "1.0");

        Php::Class<PipcPublisher> publisher("PipcPublisher");
        publisher.method<&PipcPublisher::__construct>("__construct");
        publisher.method<&PipcPublisher::send_message>("send_message", {
            Php::ByVal("message", Php::Type::String)
        });
        extension.add(std::move(publisher));

        Php::Class<PipcSubscriber> subscriber("PipcSubscriber");
        subscriber.method<&PipcSubscriber::__construct>("__construct");
        subscriber.method<&PipcSubscriber::receive_message>("receive_message", {
            Php::ByRef("message", Php::Type::String)
        });
        subscriber.method<&PipcSubscriber::subscribe>("subscribe", {
            Php::ByVal("callback", Php::Type::Callable)
        });
        subscriber.method<&PipcSubscriber::unsubscribe>("unsubscribe");
        extension.add(std::move(subscriber));

        return extension;
    }
}
}