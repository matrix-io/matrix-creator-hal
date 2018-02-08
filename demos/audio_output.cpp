#include <gflags/gflags.h>
#include <wiringPi.h>

#include <math.h>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <valarray>

#include "../cpp/driver/audio_output.h"
#include "../cpp/driver/wishbone_bus.h"

DEFINE_int32(sampling_frequency, 44100, "Sampling Frequency");
DEFINE_string(raw_file, "song.raw", "Raw file song");
DEFINE_int32(volumen, 50, "Volumen in %");

namespace hal = matrix_hal;

int main(int argc, char *agrv[]) {
  google::ParseCommandLineFlags(&argc, &agrv, true);

  hal::WishboneBus bus;
  bus.SpiInit();

  int sampling_rate = FLAGS_sampling_frequency;
  std::ifstream is(FLAGS_raw_file);

  hal::AudioOutput dac;
  dac.Setup(&bus);
  dac.SetVolumen(FLAGS_volumen);
  dac.SetOutputSelector(matrix_hal::kSpeaker);
  while (true) {
    is.read(reinterpret_cast<char *>(&dac.write_data_[0]),
            sizeof(uint16_t) * (matrix_hal::kMaxWriteLength));
    dac.Write();
  }
  return 0;
}
