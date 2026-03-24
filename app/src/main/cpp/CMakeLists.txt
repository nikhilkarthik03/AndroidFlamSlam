cmake_minimum_required(VERSION 3.22.1)

project("androidflamslam")

file(GLOB SOKOL_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/sokol/*.cpp
)

add_library(${CMAKE_PROJECT_NAME} SHARED
        native-lib.cpp
        sokol.cpp
        ${SOKOL_SOURCES}
)

target_include_directories(
        ${CMAKE_PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/sokol
)

target_link_libraries(${CMAKE_PROJECT_NAME}
        android
        log
        GLESv3
        jnigraphics
)