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
#include "../cpp/driver/cross_correlation.h"

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

  int16_t buffer[mics.Channels()][mics.SamplingRate()];

  hal::CrossCorrelation corr(N);
  std::valarray<float> current_mag(4);
  std::valarray<float> current_index(4);

  while (true) {
    mics.Read(); /* Reading 8-mics buffer from de FPGA */

    for (uint32_t s = 0; s < mics.NumberOfSamples(); s++) {
      for (uint16_t c = 0; c < mics.Channels(); c++) { /* mics.Channels()=8 */
        buffer[c][s] = mics.At(s, c);
      }
    }

    for (int channel = 0; channel < 4; channel++) {
      corr.Exec(buffer[channel], buffer[channel + 4]);

      float* c = corr.Result();

      int index = 0;
      float m = c[0];
      int max_tof = 6;
      for (int i = 1; i < max_tof; i++)
        if (c[i] > m) {
          index = i;
          m = c[i];
        }

      for (int i = N - max_tof; i < N; i++)
        if (c[i] > m) {
          index = i;
          m = c[i];
        }

      current_mag[channel] = m;
      current_index[channel] = index;
    }

    int dir = 0;
    int index = current_index[0];
    float mag = current_mag[0];
    for (int channel = 1; channel < 4; channel++) {
      if (mag < current_mag[channel]) {
        dir = channel;
        mag = current_mag[channel];
        index = current_index[channel];
      }
    }

    if (index > 64) index = -(128 - index);

    if (mag > 2e8)
      std::cout << dir << "\t" << index << "\t" << mag << std::endl;
  }

  return 0;
}
