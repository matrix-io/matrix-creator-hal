/*
 * Copyright 2016 <Admobilize>
 * All rights reserved.
 */

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <wiringPi.h>

#include <fstream>
#include <iostream>
#include <string>
#include <valarray>

#include "../cpp/driver/everloop.h"
#include "../cpp/driver/everloop_image.h"
#include "../cpp/driver/matrixio_bus.h"
#include "../cpp/driver/microphone_array.h"

namespace hal = matrix_hal;

int main() {
  hal::MatrixIOBus bus;
  if (!bus.Init()) return false;

  if(!bus.IsDirectBus()) {
    std::cerr << "Kernel Modules has been loaded. Use ALSA implementation " << std::endl;
    return false; 
  }

  hal::MicrophoneArray mics;
  mics.Setup(&bus);

  hal::Everloop everloop;
  everloop.Setup(&bus);

  hal::EverloopImage image1d(bus.MatrixLeds());

  for (auto &led : image1d.leds) led.red = 5;

  everloop.Write(&image1d);

  for (uint16_t c = 0; c < mics.Channels(); c++) {
    std::string name = "/tmp/matrix_micarray_channel_" + std::to_string(c);

    /* create the FIFO (named pipe) */
    if (mkfifo(name.c_str(), 0666) != 0) {
      /*std::cerr << "unable to create " << name << " FIFO." << std::endl;*/
    }
  }
  int named_pipe_handle;
  std::valarray<int16_t> buffer(mics.NumberOfSamples());
  while (true) {
    mics.Read(); /* Reading 8-mics buffer from de FPGA */
    for (uint16_t c = 0; c < mics.Channels(); c++) {
      std::string name = "/tmp/matrix_micarray_channel_" + std::to_string(c);
      named_pipe_handle = open(name.c_str(), O_WRONLY | O_NONBLOCK);

      for (uint32_t s = 0; s < mics.NumberOfSamples(); s++)
        buffer[s] = mics.At(s, c);

      write(named_pipe_handle, &buffer[0],
            sizeof(int16_t) * mics.NumberOfSamples());
      close(named_pipe_handle);
    }
  }

  return 0;
}
