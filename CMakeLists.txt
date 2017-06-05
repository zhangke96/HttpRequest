cmake_minimum_required(VERSION 3.6)
project(HttpRequest)

set(CMAKE_CXX_COMPILER  /usr/bin/clang++)
set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        http_parser.c
        http_parser.h
        http_parser_note.md
        makefile
        README.md
        request.cpp
        tool.h ResponseParser.cpp ResponseParser.h Response.cpp Response.h)

add_executable(HttpRequest ${SOURCE_FILES})