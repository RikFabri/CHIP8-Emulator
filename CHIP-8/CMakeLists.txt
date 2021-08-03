add_library(CHIP8 VM.h VM.cpp OpCodeHandler.h OpCodeHandler.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE CHIP8)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
