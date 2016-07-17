project(malos_service C CXX)
cmake_minimum_required(VERSION 2.8)

add_definitions(-std=c++11)

# Enable extra warnings. Not needed.
include (../cmake/FatalWarnings.cmake)
ADM_EXTRA_WARNINGS()

find_package(Threads)

add_subdirectory(../cpp/driver driver)


add_executable(imu_demo imu_demo.cpp)
set_property(TARGET imu_demo PROPERTY CXX_STANDARD 11)
target_link_libraries(imu_demo matrix_creator_hal)
target_link_libraries(imu_demo ${CMAKE_THREAD_LIBS_INIT})


add_executable(everloop_demo everloop_demo.cpp)
set_property(TARGET everloop_demo PROPERTY CXX_STANDARD 11)
target_link_libraries(everloop_demo matrix_creator_hal)
target_link_libraries(everloop_demo ${CMAKE_THREAD_LIBS_INIT})


add_executable(arc_demo arc_demo.cpp)
set_property(TARGET arc_demo PROPERTY CXX_STANDARD 11)
target_link_libraries(arc_demo matrix_creator_hal)
target_link_libraries(arc_demo ${CMAKE_THREAD_LIBS_INIT})



