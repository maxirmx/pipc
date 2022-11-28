/*
 *   Copyright (c) 2022 Maxim Samsonov. All rights reserved.
 *   This file is a part of pipc
 */

#include <message.h>
#include <publisher.h>
#include <subscriber.h>

using namespace std;
using namespace Php;

namespace pipc {
extern "C" {
    PHPCPP_EXPORT void *get_module()
    {
        static Extension extension("pipc", "1.0");

        Class<PipcPublisher> publisher("PipcPublisher");
        publisher.method<&PipcPublisher::__construct>("__construct");
        publisher.method<&PipcPublisher::send_message>("send_message", {
            ByVal("message", Type::String)
        });
        extension.add(move(publisher));

        Class<PipcSubscriber> subscriber("PipcSubscriber");
        subscriber.method<&PipcSubscriber::__construct>("__construct");
        subscriber.method<&PipcSubscriber::receive_message>("receive_message", {
            ByRef("message", Type::String)
        });
        subscriber.method<&PipcSubscriber::subscribe>("subscribe", {
            ByVal("callback", Type::Callable)
        });
        subscriber.method<&PipcSubscriber::unsubscribe>("unsubscribe");
        subscriber.method<&PipcSubscriber::unsubscribe>("get_stat", {
            ByVal("stat", Type::Object)
        });
        extension.add(move(subscriber));

        return extension;
    }
}
}