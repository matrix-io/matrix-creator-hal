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
#include <string>

#include "../cpp/driver/audio_output.h"
#include "../cpp/driver/matrixio_bus.h"

DEFINE_int32(
    sampling_frequency, -1,
    "Sampling PCM Frequency: 8000,16000,32000,44100,48000,88200,96000");
DEFINE_string(output, "", "Output Selector h=Headphone s=Speaker");
DEFINE_int32(volumen, -1, "Volumen in %");

namespace hal = matrix_hal;

int main(int argc, char *agrv[]) {
  google::SetUsageMessage(
      "\n\nFlags Usage:\n\n"
      "-output (Output Selector h=Headphone s=Speaker) \n"
      "  type: string\n"
      "-raw_file (Raw audio file ) \n"
      "  type: string \n"
      "-sampling_frequency \n"
      "  sampling PCM frequency: 8000,16000,32000,44100,48000,88200,96000)\n"
      "   type: int32\n"
      "-volumen (Volumen in %) \n"
      "   type: int32\n");
  google::ParseCommandLineFlags(&argc, &agrv, true);

  hal::MatrixIOBus bus;
  if (!bus.Init()) return false;

  hal::AudioOutput dac;
  dac.Setup(&bus);

  std::cout << std::endl;

  if (FLAGS_volumen != -1){
    dac.SetVolumen(FLAGS_volumen);
    std::cout << "\nVolume set to: " << FLAGS_volumen << "%" << std::endl;
  }

  if (FLAGS_sampling_frequency != -1) {
    if (dac.SetPCMSamplingFrequency(FLAGS_sampling_frequency)) {
      std::cout << "Frequency selected: " << FLAGS_sampling_frequency
                << std::endl;
    } else {
      std::cout
          << "Invalid Selection. Please Select a supported PCM Frequency"
          << std::endl
          << "PCM Frequencies supported :" << std::endl
          << "8000, 16000, 32000, 44100, 48000, 88200, 96000." << std::endl;
    }
  }

  switch (FLAGS_output[0]) {
    case 'h':
    case 'H':
      if (dac.SetOutputSelector(matrix_hal::kHeadPhone)) {
        std::cout << "Audio Output Selected: "
                  << "3.5mm Headphone Jack Selected."
                  << std::endl;
      }
      break;
    case 's':
    case 'S':
      if (dac.SetOutputSelector(matrix_hal::kSpeaker)) {
        std::cout << "Audio Output Selected: 3W Stereo Speaker Connectors "
                  << std::endl;
      }
      break;
    default:
      std::cout << "Invalid Selection for Audio Output Selection. "
                << "Please try again" << std::endl;
       return 1;
  }

  return 0;
}
