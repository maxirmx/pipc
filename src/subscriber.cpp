/*
 *   Copyright (c) 2022 Maxim Samsonov. All rights reserved.
 *   This file is a part of pipc
 */

#include <iostream>

#include <message.h>
#include <subscriber.h>

using namespace std;

namespace pipc {

/**
 *  C++ object constructor
 */
PipcSubscriber::PipcSubscriber(void):
    PipcObject("subscriber"),
    subscriber{nullptr},
    callback{nullptr},
    n_msg{0},
    t_min{numeric_limits<int64_t>::max()},
    t_max{0},
    t_total{0}
{
}

/**
 *  C++ object destructor
 */
PipcSubscriber::~PipcSubscriber()
{
    unsubscribe();
    if (subscriber) {
        delete subscriber;
    }
}

/**
 *  PHP object constructor
 */
void PipcSubscriber::__construct(void)
{
    subscriber = new iox::popo::Subscriber<PipcMessage, PipcHeader>({
        "Pipc",
        "Pipc",
        "default"
    });
}

/**
 *  Wait for message (blocking)
 *  *** Debug only, do not use ***
 *  @param  params[0] Php::String Php::ByRef  target for new message
 */
void PipcSubscriber::receive_message(Php::Parameters &params)
{
    while(true) {
        auto take_result = subscriber->take();
        if (!take_result.has_error()) {
            params[0] =  (char*) take_result.value()->data;
            return;
        }
        else {
            auto error = take_result.get_error();
            if (take_result.get_error() != iox::popo::ChunkReceiveResult::NO_CHUNK_AVAILABLE) {
                throw Php::Exception(string("Pipc subscriber is unable to receive data: '") + iox::popo::asStringLiteral(error) + "'");
            }
        }
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

/**
 *  Translates listener error to char literal
 *  (not provided by iceoryx ???)
 *  Private, static
 *  @param  value ListenerError enum
 */
const char *PipcSubscriber::listenerErrorAsStringLiteral(const iox::popo::ListenerError value) noexcept
{
    switch (value)
    {
        case iox::popo::ListenerError::EVENT_ALREADY_ATTACHED:
            return "Event listener is already attached";
        case iox::popo::ListenerError::LISTENER_FULL:
            return "Event listener is full";
        case iox::popo::ListenerError::EMPTY_EVENT_CALLBACK:
            return "Event call back is empty";
    }
    return "Undefined listener error";
}

/**
 *  Subscribe for messages
 *  @param  params[0] Php::Callable object, callback that shall receive incoming message
 *  @throw  Php::Exception subscription failed
 */
void PipcSubscriber::subscribe(Php::Parameters &params)
{
    std::unique_lock<std::mutex> lck(callback_mtx, std::defer_lock);
    lck.lock();
    callback = params[0];
    listener.attachEvent(*subscriber,
                          iox::popo::SubscriberEvent::DATA_RECEIVED,
                          iox::popo::createNotificationCallback(on_message_received, *this))
            .or_else([&](iox::popo::ListenerError& error) {
                callback = nullptr;
                lck.unlock();
                throw Php::Exception(string("Pipc was unable to attach subscriber subscriber is unable to receive data: '") + listenerErrorAsStringLiteral(error) + "'");
    });
    lck.unlock();
}

/**
 *  Unsubscribe (detach) from messages
 *  Does nothing if not subscribed, i.e.: safe to call without checks or several times
 *  @param  subscriber a pointer to iox::popo::Subscriber object reveieving message
 *  @param  self a pointer to PipcSubsxriber object to route message to
 */
void PipcSubscriber::unsubscribe(void)
{
    std::unique_lock<mutex> lck(callback_mtx, defer_lock);
    listener.detachEvent(*subscriber, iox::popo::SubscriberEvent::DATA_RECEIVED);
    lck.lock();
    callback = nullptr;
    lck.unlock();
}

/**
 *  Message listener callback function
 *  Private, static
 *  @param  subscriber a pointer to iox::popo::Subscriber object reveieving message
 *  @param  self a pointer to PipcSubsxriber object to route message to
 */
void PipcSubscriber::on_message_received(iox::popo::Subscriber<PipcMessage, PipcHeader>* subscriber, PipcSubscriber* self)
{
   subscriber->take().and_then([subscriber, self](auto& message) {
        auto finish = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        Php::Value msg(message->data);
        auto duration = finish - message.getUserHeader().timestamp;
        unique_lock<mutex> lck(self->callback_mtx, defer_lock);
        lck.lock();
        self->account_for_new_message(duration);
        auto &f = self->callback;
        lck.unlock();
        if (!f.isNull()) f(msg);
    });
}

/**
 *  Check if property is known
 *  Private
 *  @param  name Property name
 */
bool PipcSubscriber::is_known_property(const Php::Value &name)  const {
    return (name == "number_of_messages"        || name == "minimal_delivery_time"  ||
            name == "maximal_delievery_time"    || name == "average_delivery_time" );

}

/**
 *  Account for a new message
 *  Private
 *  @param  duration       Delivery duration (seconds)
 */
void PipcSubscriber::account_for_new_message(int64_t d)
{
    n_msg++;
    t_min = min(t_min, d);
    t_max = max(t_max, d);
    t_total += d;
}

/**
 *  Get access to a property
 *  @param  name        Name of the property
 *  @return Value       Property value
 */
Php::Value PipcSubscriber::__get(const Php::Value &name)
{
    if (name == "number_of_messages") return n_msg;
    if (name == "minimal_delivery_time") return t_min;
    if (name == "maximal_delivery_time") return t_max;
    if (name == "average_delivery_time") return t_total/n_msg;

    return Php::Base::__get(name);
}

/**
 *  Overwrite a property
 *  @param  name        Name of the property
 *  @param  value       New property value
 */
void PipcSubscriber::__set(const Php::Value &name, const Php::Value &value)
{
    if (is_known_property(name))  throw Php::Exception("Pipc statistics cannot be altered manually");
    else                                Php::Base::__set(name, value);
}

/**
 *  Check if a property is set
 *  @param  name        Name of the property
 *  @return bool
 */
bool PipcSubscriber::__isset(const Php::Value &name)
{
    if (is_known_property(name)) return true;
    return Php::Base::__isset(name);
}

/**
 *  Remove a property
 *  @param  name        Name of the property to remove
 */
void PipcSubscriber::__unset(const Php::Value &name)
{
    if (is_known_property(name))  throw Php::Exception("Pipc statistics properties cannot be removed");
    Php::Base::__unset(name);
}


}
