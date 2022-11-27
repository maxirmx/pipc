/*
 *   Copyright (c) 2022 Maxim Samsonov. All rights reserved.
 *   This file is a part of pipc
 */

#include <iostream>
#include <iceoryx_posh/popo/subscriber.hpp>
#include <phpcpp.h>

#include <message.h>
#include <object.h>

namespace pipc {

class PipcSubscriber : public Php::Base, public PipcObject
{
private:
    iox::popo::Subscriber<PipcMessage, PipcHeader> *subscriber;
public:
    PipcSubscriber(void);
    ~PipcSubscriber();
    void __construct(void);
    void receive_message(Php::Parameters &params);
};

}
