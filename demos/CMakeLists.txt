project(malos_service C CXX)
cmake_minimum_required(VERSION 2.8)

add_definitions(-std=c++11)

# Enable extra warnings. Not needed.
include (../cmake/FatalWarnings.cmake)
ADM_EXTRA_WARNINGS()

find_package(Threads)

add_subdirectory(../cpp/driver driver)


add_executable(compass_demo compass_demo.cpp)
set_property(TARGET compass_demo PROPERTY CXX_STANDARD 11)
target_link_libraries(compass_demo matrix_creator_hal)
target_link_libraries(compass_demo ${CMAKE_THREAD_LIBS_INIT})

add_executable(everloop_demo everloop_demo.cpp)
set_property(TARGET everloop_demo PROPERTY CXX_STANDARD 11)
target_link_libraries(everloop_demo matrix_creator_hal)
target_link_libraries(everloop_demo ${CMAKE_THREAD_LIBS_INIT})

add_executable(arc_demo arc_demo.cpp)
set_property(TARGET arc_demo PROPERTY CXX_STANDARD 11)
target_link_libraries(arc_demo matrix_creator_hal)
target_link_libraries(arc_demo ${CMAKE_THREAD_LIBS_INIT})

add_executable(sensors_demo sensors_demo.cpp)
set_property(TARGET sensors_demo PROPERTY CXX_STANDARD 11)
target_link_libraries(sensors_demo matrix_creator_hal)
target_link_libraries(sensors_demo ${CMAKE_THREAD_LIBS_INIT})

add_executable(uv_demo uv_demo.cpp)
set_property(TARGET uv_demo PROPERTY CXX_STANDARD 11)
target_link_libraries(uv_demo matrix_creator_hal)
target_link_libraries(uv_demo ${CMAKE_THREAD_LIBS_INIT})

add_executable(servo_demo servo_demo.cpp)
set_property(TARGET servo_demo PROPERTY CXX_STANDARD 11)
target_link_libraries(servo_demo matrix_creator_hal)
target_link_libraries(servo_demo ${CMAKE_THREAD_LIBS_INIT})

add_executable(firmware_info firmware_info.cpp)
set_property(TARGET firmware_info PROPERTY CXX_STANDARD 11)
target_link_libraries(firmware_info matrix_creator_hal)
target_link_libraries(firmware_info ${CMAKE_THREAD_LIBS_INIT})

add_executable(clear_demo clear_demo.cpp)
set_property(TARGET clear_demo PROPERTY CXX_STANDARD 11)
target_link_libraries(clear_demo matrix_creator_hal)
target_link_libraries(clear_demo ${CMAKE_THREAD_LIBS_INIT})

add_executable(sensors_status sensors_status.cpp)
set_property(TARGET ipaddress_demo PROPERTY CXX_STANDARD 11)
target_link_libraries(sensors_status matrix_creator_hal)
target_link_libraries(sensors_status ${CMAKE_THREAD_LIBS_INIT})

