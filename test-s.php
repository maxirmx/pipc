<?php
    print("Creating pipc suscriber ...");
    $subscriber = new PipcSubscriber();
    print(" OK" . PHP_EOL);

    $on_message = function($msg) {
        print("Message received: '" . $msg . "'" . PHP_EOL);
    };

    print("Subscribing for messages ... ");
    $subscriber->subscribe($on_message);
    print(" OK" . PHP_EOL);

    sleep(60);
    print("Unubscribing from messages ... ");
    $subscriber->unsubscribe();
    print(" OK" . PHP_EOL);
?>
