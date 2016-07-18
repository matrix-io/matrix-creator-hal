/*
 * Copyright 2016 <Admobilize>
 * All rights reserved.
 */

#include <wiringPi.h>

#include <string>
#include <cmath>
#include <iostream>
#include <valarray>

#include "../cpp/driver/microphone_array.h"
#include "../cpp/driver/everloop_image.h"
#include "../cpp/driver/everloop.h"
#include "../cpp/driver/wishbone_bus.h"

namespace hal = matrix_hal;

int main() {
  hal::WishboneBus* bus = new hal::WishboneBus();
  bus->SpiInit();

  hal::Everloop everloop;
  everloop.Setup(bus);

  hal::MicrophoneArray mics;
  mics.Setup(bus);

  hal::EverloopImage image1d;

  std::valarray<int> lookup = {23, 27, 32, 1, 6, 10, 14, 19};

  std::valarray<float> magnitude(hal::kChannels);

  while (true) {
    mics.Read();
    magnitude = 0.0;
    for (unsigned int s = 0; s < mics.NumberOfSamples(); s++) {
      for (unsigned int c = 0; c < hal::kChannels; c++) {
        magnitude[c] += mics.At(s, c) * mics.At(s, c);
      }
    }

    for (auto& m : magnitude) {
      m = std::sqrt(1.0 / (float)mics.NumberOfSamples() * m);
    }

    for (unsigned int c = 0; c < hal::kChannels; c++) {
      image1d.leds[lookup[c]].red = magnitude[c] / 255;
      std::cout << image1d.leds[lookup[c]].red << "\t";
    }
    std::cout << std::endl;

    everloop.Write(&image1d);
  }

  return 0;
}
