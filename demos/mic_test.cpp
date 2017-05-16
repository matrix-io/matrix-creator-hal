/*
 * Copyright 2016 <Admobilize>
 * All rights reserved.
 */

#include <wiringPi.h>

#include <string>
#include <cmath>
#include <iostream>
#include <valarray>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <iomanip>
#include <algorithm>    // std::sort
#include <vector>       // std::vector

#include "matrix_hal/microphone_array.h"
#include "matrix_hal/everloop_image.h"
#include "matrix_hal/everloop.h"
#include "matrix_hal/wishbone_bus.h"
#include "./fir.h"

namespace hal = matrix_hal;

float median(float channels[], int size){
  std::vector<float> amps (channels,channels + size );  
  std::sort(amps.begin(), amps.end());
  return amps[size/2];
}

int main() {
  hal::WishboneBus bus;
  bus.SpiInit();

  hal::Everloop everloop;
  everloop.Setup(&bus);

  hal::MicrophoneArray mics;
  mics.Setup(&bus);

  hal::EverloopImage image1d;

  std::valarray<int> lookup = {23, 27, 32, 1, 6, 10, 14, 19};

  std::valarray<float> magnitude(mics.Channels());

  std::valarray<float> coeff_hp = {
    0.0599851024734,-1.300381417101e-17,  -0.1549721713331,  -0.1626987043005,
     0.1053874898562,   0.2920599418361,   0.1053874898562,  -0.1626987043005,
    -0.1549721713331,-1.300381417101e-17,   0.0599851024734
  };

  std::valarray<FIR> filter_bank_hp(mics.Channels());
  for (auto& iir : filter_bank_hp) iir.Setup(coeff_hp);

  std::valarray<float> coeff_lp = {
      1.5414364192065861E-003, 2.3293447964816889E-003, 4.5521160193679406E-003,
      7.8514472469795749E-003, 1.1667089756749511E-002, 1.5336748122750850E-002,
      1.8215960726324591E-002, 1.9795701289425597E-002, 1.9795701289425597E-002,
      1.8215960726324591E-002, 1.5336748122750850E-002, 1.1667089756749511E-002,
      7.8514472469795749E-003, 4.5521160193679406E-003, 2.3293447964816889E-003,
      1.5414364192065861E-003};
  std::valarray<FIR> filter_bank_lp(mics.Channels());
  for (auto& iir : filter_bank_lp) iir.Setup(coeff_lp);

  int amps_size = 50;
  float values[mics.Channels()][mics.NumberOfSamples()];
  float amps[mics.Channels()][amps_size];
  bool bad_samples[mics.Channels()][amps_size];

  float percent = 1; // Threshold for bad sample detection


  float average[mics.Channels()];
  float max[mics.Channels()];
  float min[mics.Channels()];
  float amp[mics.Channels()];

  while (true) {
    mics.Read();

    for (int i = 0; i < 8; ++i)
    {
      average[i] = 0;
      min[i] = 10000;
      max[i] = -10000;
      amp[i] = 0;
    }

    // *********************************************
    //  Getting data from mics and calc max, min, ave etc
    // *********************************************
    for (unsigned int c = 0; c < mics.Channels(); c++) {
      for (unsigned int s = 0; s < mics.NumberOfSamples(); s++) {

        float x = filter_bank_hp[c].Filter(mics.At(s, c));
        values[c][s] = filter_bank_lp[c].Filter(x);

        if(values[c][s] > max[c]) max[c] = values[c][s];
        if(values[c][s] < min[c]) min[c] = values[c][s];
        average[c] += values[c][s];
      }
      average[c] = average[c] / mics.NumberOfSamples();
      max[c] = max[c] - average[c];
      min[c] = min[c] - average[c];
      amp[c] = max[c] - min[c];
    }

    for (int c = 0; c < mics.Channels(); c++) {
      // *********************************************
      //      Shifting amps % bad_samples arrays 
      // *********************************************
      for (int i = amps_size - 1; i > 0; i--){
        amps[c][i] = amps[c][i - 1];
        bad_samples[c][i] = bad_samples[c][i - 1];
      }

      amps[c][0] = amp[c];

      // *********************************************
      //           Detecting the bad MICs 
      // *********************************************
      float amp_median = median(amp,mics.Channels());

      if(abs(amp[c] - amp_median) > amp_median * percent)
        bad_samples[c][0] = true;
      else
        bad_samples[c][0] = false;
    }

    // *********************************************
    //                  Plot
    // *********************************************

    system("clear");

    // Signal Plot
    float signal_scale =  100;
    float plot_scale = 20;
    int val;

    std::cout << "|";

    for (int i = 0; i < mics.Channels(); i++)
      std::cout << "        MIC" << i + 1<< "        |" ;

    std::cout << std::endl;

    std::cout << "|";
    for (int i = 0; i < mics.Channels(); i++)
      std::cout << "--------------------|" ;

    std::cout << std::endl;

    std::cout << "|";
    for (int i = 0; i < mics.Channels(); i++){
      std::cout << "        " ;
      std::cout << std::left<< std::setw(12) << (int)amp[i];
      std::cout << "|" ;

    }

    std::cout << std::endl;
    std::cout << "|";

    for (int i = 0; i < mics.Channels(); i++)
      std::cout << "--------------------|" ;

    std::cout << std::endl;

    for ( int s = 0; s <  50; s++) {
      for (unsigned int c = 0; c < mics.Channels(); c++) {
        std::cout << "|" ;
        if(amps[c][s] < 0 ) amps[c][s] = 0;
        val = amps[c][s] * plot_scale / signal_scale;
        for (int i = 0; i < plot_scale; i++){
          if(i < val) {
            if(bad_samples[c][s])
              std::cout << "\033[31m" <<"x" << "\033[39m" ;
            else
              std::cout << "x" ;
          } else {
            std::cout << " ";
          }
        }
      }

      std::cout << "|";
      std::cout << std::endl;
    }

    usleep(100000);
  }

  return 0;
}
