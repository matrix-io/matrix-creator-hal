project(matrix_creator_hal C CXX)
cmake_minimum_required(VERSION 2.8)

find_library (FFTW_LIBRARIES NAMES fftw3f )

add_compile_options(-std=c++11)

# Enable extra warnings. Not needed.
include (../../cmake/FatalWarnings.cmake)
ADM_EXTRA_WARNINGS()

find_package(Threads)

include_directories(
  ../..
)

set(matrix_creator_hal_src
  dummy_sensor.cpp
  everloop.cpp
  gpio_control.cpp
  humidity_sensor.cpp
  imu_sensor.cpp
  matrix_driver.cpp
  mcu_firmware.cpp
  microphone_array.cpp
  pressure_sensor.cpp
  uv_sensor.cpp
  wishbone_bus.cpp
  cross_correlation.cpp
  direction_of_arrival.cpp
)

add_library(matrix_creator_hal ${matrix_creator_hal_src})
  set_property(TARGET matrix_creator_hal PROPERTY CXX_STANDARD 11)
  target_link_libraries(matrix_creator_hal ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_THREAD_LIBS_INIT})
  target_link_libraries(matrix_creator_hal ${FFTW_LIBRARIES})

set (matrix_creator_hal_headers
  circular_queue.h
  creator_memory_map.h  
  dummy_data.h          
  dummy_sensor.h        
  everloop.h        
  everloop_image.h  
  fw_data.h          
  gpio_control.h    
  humidity_data.h    
  humidity_sensor.h  
  imu_data.h         
  imu_sensor.h        
  matrix_driver.h     
  mcu_firmware.h
  microphone_array.h  
  microphone_array_location.h
  pressure_data.h
  pressure_sensor.h
  uv_data.h             
  uv_sensor.h       
  wishbone_bus.h
  cross_correlation.h
  direction_of_arrival.h
)

install (TARGETS matrix_creator_hal DESTINATION lib)

install (FILES ${matrix_creator_hal_headers} DESTINATION include/matrix_hal)
