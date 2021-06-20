add_library(chip Emulator.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE chip)
target_include_directories(${PROJECT_NAME}
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")