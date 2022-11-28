# pipc

#### iceoryx logging

By default iceoryx logging is turned off. This behavior can be altered via environment variable `IOX_LOG_LEVEL`

| LogLevel | Usage |
|----------|-------|
| FATAL    | For fatal and non-recoverable errors which essentially lead to termination of the program. |
| ERROR    | For severe but recoverable errors. |
| WARN     | For cases when something unintentional happens which is not considered a severe error. |
| INFO     | Anything which could be relevant for the daily user. |
| DEBUG    | Anything that is helpful for debugging, i.e. the info log level for the fellow developer. |
| TRACE    | Anything that might be helpful for debugging in some cases but would be too verbose for debug log level. |

Note that iceoryx logger writes to "console" (aka stdout).
Detailed description is provided be iceoryx documentation: https://github.com/eclipse-iceoryx/iceoryx/blob/master/doc/design/logging.md
