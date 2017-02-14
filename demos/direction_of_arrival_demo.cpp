/*
 * Copyright 2016 <Admobilize>
 * All rights reserved.
 */

#include <fftw3.h>
#include <stdint.h>
#include <string.h>
#include <wiringPi.h>

#include <fstream>
#include <iostream>
#include <string>
#include <valarray>

#include "../cpp/driver/direction_of_arrival.h"
#include "../cpp/driver/everloop.h"
#include "../cpp/driver/everloop_image.h"
#include "../cpp/driver/microphone_array.h"
#include "../cpp/driver/wishbone_bus.h"

namespace hal = matrix_hal;

const int N = 128;

int main() {
  hal::WishboneBus bus;
  bus.SpiInit();

  hal::MicrophoneArray mics;
  mics.Setup(&bus);

  hal::Everloop everloop;
  everloop.Setup(&bus);

  hal::EverloopImage image1d;

  for (auto& led : image1d.leds) led.red = 10;

  everloop.Write(&image1d);

  hal::DirectionOfArrival doa(mics);

  while (true) {
    mics.Read(); /* Reading 8-mics buffer from de FPGA */

    doa.Calculate();

    std::cout << "azimutal angle = " << doa.GetAzimutalAngle()*180/M_PI << std::endl;
    std::cout << "polar angle = " << doa.GetPolarAngle()*180/M_PI << std::endl;
    std::cout << "mic = " << doa.GetNearestMicrophone() << std::endl
              << std::endl;
  }

  return 0;
}
