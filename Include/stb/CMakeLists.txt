add_library(stb stb_image.cpp)

target_include_directories(${PROJECT_NAME}
PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/"
)
target_link_libraries(${PROJECT_NAME} PUBLIC stb)