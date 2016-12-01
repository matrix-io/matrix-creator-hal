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

#include <unistd.h>
#include <ctime>
#include <iostream>
#include <chrono>
#include <sstream>

#include "../cpp/driver/everloop.h"
#include "../cpp/driver/everloop_image.h"
#include "../cpp/driver/gpio_control.h"
#include "../cpp/driver/wishbone_bus.h"

#define OUTPUT 1
#define SPEAKER 15

namespace hal = matrix_hal;

void clear(hal::EverloopImage *img) {
  for (hal::LedValue &led : img->leds) {
    led.red = 0;
    led.green = 0;
    led.blue = 0;
    led.white = 0;
  }
}

void setColor(hal::EverloopImage *img, int pos, int r, int g, int b, int w) {
  img->leds[pos % 35].red = r;
  img->leds[pos % 35].green = g;
  img->leds[pos % 35].blue = b;
  img->leds[pos % 35].white = w;
}

void drawProgress(hal::EverloopImage *img, unsigned counter) {
  int min = counter % 35;
  for (int y = 0; y <= min; y++) {
    if (y > 30) {
      setColor(img, y, 40, 10, 0, 0);
    } else {
      setColor(img, y, 10, 40, 0, 0);
    }
  }
}

void drawSeconds(hal::EverloopImage *img, unsigned seconds) {
  int sec = seconds % 35;
  setColor(img, sec, 0, 0, 40, 0);
}

int main(int argc, char *argv[]) {
  hal::WishboneBus bus;
  bus.SpiInit();

  hal::GPIOControl gpio;
  gpio.Setup(&bus);
  gpio.SetMode(SPEAKER, OUTPUT);

  hal::Everloop everloop;
  hal::EverloopImage img;

  everloop.Setup(&bus);

  unsigned counter = 0;
  unsigned seconds = 0;
  uint16_t speaker = 0;
  unsigned tick = 0;
  unsigned ticks = 15;
  bool start_alarm = false;

  float time = 180.0 / 35.0;
  if (argc == 2) {
    std::istringstream iss(argv[1]);
    int val;
    if (iss >> val) {
      std::cout << "time set to: " << argv[1] << " secs.\n";
      time = (float)val / 35.0;
      std::cout << "led time: " << time << "\n";
    }
  }

  clear(&img);

  gpio.SetGPIOValue(SPEAKER, 0);
  std::chrono::time_point<std::chrono::system_clock> smin, now, ssec, sbeep;
  smin = std::chrono::system_clock::now();
  ssec = smin;
  sbeep = smin;

  setColor(&img, 0, 10, 40, 0, 0);

  while (1) {
    now = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_minutes = now - smin;
    std::chrono::duration<double> elapsed_seconds = now - ssec;
    std::chrono::duration<double> elapsed_beep = now - sbeep;

    if (elapsed_minutes.count() > time) {
      smin = std::chrono::system_clock::now();
      ++counter;
    }

    if (elapsed_seconds.count() > time / 35.) {
      ssec = std::chrono::system_clock::now();
      ++seconds;
    }

    if (elapsed_beep.count() > 0.03) {
      sbeep = std::chrono::system_clock::now();
      speaker = ~speaker;
      tick++;
    }

    if ((counter % 35) == 0) {
      start_alarm = true;
    }

    if (start_alarm && tick < ticks) {
      gpio.SetGPIOValue(SPEAKER, speaker);
    } else {
      gpio.SetGPIOValue(SPEAKER, 0);
    }

    if ((counter % 35) == 34) {
      start_alarm = false;
      tick = 0;
    }

    if (start_alarm && tick < ticks) {
      if(speaker){
        clear(&img);
      }
      else{
        drawProgress(&img, counter);
        drawSeconds(&img, seconds);
      }
    }
    else {
      clear(&img);
      drawProgress(&img, counter);
      drawSeconds(&img, seconds);
    }
    everloop.Write(&img);
      
  }

  return 0;
}
