#include <php_version.h>
#include <iostream>
#include <phpcpp.h>

using namespace std;

void send_message()
{
    cout << "In send_message()" << endl;
    cout << PHP_VERSION_ID << endl;
}

extern "C" {

    /**
     *  Function that is called by PHP right after the PHP process
     *  has started, and that returns an address of an internal PHP
     *  strucure with all the details and features of your extension
     *
     *  @return void*   a pointer to an address that is understood by PHP
     */
    PHPCPP_EXPORT void *get_module()
    {
        // static(!) Php::Extension object that should stay in memory
        // for the entire duration of the process (that's why it's static)
        static Php::Extension extension("pipc", "1.0");

       // add function to extension
        extension.add<send_message>("send_message");
        return extension;
    }
}
