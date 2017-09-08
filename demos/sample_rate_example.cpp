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
  
  std::cout << str << std:endl;


  hal::MicrophoneArray mics;
  mics.Setup(&bus);

  uint16_t sample_rate;
  std::cout << "Set MicrophoneArray sample rate value: "; 
  std::cin >> sample_rate;
  std::cout << std::endl << "Sample rate value: " << sample_rate << std::endl;
  
  mics.SetSampleFrequency(sample_rate);
  mics.GetSampleFrequency();
  std::cout << "MicrophoneArray sample rate value: " << mics.SamplingRate() << std::endl;

  return 0;

}
