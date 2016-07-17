project(malos_service C CXX)
cmake_minimum_required(VERSION 2.8)

add_definitions(-std=c++11)

# Enable extra warnings. Not needed.
include (../cmake/FatalWarnings.cmake)
ADM_EXTRA_WARNINGS()

find_package(Threads)

add_subdirectory(../cpp/driver driver)


add_executable(test_imu test_imu.cpp)
set_property(TARGET test_imu PROPERTY CXX_STANDARD 11)
target_link_libraries(test_imu matrix_creator_hal)
target_link_libraries(test_imu ${CMAKE_THREAD_LIBS_INIT})


add_executable(test_everloop test_everloop.cpp)
set_property(TARGET test_everloop PROPERTY CXX_STANDARD 11)
target_link_libraries(test_everloop matrix_creator_hal)
target_link_libraries(test_everloop ${CMAKE_THREAD_LIBS_INIT})


add_executable(arc_demo arc_demo.cpp)
set_property(TARGET arc_demo PROPERTY CXX_STANDARD 11)
target_link_libraries(arc_demo matrix_creator_hal)
target_link_libraries(arc_demo ${CMAKE_THREAD_LIBS_INIT})



