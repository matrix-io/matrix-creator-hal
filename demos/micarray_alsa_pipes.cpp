/*
 * Copyright 2016 <Admobilize>
 * All rights reserved.
 */

#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <wiringPi.h>

#include <string>
#include <fstream>
#include <iostream>
#include <valarray>

#include "../cpp/driver/everloop_image.h"
#include "../cpp/driver/everloop.h"
#include "../cpp/driver/microphone_array.h"
#include "../cpp/driver/wishbone_bus.h"

namespace hal = matrix_hal;

int main() {
  hal::WishboneBus bus;
  bus.SpiInit();

  hal::MicrophoneArray mics;
  mics.Setup(&bus);

  hal::Everloop everloop;
  everloop.Setup(&bus);

  hal::EverloopImage image1d;

  for (auto& led : image1d.leds) led.red = 5;

  everloop.Write(&image1d);

  std::valarray<int> named_pipes_handle(mics.Channels());

  for (uint16_t c = 0; c < mics.Channels(); c++) {
    std::string name = "/tmp/matrix_micarray_channel_" + std::to_string(c);

    /* create the FIFO (named pipe) */
    if (mkfifo(name.c_str(), 0666) != 0) {
      std::cerr << "unable to create " << name << " FIFO." << std::endl;
      return -1;
    }

    named_pipes_handle[c] = open(name.c_str(), O_WRONLY | O_NONBLOCK);
  }

  while (true) {
    mics.Read(); /* Reading 8-mics buffer from de FPGA */

    for (uint32_t s = 0; s < mics.NumberOfSamples(); s++) {
      for (uint16_t c = 0; c < mics.Channels(); c++) { /* mics.Channels()=8 */
        write(named_pipes_handle[c], &mics.At(s, c), sizeof(int16_t));
      }
    }
  }

  return 0;
}
