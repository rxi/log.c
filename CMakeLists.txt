cmake_minimum_required(VERSION 2.8)

project(logc)

enable_language(C)
add_definitions(-std=c99)

set(LOGC_SOURCES
    src/log.c
)

include_directories(src)

add_library(logc_shared SHARED ${LOGC_SOURCES})

set(PUBLIC_HEADERS
    src/log.h
)

set_target_properties(logc_shared PROPERTIES
    PUBLIC_HEADER "${PUBLIC_HEADERS}"
)

install(TARGETS logc_shared
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
