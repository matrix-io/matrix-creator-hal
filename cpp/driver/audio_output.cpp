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

#include <string>
#include <thread>
#include <valarray>

#include "cpp/driver/audio_output.h"
#include "cpp/driver/creator_memory_map.h"

namespace matrix_hal {

AudioOutput::AudioOutput()
    : mute_status_(kUnMute),
      volumen_percentage_(48),
      selector_hp_nspk_(kHeadPhone),
      PCM_sampling_frequency_(44100) {
  write_data_.resize(kMaxWriteLength);
}

AudioOutput::~AudioOutput() {}

void AudioOutput::Setup(MatrixIOBus *bus) {
  MatrixDriver::Setup(bus);
  FIFOFlush();
}

bool AudioOutput::Mute() {
  if (!bus_) return false;
  bus_->Write(kConfBaseAddress + 10, kMute);
  mute_status_ = kMute;
  return true;
}

bool AudioOutput::UnMute() {
  if (!bus_) return false;
  bus_->Write(kConfBaseAddress + 10, kUnMute);
  mute_status_ = kUnMute;
  return true;
}

bool AudioOutput::FIFOFlush() {
  if (!bus_) return false;
  bus_->Write(kConfBaseAddress + 12, 0x0001);
  bus_->Write(kConfBaseAddress + 12, 0x0000);
  return true;
}

bool AudioOutput::SetOutputSelector(OutputSelector output_selector) {
  if (!bus_) return false;
  bus_->Write(kConfBaseAddress + 11, output_selector);
  selector_hp_nspk_ = output_selector;
  return true;
}

bool AudioOutput::GetPCMSamplingFrequency() {
  if (!bus_) return false;
  uint16_t PCM_constant;
  bus_->Read(kConfBaseAddress + 9, &PCM_constant);
  for (int i = 0;; i++) {
    if (PCM_constant == PCM_sampling_frequencies[i][1]) {
      PCM_sampling_frequency_ = PCM_sampling_frequencies[i][0];
      break;
    }
  }
  return true;
}

bool AudioOutput::SetPCMSamplingFrequency(uint32_t PCM_sampling_frequency) {
  if (!bus_) return false;
  uint16_t PCM_constant;
  for (int i = 0;; i++) {
    if (PCM_sampling_frequencies[i][0] == 0) return false;
    if (PCM_sampling_frequency == PCM_sampling_frequencies[i][0]) {
      PCM_sampling_frequency_ = PCM_sampling_frequencies[i][0];
      PCM_constant = PCM_sampling_frequencies[i][1];
      break;
    }
  }
  bus_->Write(kConfBaseAddress + 9, PCM_constant);
  return true;
}

uint16_t AudioOutput::GetFIFOStatus() {
  if (!bus_) return false;
  uint16_t write_pointer;
  uint16_t read_pointer;
  bus_->Read(kAudioOutputBaseAddress + 0x802, &read_pointer);
  bus_->Read(kAudioOutputBaseAddress + 0x803, &write_pointer);

  if (write_pointer > read_pointer)
    return write_pointer - read_pointer;
  else
    return kFIFOSize - read_pointer + write_pointer;
}

void AudioOutput::Write() {
  std::thread pcmt([this] {
    float sample_time = 1.0 / PCM_sampling_frequency_;
    uint16_t fifo_status = GetFIFOStatus();

    if (fifo_status > kFIFOSize * 3 / 4) {
      int sleep = int(kMaxWriteLength * sample_time * 1000);
      std::this_thread::sleep_for(std::chrono::milliseconds(sleep));
    }
    bus_->Write(kAudioOutputBaseAddress,
                reinterpret_cast<unsigned char *>(&write_data_[0]),
                sizeof(uint16_t) * kMaxWriteLength);
  });
  pcmt.join();
}

bool AudioOutput::SetVolumen(int volumen_percentage) {
  if (!bus_) return false;
  if (volumen_percentage > 100) return false;
  uint16_t volumen_constant =
      (100 - volumen_percentage) * kMaxVolumenValue / 100;
  bus_->Write(kConfBaseAddress + 8, volumen_constant);
  return true;
}

};  // namespace matrix_hal
