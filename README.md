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


#### log_set_quiet(int enable)
Quiet-mode can be enabled by passing `1` to the `log_set_quiet()` function.
While this mode is enabled the library will not output anything to stderr, but
will continue to write to the file if one is set.


#### log_set_level(int level)
The current logging level can be set by using the `log_set_level()` function.
All logs below the given level will be ignored. By default the level is
`LOG_TRACE`, such that nothing is ignored.


#### log_set_fp(FILE *fp)
A file pointer where the log should be written can be provided to the library by
using the `log_set_fp()` function. The data written to the file output is
of the following format:

```
2047-03-11 20:18:26 TRACE src/main.c:11: Hello world
```


#### log_set_lock(log_LockFn fn)
If the log will be written to from multiple threads a lock function can be set.
The function is passed a `udata` value (set by `log_set_udata()`) and the
integer `1` if the lock should be acquired or `0` if the lock should be
released.


#### LOG_USE_COLOR
If the library is compiled with `-DLOG_USE_COLOR` ANSI color escape codes will
be used when printing.


## License
This library is free software; you can redistribute it and/or modify it under
the terms of the MIT license. See [LICENSE](LICENSE) for details.
