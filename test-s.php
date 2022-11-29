<?php
    print("Creating pipc suscriber ...");
    $subscriber = new PipcSubscriber();
    print(" OK" . PHP_EOL);

    $on_message = function($msg) {
//        print("Message received: '" . $msg . "'" . PHP_EOL);
        time_nanosleep(0, 5000000);
    };

    print("Subscribing for messages ... ");
    $subscriber->subscribe($on_message);
    print(" OK" . PHP_EOL);

    sleep(20);
    print("Unsubscribing from messages ... ");
    $subscriber->unsubscribe();
    print(" OK" . PHP_EOL);

    time_nanosleep(1, 50000000);

    print("Number of messages received: " . $subscriber->number_of_messages . PHP_EOL);
    print("Average delivery time: " . $subscriber->average_delivery_time . " microseconds" . PHP_EOL);
    print("Minimal delivery time: " . $subscriber->minimal_delivery_time . " microseconds" . PHP_EOL);
    print("Maximal delivery time: " . $subscriber->maximal_delivery_time . " microseconds" . PHP_EOL);

    time_nanosleep(1, 50000000);

?>
