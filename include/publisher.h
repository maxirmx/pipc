/*
 *   Copyright (c) 2022 Maxim Samsonov. All rights reserved.
 *   This file is a part of pipc
 */

#include <iostream>
#include <iceoryx_posh/popo/publisher.hpp>
#include <phpcpp.h>

#include <message.h>
#include <object.h>

namespace pipc {
class PipcPublisher : public Php::Base, public PipcObject
{
private:
    iox::popo::Publisher<PipcMessage, PipcHeader> *publisher;
public:
    PipcPublisher(void) : PipcObject("publisher"), publisher{nullptr} { };
    ~PipcPublisher() { if (publisher) delete publisher; }

    void __construct(void);
    void send_message(Php::Parameters &params);
};

}
