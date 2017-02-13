/*
 * Copyright 2016 <Admobilize>
 * All rights reserved.
 */

#include <wiringPi.h>
#include <string.h>
#include <fftw3.h>
#include <stdint.h>

#include <string>
#include <fstream>
#include <iostream>
#include <valarray>

#include "../cpp/driver/everloop_image.h"
#include "../cpp/driver/everloop.h"
#include "../cpp/driver/microphone_array.h"
#include "../cpp/driver/wishbone_bus.h"
#include "../cpp/driver/direction_of_arrival.h"

namespace hal = matrix_hal;

const int N = 128;

int main() {
  hal::WishboneBus bus;
  bus.SpiInit();

  hal::MicrophoneArray mics;
  mics.Setup(&bus);

  hal::Everloop everloop;
  everloop.Setup(&bus);

  hal::EverloopImage image1d;

  for (auto& led : image1d.leds) led.red = 10;

  everloop.Write(&image1d);

  int16_t buffer_1D[mics.Channels()*mics.SamplingRate()];
  int16_t* buffer_2D[mics.Channels()];

  for (uint16_t c = 0; c < mics.Channels(); c++)
  {
     buffer_2D[c] = &buffer_1D[c*mics.SamplingRate()];  
  }

  hal::DirectionOfArrival DoA(N);

  while (true) {
    mics.Read(); /* Reading 8-mics buffer from de FPGA */

    for (uint32_t s = 0; s < mics.NumberOfSamples(); s++) {
      for (uint16_t c = 0; c < mics.Channels(); c++) { /* mics.Channels()=8 */
        buffer_2D[c][s] = mics.At(s, c);
      }
    }

    DoA.Calculate(buffer_2D);

  }

  return 0;
}
