<?php
    print("Creating pipc publisher ...");
    $publisher = new PipcPublisher();
    print(" OK" . PHP_EOL);
    $msg = "Some message";
    print("Sending '" . $msg . "'" . PHP_EOL );
    $publisher->send_message($msg);
?>
