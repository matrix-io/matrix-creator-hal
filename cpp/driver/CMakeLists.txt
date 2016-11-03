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
    mcu_firmware.cpp
    microphone_array.cpp
    matrix_driver.cpp
    humidity_sensor.cpp
    imu_sensor.cpp
    pressure_sensor.cpp
    uv_sensor.cpp
    dummy_sensor.cpp
    gpio_control.cpp
    everloop.cpp
    wishbone_bus.cpp
)

add_library(matrix_creator_hal ${matrix_creator_hal_src})
set_property(TARGET matrix_creator_hal PROPERTY CXX_STANDARD 11)
target_link_libraries(matrix_creator_hal ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_THREAD_LIBS_INIT})

install (TARGETS matrix_creator_hal DESTINATION lib)

set (matrix_creator_hal_headers
creator_memory_map.h  everloop.h        humidity_data.h    imu_sensor.h        pressure_data.h
dummy_data.h          everloop_image.h  humidity_sensor.h  matrix_driver.h     pressure_sensor.h
dummy_sensor.h        gpio_control.h    imu_data.h         microphone_array.h  wishbone_bus.h
uv_data.h             uv_sensor.h       fw_data.h          mcu_firmware.h
)

install (FILES ${matrix_creator_hal_headers} DESTINATION include/matrix_hal)
