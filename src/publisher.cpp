/*
 *   Copyright (c) 2022 Maxim Samsonov. All rights reserved.
 *   This file is a part of pipc
 */

#include <message.h>
#include <publisher.h>

using namespace std;

namespace pipc {

void PipcPublisher::__construct(void)
{

    // create publisher with some options set
    iox::popo::PublisherOptions publisher_options;
    // the publishers offer single message only, no history
    publisher_options.historyCapacity = 1U;
    publisher = new iox::popo::Publisher<PipcMessage, PipcHeader>(
        { "Pipc", "Pipc",  "default" },
        publisher_options
    );
}

void PipcPublisher::send_message(Php::Parameters &params)
{
    auto loan_result = publisher->loan();
    if (!loan_result.has_error()) {
        auto& sample = loan_result.value();
        strncpy(sample->data, static_cast<string>(params[0]).c_str(), sizeof(sample->data)-1);
        sample->data[sizeof(sample->data)-1] = 0;
        sample.getUserHeader().timestamp = std::chrono::steady_clock::now();
        sample.publish();
    }
    else {
        auto error = loan_result.get_error();
        throw Php::Exception(string("Pipc publisher is unable to loan shared memory: '") + iox::popo::asStringLiteral(error) + "'");
    }
}
}
