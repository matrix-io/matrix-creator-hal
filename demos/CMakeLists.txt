project(malos_service C CXX)
cmake_minimum_required(VERSION 2.8)

add_definitions(-std=c++11)

# Enable extra warnings. Not needed.
include (../cmake/FatalWarnings.cmake)
ADM_EXTRA_WARNINGS()

find_package(Threads)
find_library (FFTW_LIBRARIES NAMES fftw3f )
FIND_LIBRARY(WIRINGPI_LIB NAMES wiringPi)
FIND_LIBRARY(WIRINGPI_DEV_LIB NAMES wiringPiDev)
FIND_LIBRARY(CRYPT_LIB NAMES crypt)

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

add_executable(mic_demo mic_demo.cpp)
  set_property(TARGET mic_demo PROPERTY CXX_STANDARD 11)
  target_link_libraries(mic_demo matrix_creator_hal)
  target_link_libraries(mic_demo ${CMAKE_THREAD_LIBS_INIT})
  target_link_libraries(mic_demo ${WIRINGPI_LIB} ${WIRINGPI_DEV_LIB} ${CRYPT_LIB})

add_executable(ir_demo ir_demo.cpp)
  set_property(TARGET ir_demo PROPERTY CXX_STANDARD 11)
  target_link_libraries(ir_demo matrix_creator_hal)
  target_link_libraries(ir_demo ${CMAKE_THREAD_LIBS_INIT})
  target_link_libraries(ir_demo ${WIRINGPI_LIB} ${WIRINGPI_DEV_LIB} ${CRYPT_LIB})

add_executable(micarray_recorder micarray_recorder.cpp)
  set_property(TARGET micarray_recorder PROPERTY CXX_STANDARD 11)
  target_link_libraries(micarray_recorder matrix_creator_hal)
  target_link_libraries(micarray_recorder ${CMAKE_THREAD_LIBS_INIT})
  target_link_libraries(micarray_recorder ${WIRINGPI_LIB} ${WIRINGPI_DEV_LIB} ${CRYPT_LIB})

add_executable(servo_demo servo_demo.cpp)
  set_property(TARGET servo_demo PROPERTY CXX_STANDARD 11)
  target_link_libraries(servo_demo matrix_creator_hal)
  target_link_libraries(servo_demo ${CMAKE_THREAD_LIBS_INIT})

add_executable(gpio_demo gpio_demo.cpp)
  set_property(TARGET gpio_demo PROPERTY CXX_STANDARD 11)
  target_link_libraries(gpio_demo matrix_creator_hal)
  target_link_libraries(gpio_demo ${CMAKE_THREAD_LIBS_INIT})

add_executable(mic_energy mic_energy.cpp)
  set_property(TARGET mic_energy PROPERTY CXX_STANDARD 11)
  target_link_libraries(mic_energy matrix_creator_hal)
  target_link_libraries(mic_energy ${CMAKE_THREAD_LIBS_INIT})
  target_link_libraries(mic_energy ${WIRINGPI_LIB} ${WIRINGPI_DEV_LIB} ${CRYPT_LIB})

add_executable(micarray_alsa_pipes micarray_alsa_pipes.cpp)
  set_property(TARGET micarray_alsa_pipes PROPERTY CXX_STANDARD 11)
  target_link_libraries(micarray_alsa_pipes matrix_creator_hal)
  target_link_libraries(micarray_alsa_pipes ${CMAKE_THREAD_LIBS_INIT})
  target_link_libraries(micarray_alsa_pipes ${WIRINGPI_LIB} ${WIRINGPI_DEV_LIB} ${CRYPT_LIB})

add_executable(firmware_info firmware_info.cpp)
  set_property(TARGET firmware_info PROPERTY CXX_STANDARD 11)
  target_link_libraries(firmware_info matrix_creator_hal)
  target_link_libraries(firmware_info ${CMAKE_THREAD_LIBS_INIT})

add_executable(ipaddress_demo ipaddress_demo.cpp)
  set_property(TARGET ipaddress_demo PROPERTY CXX_STANDARD 11)
  target_link_libraries(ipaddress_demo matrix_creator_hal)
  target_link_libraries(ipaddress_demo ${CMAKE_THREAD_LIBS_INIT})

add_executable(sensors_status sensors_status.cpp)
  set_property(TARGET ipaddress_demo PROPERTY CXX_STANDARD 11)
  target_link_libraries(sensors_status matrix_creator_hal)
  target_link_libraries(sensors_status ${CMAKE_THREAD_LIBS_INIT})

add_executable(direction_of_arrival_demo direction_of_arrival_demo.cpp)
  set_property(TARGET direction_of_arrival_demo PROPERTY CXX_STANDARD 11)
  target_link_libraries(direction_of_arrival_demo matrix_creator_hal)
  target_link_libraries(direction_of_arrival_demo ${CMAKE_THREAD_LIBS_INIT})
  target_link_libraries(direction_of_arrival_demo ${WIRINGPI_LIB} ${WIRINGPI_DEV_LIB} ${CRYPT_LIB})
  target_link_libraries(direction_of_arrival_demo ${FFTW_LIBRARIES})

