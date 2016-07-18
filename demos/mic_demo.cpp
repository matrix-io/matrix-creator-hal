/*
 * Copyright 2016 <Admobilize>
 * All rights reserved.
 */

#include <wiringPi.h>

#include <string>
#include <iostream>
#include <valarray>

#include "../cpp/driver/microphone_array.h"
#include "../cpp/driver/everloop_image.h"
#include "../cpp/driver/everloop.h"
#include "../cpp/driver/wishbone_bus.h"
#include "./fir.h"

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

  std::valarray<float> coeff = {
      1.5999578900203774E-004, 1.0799928938988583E-003, 2.0000000000000000E-003,
      1.0799928938988583E-003, 1.5999578900203774E-004};

  std::valarray<FIR> fir_array(hal::kChannels);
  std::valarray<float> magnitude(hal::kChannels);

  for (auto& fir : fir_array) fir.Setup(coeff);

  while (true) {
    mics.Read();

    for (unsigned int s = 0; s < mics.NumberOfSamples(); s++) {
      for (unsigned int c = 0; c < hal::kChannels; c++) {
        magnitude[c] = fir_array[c].Filter(
            static_cast<float>(mics.At(s, c) * mics.At(s, c)));
      }
    }

    for (unsigned int c = 0; c < hal::kChannels; c++) {
      image1d.leds[lookup[c]].red = magnitude[c] / 1000;
    }

    everloop.Write(&image1d);
  }

  return 0;
}
