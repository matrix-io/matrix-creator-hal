/*
 * Copyright 2016 <Admobilize>
 * MATRIX Labs  [http://creator.matrix.one]
 * This file is part of MATRIX Creator HAL
 *
 * MATRIX Creator HAL is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <gflags/gflags.h>
#include <fstream>
#include <iostream>
#include <valarray>

#include "../cpp/driver/audio_output.h"
#include "../cpp/driver/matrixio_bus.h"

DEFINE_int32(
    sampling_frequency, 44100,
    "Sampling PCM Frequency: 8000,16000,32000,44100,48000,88200,96000");
DEFINE_string(output, "h", "Output Selector h=Headphone s=Speaker");
DEFINE_string(raw_file, "song.raw", "Raw file song");
DEFINE_int32(volumen, 50, "Volumen in %");

namespace hal = matrix_hal;

int main(int argc, char *agrv[]) {
  google::ParseCommandLineFlags(&argc, &agrv, true);

  hal::MatrixIOBus bus;
  if (!bus.Init()) return false;

  std::ifstream is(FLAGS_raw_file);

  hal::AudioOutput dac;
  dac.Setup(&bus);
  dac.SetVolumen(FLAGS_volumen);

  if (!(dac.SetPCMSamplingFrequency(FLAGS_sampling_frequency))) {
    std::cout << "Invalid Selection. Please Select a supported PCM Frequency"
              << std::endl;
    return false;
  }

  switch (FLAGS_output[0]) {
    case 'h':
    case 'H':
      dac.SetOutputSelector(matrix_hal::kHeadPhone);
      break;
    case 's':
    case 'S':
      dac.SetOutputSelector(matrix_hal::kSpeaker);
      break;
    default:
      std::cout << "Invalid Selection. Please try again" << std::endl;
      return 1;
  }

  while (true) {
    is.read(reinterpret_cast<char *>(&dac.write_data_[0]),
            sizeof(uint16_t) * (matrix_hal::kMaxWriteLength));
    dac.Write();
  }
  return 0;
}
