# MATRIX Hardware Abstraction Layer (HAL)
![Build Status](https://drone.matrix.one/api/badges/matrix-io/matrix-creator-hal/status.svg)

HAL is the lowest-level abstraction for the MATRIX Creator and MATRIX Voice drivers. You can interface directly with HAL, or use the higher level components like MALOS, and the MATRIX OS itself.

## Install
```
# Add repo and key
curl https://apt.matrix.one/doc/apt-key.gpg | sudo apt-key add -
echo "deb https://apt.matrix.one/raspbian $(lsb_release -sc) main" | sudo tee /etc/apt/sources.list.d/matrixlabs.list

# Update packages and install
sudo apt-get update
sudo apt-get upgrade

# Installation
sudo apt install libmatrixio-creator-hal libmatrixio-creator-hal-dev
```

## Install from source

### Pre-Requisites
It does have some package dependencies, so please make sure to install the pre-requisites.

```
sudo apt-get install cmake g++ git libfftw3-dev wiringpi matrixio-creator-init libgflags-dev
sudo reboot
```

To start working with HAL directly, you'll need to run `sudo make install` to get the build running. 
```
git clone --recursive https://github.com/matrix-io/matrix-creator-hal.git
cd matrix-creator-hal && mkdir build && cd build
cmake ..
make && sudo make install
[100%] Built target matrix_creator_hal
Install the project...
-- Install configuration: ""
-- Up-to-date: /usr/local/lib/libmatrix_creator_hal.a
-- Installing: /usr/local/include/matrix_hal/creator_memory_map.h
-- Installing: /usr/local/include/matrix_hal/everloop.h
-- Installing: /usr/local/include/matrix_hal/humidity_data.h
-- Installing: /usr/local/include/matrix_hal/imu_sensor.h
-- Installing: /usr/local/include/matrix_hal/pressure_data.h
-- Installing: /usr/local/include/matrix_hal/dummy_data.h
-- Installing: /usr/local/include/matrix_hal/everloop_image.h
-- Installing: /usr/local/include/matrix_hal/humidity_sensor.h
-- Installing: /usr/local/include/matrix_hal/matrix_driver.h
-- Installing: /usr/local/include/matrix_hal/pressure_sensor.h
-- Installing: /usr/local/include/matrix_hal/dummy_sensor.h
-- Installing: /usr/local/include/matrix_hal/gpio_control.h
-- Installing: /usr/local/include/matrix_hal/imu_data.h
-- Installing: /usr/local/include/matrix_hal/microphone_array.h
-- Installing: /usr/local/include/matrix_hal/wishbone_bus.h
```

### Examples
Run some of the examples below.
```
# should already be in ~/matrix-creator-hal/build
cd demos

# run the everloop demo
./everloop_demo

# see the rest of the examples
ls -l
```
