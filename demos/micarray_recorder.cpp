/*
 * Copyright 2017 <Admobilize>
 * All rights reserved.
 */

#include <gflags/gflags.h>
#include <wiringPi.h>

#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <valarray>

#include "../cpp/driver/everloop.h"
#include "../cpp/driver/everloop_image.h"
#include "../cpp/driver/microphone_array.h"
#include "../cpp/driver/wishbone_bus.h"

DEFINE_int32(sampling_frequency, 16000, "Sampling Frequency");
DEFINE_int32(duration, 5, "Interrupt after N seconds");
DEFINE_int32(gain, -1, "Microphone Gain");

namespace hal = matrix_hal;

int main(int argc, char *agrv[]) {
  google::ParseCommandLineFlags(&argc, &agrv, true);

  hal::WishboneBus bus;
  bus.SpiInit();

  hal::MicrophoneArray mics;
  mics.Setup(&bus);

  int sampling_rate = FLAGS_sampling_frequency;
  int seconds_to_record = FLAGS_duration;

  mics.SetSamplingRate(sampling_rate);

  if (FLAGS_gain > 0) mics.SetGain(FLAGS_gain);

  mics.ReadConfValues();
  mics.ShowConfiguration();

#if 1
  std::cout << "Duration : " << seconds_to_record << "s" << std::endl;

  int16_t buffer[mics.Channels() + 1]
                [mics.SamplingRate() + mics.NumberOfSamples()];

  mics.CalculateDelays(0, 0, 1000, 320 * 1000);

  std::ofstream os[mics.Channels() + 1];

  for (uint16_t c = 0; c < mics.Channels() + 1; c++) {
    std::string filename = "mic_" + std::to_string(mics.SamplingRate()) +
                           "_s16le_channel_" + std::to_string(c) + ".raw";
    os[c].open(filename, std::ofstream::binary);
  }

  std::thread et(
      [seconds_to_record](hal::WishboneBus *bus) {

        hal::Everloop everloop;
        everloop.Setup(bus);

        hal::EverloopImage image;

        for (auto &led : image.leds) led.red = 10;
        everloop.Write(&image);

        int sleep = int(1000.0 * seconds_to_record / image.leds.size());

        for (auto &led : image.leds) {
          led.red = 0;
          led.green = 10;

          everloop.Write(&image);

          std::this_thread::sleep_for(std::chrono::milliseconds(sleep));
        }
      },
      &bus);

  uint32_t samples = 0;
  for (int s = 0; s < seconds_to_record; s++) {
    for (;;) {
      mics.Read(); /* Reading 8-mics buffer from de FPGA */

      /* buffering */
      for (uint32_t s = 0; s < mics.NumberOfSamples(); s++) {
        for (uint16_t c = 0; c < mics.Channels(); c++) { /* mics.Channels()=8 */
          buffer[c][samples] = mics.At(s, c);
        }
        buffer[mics.Channels()][samples] = mics.Beam(s);
        samples++;
      }

      /* write to file */
      if (samples >= mics.SamplingRate()) {
        for (uint16_t c = 0; c < mics.Channels() + 1; c++) {
          os[c].write((const char *)buffer[c], samples * sizeof(int16_t));
        }
        samples = 0;
        break;
      }
    }
  }

  et.join();
#endif
  return 0;
}
