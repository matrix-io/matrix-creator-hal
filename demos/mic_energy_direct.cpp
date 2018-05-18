/*
 * Copyright 2016 <Admobilize>
 * All rights reserved.
 */
#include <gflags/gflags.h>
#include <wiringPi.h>

#include <unistd.h>
#include <iostream>
#include <string>
#include <valarray>

#include "../cpp/driver/everloop.h"
#include "../cpp/driver/everloop_image.h"
#include "../cpp/driver/matrixio_bus.h"
#include "../cpp/driver/microphone_array.h"
#include "../cpp/driver/microphone_core.h"

DEFINE_bool(big_menu, true, "Include 'advanced' options in the menu listing");
DEFINE_int32(sampling_frequency, 16000, "Sampling Frequency");

namespace hal = matrix_hal;

int main(int argc, char *agrv[]) {
  google::ParseCommandLineFlags(&argc, &agrv, true);

  hal::MatrixIOBus bus;
  if (!bus.Init()) return false;

  if (!bus.IsDirectBus()) {
    std::cerr << "Kernel Modules has been loaded. Use ALSA examples "
              << std::endl;
  }

  int sampling_rate = FLAGS_sampling_frequency;

  hal::MicrophoneArray mics;
  mics.Setup(&bus);
  mics.SetSamplingRate(sampling_rate);
  mics.ShowConfiguration();

  // Microphone Core Init
  hal::MicrophoneCore mic_core(mics);
  mic_core.Setup(&bus);

  hal::Everloop everloop;
  everloop.Setup(&bus);

  hal::EverloopImage image1d(bus.MatrixLeds());
  int j = 0;
  uint64_t instantE = 0;
  uint64_t avgEnergy = 0;
  std::valarray<uint64_t> localAverage(20);
  localAverage = 0;

  while (true) {
    mics.Read(); /* Reading 8-mics buffer from de FPGA */
    instantE = 0;
    for (uint32_t s = 0; s < mics.NumberOfSamples(); s++) {
      instantE = instantE + (mics.At(s, 0)) * (mics.At(s, 0));
    }

    localAverage[j % 20] = instantE;
    avgEnergy = 0;
    for (auto &data : localAverage) {
      avgEnergy = (avgEnergy + data);
    }

    avgEnergy = avgEnergy / 20;

    for (auto &led : image1d.leds) {
      led.red = avgEnergy >> 24;
    }
    everloop.Write(&image1d);

    j++;
  }

  return 0;
}
