<?php
    print("Creating pipc suscriber ...");
    $subscriber = new PipcSubscriber();
    print(" OK" . PHP_EOL);
    $msg = "*** No message ***";
    print("Waiting for message ...");
    $subscriber->receive_message($msg);
    print(" received: '" . $msg . "'" . PHP_EOL);
?>
