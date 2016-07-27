/*
 * Copyright 2016 <Admobilize>
 * All rights reserved.
 */

#include <wiringPi.h>

#include <string>
#include <fstream>
#include <iostream>
#include <valarray>

#include "../cpp/driver/microphone_array.h"
#include "../cpp/driver/wishbone_bus.h"

namespace hal = matrix_hal;

int main() {
  hal::WishboneBus* bus = new hal::WishboneBus();
  bus->SpiInit();

  hal::MicrophoneArray mics;
  mics.Setup(bus);

  uint16_t seconds_to_record = 10;

  int16_t buffer[mics.Channels()][seconds_to_record * mics.SamplingRate()];

  uint32_t step = 0;
  while (true) {
    mics.Read(); /* Reading 8-mics buffer from de FPGA */

    for (uint32_t s = 0; s < mics.NumberOfSamples(); s++) {
      for (uint16_t c = 0; c < mics.Channels(); c++) { /* mics.Channels()=8 */
        buffer[c][step] = mics.At(s, c);
      }
      step++;
    }
    if (step == seconds_to_record * mics.SamplingRate()) break;
  }

  for (uint16_t c = 0; c < mics.Channels(); c++) {
    std::string filename = "mic_" + std::to_string(mics.SamplingRate()) +
                           "_s16le_channel_" + std::to_string(c) + ".raw";
    std::ofstream os(filename, std::ofstream::binary);
    os.write((const char*)buffer[c],
             seconds_to_record * mics.SamplingRate() * sizeof(int16_t));

    os.close();
  }

  return 0;
}
