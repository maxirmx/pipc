<?php
    print("Creating pipc publisher ...");
    $publisher = new PipcPublisher();
    print(" OK" . PHP_EOL);

    for ($i = 1; $i <= 1000; $i++) {
        $msg = "Message #" . $i;
//        print("Sending '" . $msg . "'" . PHP_EOL );
        $publisher->send_message($msg);
        time_nanosleep(0, 50000000);
    }
    time_nanosleep(0, 5000000);
?>
