cmake_minimum_required(VERSION 3.19)

project(CHIP-8-Emulator VERSION 0.1 LANGUAGES CXX)

add_subdirectory(QT)
add_subdirectory(Emulator)

set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
