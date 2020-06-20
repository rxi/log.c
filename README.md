# log.c
A simple logging library implemented in C99

![screenshot](https://cloud.githubusercontent.com/assets/3920290/23831970/a2415e96-0723-11e7-9886-f8f5d2de60fe.png)


## Usage
**[log.c](src/log.c?raw=1)** and **[log.h](src/log.h?raw=1)** should be dropped
into an existing project and compiled along with it. The library provides 6
function-like macros for logging:

```c
log_trace(const char *fmt, ...);
log_debug(const char *fmt, ...);
log_info(const char *fmt, ...);
log_warn(const char *fmt, ...);
log_error(const char *fmt, ...);
log_fatal(const char *fmt, ...);
```

Each function takes a printf format string followed by additional arguments:

```c
log_trace("Hello %s", "world")
```

Resulting in a line with the given format printed to stderr:

```
20:18:26 TRACE src/main.c:11: Hello world
```


#### log_set_quiet(bool enable)
Quiet-mode can be enabled by passing `true` to the `log_set_quiet()` function.
While this mode is enabled the library will not output anything to `stderr`, but
will continue to write to files and callbacks if any are set.


#### log_set_level(int level)
The current logging level can be set by using the `log_set_level()` function.
All logs below the given level will not be written to `stderr`. By default the
level is `LOG_TRACE`, such that nothing is ignored.


#### log_add_fp(FILE *fp, int level)
One or more file pointers where the log will be written can be provided to the
library by using the `log_add_fp()` function. The data written to the file
output is of the following format:

```
2047-03-11 20:18:26 TRACE src/main.c:11: Hello world
```

Any messages below the given `level` are ignored. If the library failed to add a
file pointer a value less-than-zero is returned.


#### log_add_callback(log_LogFn fn, void *udata, int level)
One or more callback functions which are called with the log data can be
provided to the library by using the `log_add_callback()` function. A callback
function is passed a `log_Event` structure containing the `line` number,
`filename`, `fmt` string, `va` printf va\_list, `level` and the given `udata`.


#### log_set_lock(log_LockFn fn, void *udata)
If the log will be written to from multiple threads a lock function can be set.
The function is passed the boolean `true` if the lock should be acquired or
`false` if the lock should be released and the given `udata` value.


#### const char* log_level_string(int level)
Returns the name of the given log level as a string.


#### LOG_USE_COLOR
If the library is compiled with `-DLOG_USE_COLOR` ANSI color escape codes will
be used when printing.


## License
This library is free software; you can redistribute it and/or modify it under
the terms of the MIT license. See [LICENSE](LICENSE) for details.
