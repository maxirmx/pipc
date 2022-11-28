/*
 *   Copyright (c) 2022 Maxim Samsonov. All rights reserved.
 *   This file is a part of pipc
 */

#include <message.h>
#include <subscriber.h>

using namespace std;

namespace pipc {

PipcSubscriber::PipcSubscriber(void):
    PipcObject("subscriber"),
    subscriber{nullptr},
    n_msg{0},
    t_min{0},
    t_max{0},
    t_total{0},
    callback(nullptr)
{
}

PipcSubscriber::~PipcSubscriber()
{
    unsubscribe();
    if (subscriber) {
        delete subscriber;
    }
}

void PipcSubscriber::__construct(void)
{
    subscriber = new iox::popo::Subscriber<PipcMessage, PipcHeader>({
        "Pipc",
        "Pipc",
        "default"
    });
}

//
// PipcSubscriber::receive_message
// Wait for message (blocking)
// *** Debug only ***
//
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

void PipcSubscriber::subscribe(Php::Parameters &params)
{
    std::unique_lock<std::mutex> lck(callback_mtx, std::defer_lock);
    lck.lock();
    callback = params[0];
    listener.attachEvent(*subscriber,
                          iox::popo::SubscriberEvent::DATA_RECEIVED,
                          iox::popo::createNotificationCallback(on_message_received, *this))
            .or_else([](auto) {
                cerr << "unable to attach subscriberLeft" << std::endl;
                exit(EXIT_FAILURE);
    });
    lck.unlock();
}

void PipcSubscriber::unsubscribe(void)
{
    std::unique_lock<std::mutex> lck(callback_mtx, std::defer_lock);
    lck.lock();
    listener.detachEvent(*subscriber, iox::popo::SubscriberEvent::DATA_RECEIVED);
    callback = nullptr;
    lck.unlock();
}

void PipcSubscriber::get_stat(Php::Parameters &params)
{

}

void PipcSubscriber::on_message_received(iox::popo::Subscriber<PipcMessage, PipcHeader>* subscriber, PipcSubscriber* self)
{
   subscriber->take().and_then([subscriber, self](auto& message) {
        Php::Value msg(message->data);
        std::unique_lock<std::mutex> lck(self->callback_mtx, std::defer_lock);
        lck.lock();
        if (!self->callback.isNull()) self->callback(msg);
        lck.unlock();
    });
}
}
