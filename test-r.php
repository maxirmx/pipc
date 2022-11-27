<?php
    $subscriber = new PipcSubscriber();
    $msg = "No message";
    $subscriber->receive_message($msg);
    print($msg . PHP_EOL);
?>
