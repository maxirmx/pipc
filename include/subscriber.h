/*
 *   Copyright (c) 2022 Maxim Samsonov. All rights reserved.
 *   This file is a part of pipc
 */

#include <iostream>
#include <mutex>

#include <iceoryx_posh/popo/subscriber.hpp>
#include <iceoryx_posh/popo/listener.hpp>

#include <phpcpp.h>

#include <message.h>
#include <object.h>

namespace pipc {

class PipcSubscriber : public Php::Base, public PipcObject
{
private:
    iox::popo::Subscriber<PipcMessage, PipcHeader> *subscriber;
    uint64_t n_msg;                     // number of messages received
    uint64_t t_min;                     // minimal delivery time (microseconds)
    uint64_t t_max;                     // maximum delivery time (microseconds)
    uint64_t t_total;                   // sum of delivery times for all messages (microseconds)
                                        // we will return average delivery time = t_total/n_msg

    Php::Value callback;
    std::mutex callback_mtx;
    iox::popo::Listener listener;

    static void on_message_received(iox::popo::Subscriber<PipcMessage, PipcHeader>* subscriber, PipcSubscriber* self);

public:
    PipcSubscriber(void);
    ~PipcSubscriber();
;
    void __construct(void);
    void receive_message(Php::Parameters &params);
    void subscribe(Php::Parameters &params);
    void unsubscribe(void);
    void get_stat(Php::Parameters &params);
};

}
