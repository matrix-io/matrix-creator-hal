project(matrix_creator_hal C CXX)
cmake_minimum_required(VERSION 2.8)

add_compile_options(-std=c++11)

# Enable extra warnings. Not needed.
include (../../cmake/FatalWarnings.cmake)
ADM_EXTRA_WARNINGS()

find_package(Threads)

include_directories(
  ../..
)

set(matrix_creator_hal_src
    matrix_driver.cpp
    humidity_sensor.cpp
    imu_sensor.cpp
    pressure_sensor.cpp
    dummy_sensor.cpp
    gpio_control.cpp
    everloop.cpp
    wishbone_bus.cpp
)

add_library(matrix_creator_hal ${matrix_creator_hal_src})
set_property(TARGET matrix_creator_hal PROPERTY CXX_STANDARD 11)
target_link_libraries(matrix_creator_hal ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_THREAD_LIBS_INIT})

