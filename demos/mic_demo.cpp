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

  std::valarray<float> magnitude(mics.Channels());

  std::valarray<float> coeff_hp = {
   0.0599851024734,-1.300381417101e-17,  -0.1549721713331,  -0.1626987043005,
     0.1053874898562,   0.2920599418361,   0.1053874898562,  -0.1626987043005,
    -0.1549721713331,-1.300381417101e-17,   0.0599851024734
  };
  std::valarray<FIR> filter_bank_hp(mics.Channels());
  for (auto& iir : filter_bank_hp) iir.Setup(coeff_hp);

  std::valarray<float> coeff_lp = {
      1.5414364192065861E-003, 2.3293447964816889E-003, 4.5521160193679406E-003,
      7.8514472469795749E-003, 1.1667089756749511E-002, 1.5336748122750850E-002,
      1.8215960726324591E-002, 1.9795701289425597E-002, 1.9795701289425597E-002,
      1.8215960726324591E-002, 1.5336748122750850E-002, 1.1667089756749511E-002,
      7.8514472469795749E-003, 4.5521160193679406E-003, 2.3293447964816889E-003,
      1.5414364192065861E-003};
  std::valarray<FIR> filter_bank_lp(mics.Channels());
  for (auto& iir : filter_bank_lp) iir.Setup(coeff_lp);

  while (true) {
    mics.Read();
    magnitude = 0.0;
    for (unsigned int s = 0; s < mics.NumberOfSamples(); s++) {
      for (unsigned int c = 0; c < mics.Channels(); c++) {
        float x = filter_bank_hp[c].Filter(mics.At(s, c));
        magnitude[c] += filter_bank_lp[c].Filter(x * x);
      }
    }

    for (auto& m : magnitude) {
      m = std::sqrt(1.0 / (float)mics.NumberOfSamples() * m);
    }

    for (unsigned int c = 0; c < mics.Channels(); c++) {
      image1d.leds[lookup[c]].green = magnitude[c] / 512;
      std::cout << image1d.leds[lookup[c]].green << "\t";
    }
    std::cout << std::endl;

    everloop.Write(&image1d);
  }

  return 0;
}
