/*
 * Copyright 2016 <Admobilize>
 * All rights reserved.
 */

#include <wiringPi.h>

#include <string>
#include <iostream>
#include <valarray>
#include <unistd.h>

#include <matrix_hal/everloop_image.h>
#include <matrix_hal/everloop.h>
#include <matrix_hal/microphone_array.h>
#include <matrix_hal/wishbone_bus.h>

namespace hal = matrix_hal;

int main() {
  hal::WishboneBus bus;
  bus.SpiInit();

  hal::MicrophoneArray mics;
  mics.Setup(&bus);

  hal::Everloop everloop;
  everloop.Setup(&bus);

  hal::EverloopImage image1d;
  int j = 0;
  uint64_t instantE = 0;
  uint64_t avgEnergy = 0;
  std::valarray<uint64_t> localAverage (20);
  localAverage = 0;
  mics.SetGain(8); 
  while (true) {
    mics.Read(); /* Reading 8-mics buffer from de FPGA */
    instantE  = 0; 
    for (uint32_t s = 0; s < mics.NumberOfSamples(); s++) {
      instantE = instantE + (mics.At(s, 0))*(mics.At(s, 0));
    }
    
    localAverage[j%20] = instantE;
    avgEnergy = 0;
    for(auto& data : localAverage){
      avgEnergy = (avgEnergy + data);
    }
    
    avgEnergy = avgEnergy/20;
    
    for (auto& led : image1d.leds) {
      led.red = avgEnergy>>24;
    }
    everloop.Write(&image1d);
    
    j++;

  }
  
  return 0;
}
