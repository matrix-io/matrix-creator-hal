/*
 * Copyright 2016 <Admobilize>
 * All rights reserved.
 */
#include <gflags/gflags.h>
#include <wiringPi.h>

#include <cmath>
#include <iostream>
#include <string>
#include <valarray>

#include "../cpp/driver/everloop.h"
#include "../cpp/driver/everloop_image.h"
#include "../cpp/driver/matrixio_bus.h"
#include "../cpp/driver/microphone_array.h"
#include "../cpp/driver/microphone_core.h"
#include "./fir.h"

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

  hal::Everloop everloop;
  everloop.Setup(&bus);

  hal::MicrophoneArray mics;
  mics.Setup(&bus);
  mics.SetSamplingRate(sampling_rate);
  mics.ShowConfiguration();

  // Microphone Core Init
  hal::MicrophoneCore mic_core(mics);
  mic_core.Setup(&bus);

  hal::EverloopImage image1d(bus.MatrixLeds());

  std::valarray<int> lookup(mics.Channels());
  if (bus.MatrixName() == matrix_hal::kMatrixCreator)
    lookup = {23, 27, 32, 1, 6, 10, 14, 19};
  else if (bus.MatrixName() == matrix_hal::kMatrixVoice)
    lookup = {4, 0, 2, 5, 7, 10, 13, 15};
  else
    return false;

  std::valarray<float> magnitude(mics.Channels());

  std::valarray<float> num_coeff = {
      0.000193193060168428, 0.000206822702747802, 0.000252595665690168,
      0.000256872357101376, 0.000229153460022504, 0.000211739243318005,
      0.000275943118065930, 0.000498638500969369, 0.000923786453235222,
      0.00151735239302190,  0.00213284064302757,  0.00250760733086885,
      0.00230322934522679,  0.00119491994351560,  -0.00100520978429762,
      -0.00422019569760432, -0.00801990911085965, -0.0116138510263798,
      -0.0139551939167222,  -0.0139563603852271,  -0.0107836646022430,
      -0.00416381454722954, 0.00538363033494006,  0.0164728109237464,
      0.0269935452443335,   0.0344757150173910,   0.0366218467353529,
      0.0318976192566281,   0.0200358566524419,   0.00230996051438552,
      -0.0185266778939888,  -0.0386654616273612,  -0.0539709003778071,
      -0.0608679350554966,  -0.0571886498651163,  -0.0427712111876776,
      -0.0196483703816303,  0.00824724162245257,  0.0358204111367295,
      0.0578430600514709,   0.0700498238179111,   0.0700498238179111,
      0.0578430600514709,   0.0358204111367295,   0.00824724162245257,
      -0.0196483703816303,  -0.0427712111876776,  -0.0571886498651163,
      -0.0608679350554966,  -0.0539709003778071,  -0.0386654616273612,
      -0.0185266778939888,  0.00230996051438552,  0.0200358566524419,
      0.0318976192566281,   0.0366218467353529,   0.0344757150173910,
      0.0269935452443335,   0.0164728109237464,   0.00538363033494006,
      -0.00416381454722954, -0.0107836646022430,  -0.0139563603852271,
      -0.0139551939167222,  -0.0116138510263798,  -0.00801990911085965,
      -0.00422019569760432, -0.00100520978429762, 0.00119491994351560,
      0.00230322934522679,  0.00250760733086885,  0.00213284064302757,
      0.00151735239302190,  0.000923786453235222, 0.000498638500969369,
      0.000275943118065930, 0.000211739243318005, 0.000229153460022504,
      0.000256872357101376, 0.000252595665690168, 0.000206822702747802,
      0.000193193060168428};

  std::valarray<FIR> filter_bandpass(mics.Channels());

  for (auto &fir : filter_bandpass) fir.Setup(num_coeff);

  while (true) {
    mics.Read();
    magnitude = 0.0;
    for (unsigned int s = 0; s < mics.NumberOfSamples(); s++) {
      for (unsigned int c = 0; c < mics.Channels(); c++) {
        float x = filter_bandpass[c].Filter(mics.At(s, c));
        magnitude[c] += (x * x);
      }
    }

    for (auto &m : magnitude) {
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
