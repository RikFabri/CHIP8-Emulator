cmake_minimum_required(VERSION 3.19)

project(CHIP-8-Emulator VERSION 0.1 LANGUAGES CXX)

add_subdirectory(QT)
add_subdirectory(CHIP-8)

include(CPack)
include(InstallRequiredSystemLibraries)
install(TARGETS ${PROJECT_NAME} DESTINATION bin)