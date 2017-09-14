/*
 * Copyright 2016 <Admobilize>
 * All rights reserved.
 */

#include <gflags/gflags.h>
#include <wiringPi.h>

#include <fstream>
#include <iostream>
#include <string>
#include <valarray>

#include "../cpp/driver/everloop.h"
#include "../cpp/driver/everloop_image.h"
#include "../cpp/driver/microphone_array.h"
#include "../cpp/driver/wishbone_bus.h"

DEFINE_bool(big_menu, true, "Include 'advanced' options in the menu listing");
DEFINE_int32(sampling_frequency, 16000, "Sampling Frequency");

namespace hal = matrix_hal;

int main(int argc, char *agrv[]) {

  google::ParseCommandLineFlags(&argc, &agrv, true);

  hal::WishboneBus bus;
  bus.SpiInit();

  hal::MicrophoneArray mics;
  mics.Setup(&bus);

  hal::Everloop everloop;
  everloop.Setup(&bus);

  hal::EverloopImage image1d;

  for (auto& led : image1d.leds) led.red = 10;

  everloop.Write(&image1d);

  int sampling_rate = FLAGS_sampling_frequency;
  mics.SetSamplingRate(sampling_rate);
  mics.ShowConfiguration();

  uint16_t seconds_to_record = 5;

  int16_t buffer[mics.Channels() + 1]
                [(seconds_to_record + 1) * mics.SamplingRate()];

  mics.CalculateDelays(0, 0, 1000, 320 * 1000);

  uint32_t step = 0;
  while (true) {
    mics.Read(); /* Reading 8-mics buffer from de FPGA */

    for (uint32_t s = 0; s < mics.NumberOfSamples(); s++) {
      for (uint16_t c = 0; c < mics.Channels(); c++) { /* mics.Channels()=8 */
        buffer[c][step] = mics.At(s, c);
      }
      buffer[mics.Channels()][step] = mics.Beam(s);
      step++;
    }

    if (step >= seconds_to_record * mics.SamplingRate()) break;
  }

  for (uint16_t c = 0; c < mics.Channels() + 1; c++) {
    std::string filename = "mic_" + std::to_string(mics.SamplingRate()) +
                           "_s16le_channel_" + std::to_string(c) + ".raw";
    std::ofstream os(filename, std::ofstream::binary);
    os.write((const char*)buffer[c],
             seconds_to_record * mics.SamplingRate() * sizeof(int16_t));

    os.close();
  }

  for (auto& led : image1d.leds) {
    led.red = 0;
    led.green = 10;
  }
  everloop.Write(&image1d);

  return 0;
}
