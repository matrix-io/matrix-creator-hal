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
 
  uint16_t data_gain;
  std::cout << "Set MicrophoneArray data_gain value: ";
  std::cin >> data_gain;
  std::cout << std::endl << "Sample data gain: " << data_gain << std::endl << std::endl;
  mics.SetGain(data_gain);
  
  uint16_t sample_rate;
  std::cout << "Set MicrophoneArray sample rate value: "; 
  std::cin >> sample_rate;
  std::cout << std::endl << "Sample rate value: " << sample_rate << std::endl;
  mics.SetDecimationRatio(sample_rate);

  
  return 0;

}
