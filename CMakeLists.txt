cmake_minimum_required(VERSION 3.20)
project(total)

set(CMAKE_CXX_STANDARD 14)

add_executable(total main.cpp doDIff.cpp doEcho.cpp doGrep.cpp fileProcess.cpp stringProcess.cpp Terminal.cpp doPwd.cpp doCp.cpp doTee.cpp)
