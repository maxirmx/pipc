/*
 *   Copyright (c) 2022 Maxim Samsonov. All rights reserved.
 *   This file is a part of pipc
 */

#include <atomic>
#include <mutex>

#include <iceoryx_hoofs/cxx/optional.hpp>
#include <iceoryx_posh/popo/subscriber.hpp>
#include <iceoryx_posh/popo/wait_set.hpp>

#include <phpcpp.h>

#include <message.h>
#include <object.h>

namespace pipc {
class PipcSubscriber : public Php::Base, public PipcObject
{
private:
    iox::popo::Subscriber<PipcMessage, PipcHeader> *subscriber;
    Php::Value callback;
    std::mutex callback_mtx;

    std::atomic_bool keep_running;

    int64_t n_msg;      // number of messages received
    int64_t t_min;     // minimal delivery time (microseconds)
    int64_t t_max;     // maximum delivery time (microseconds)
    int64_t t_total;   // sum of delivery times for all messages (microseconds)
                        // we will return average delivery time = t_total/n_msg

    bool is_known_property(const Php::Value &name) const;
    void account_for_new_message(int64_t d);

public:
    PipcSubscriber(void);
    ~PipcSubscriber();

    void __construct(void);
    void receive_message(Php::Parameters &params);
    void subscribe(Php::Parameters &params);
    void unsubscribe(void);

    Php::Value __get(const Php::Value &name);
    void __set(const Php::Value &name, const Php::Value &value);
    bool __isset(const Php::Value &name);
    void __unset(const Php::Value &name);
};

}
