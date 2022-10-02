cmake_minimum_required(VERSION 3.1...3.24)

project(6502Emulator VERSION 1.0
        DESCRIPTION "6502 cpu emulator"
        LANGUAGES CXX)

add_executable(emulator cpu.cpp)