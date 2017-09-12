/*
 * Copyright 2016 <Admobilize>
 * All rights reserved.
 */

#include <wiringPi.h>

#include <string>
#include <fstream>
#include <iostream>
#include <valarray>

#include "../cpp/driver/everloop_image.h"
#include "../cpp/driver/everloop.h"
#include "../cpp/driver/microphone_array.h"
#include "../cpp/driver/wishbone_bus.h"

namespace hal = matrix_hal;

int main() {
  hal::WishboneBus bus;
  bus.SpiInit();

  char version[8];

  bus.GetSoftwareVersion(version,sizeof(version));
  
  std::string str;
  str.assign(version,8);
  
  std::cout << str << std::endl;
  
  hal::MicrophoneArray mics;
  mics.Setup(&bus);

  mics.ShowConfiguration();
 
  uint32_t sampling_rate;
  std::cout << "Set MicrophoneArray sample rate value: ";
  std::cin >> sampling_rate;
  mics.SetSamplingRate(sampling_rate);
  
  mics.ShowConfiguration();

  return 0;

}
