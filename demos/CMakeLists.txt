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


add_executable(mic_demo mic_demo.cpp)
set_property(TARGET mic_demo PROPERTY CXX_STANDARD 11)
target_link_libraries(mic_demo matrix_creator_hal)
target_link_libraries(mic_demo ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(mic_demo wiringPi)

add_executable(ir_demo ir_demo.cpp)
set_property(TARGET ir_demo PROPERTY CXX_STANDARD 11)
target_link_libraries(ir_demo matrix_creator_hal)
target_link_libraries(ir_demo ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(ir_demo wiringPi)

add_executable(micarray_recorder micarray_recorder.cpp)
set_property(TARGET micarray_recorder PROPERTY CXX_STANDARD 11)
target_link_libraries(micarray_recorder matrix_creator_hal)
target_link_libraries(micarray_recorder ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(micarray_recorder wiringPi)

add_executable(servo_demo servo_demo.cpp)
set_property(TARGET servo_demo PROPERTY CXX_STANDARD 11)
target_link_libraries(servo_demo matrix_creator_hal)
target_link_libraries(servo_demo ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(servo_demo wiringPi)

add_executable(gpio_demo gpio_demo.cpp)
set_property(TARGET gpio_demo PROPERTY CXX_STANDARD 11)
target_link_libraries(gpio_demo matrix_creator_hal)
target_link_libraries(gpio_demo ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(gpio_demo wiringPi)
