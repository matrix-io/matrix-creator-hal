# MATRIX Hardware Abstraction Layer (HAL)

![Build Status](https://drone.matrix.one/api/badges/matrix-io/matrix-creator-hal/status.svg)

[MATRIX Hardware Abstraction Layer (HAL)](https://matrix-io.github.io/matrix-documentation/matrix-hal/overview/) is an open source library for directly interfacing with the MATRIX Creator and MATRIX Voice. MATRIX HAL consists of driver files written in C++ which enable the user to write low level programs in C++.

## Install from Package

```
# Add repo and key
curl https://apt.matrix.one/doc/apt-key.gpg | sudo apt-key add -
echo "deb https://apt.matrix.one/raspbian $(lsb_release -sc) main" | sudo tee /etc/apt/sources.list.d/matrixlabs.list

# Update repo and packages
sudo apt-get update
sudo apt-get upgrade

# Install MATRIX HAL Packages
sudo apt-get install matrixio-creator-init libmatrixio-creator-hal libmatrixio-creator-hal-dev
```

### Helpful Information

MATRIX HAL header files are installed in /usr/include/matrix_hal.

The compiled MATRIX HAL library file is installed in /usr/lib/libmatrix_creator_hal.so.

## Install from Source

```
# Add repo and key
curl https://apt.matrix.one/doc/apt-key.gpg | sudo apt-key add -
echo "deb https://apt.matrix.one/raspbian $(lsb_release -sc) main" | sudo tee /etc/apt/sources.list.d/matrixlabs.list

# Update repo and packages
sudo apt-get update
sudo apt-get upgrade

# Install MATRIX HAL Packages and Build Prerequisites
sudo apt-get install cmake g++ git libfftw3-dev wiringpi libgflags-dev matrixio-creator-init

# Reboot your Pi
sudo reboot
```

### Build Intructions

```
cd ~/
git clone https://github.com/matrix-io/matrix-creator-hal.git
cd matrix-creator-hal
mkdir build
cd build
cmake ..
make -j4 && sudo make install
```

### Helpful Information
MATRIX HAL header files are installed in /usr/local/include/matrix_hal.

The compiled MATRIX HAL library file is installed in /usr/local/lib/libmatrix_creator_hal.so.

## References and Examples

[Function References](https://matrix-io.github.io/matrix-documentation/matrix-hal/reference/)

[Examples](https://matrix-io.github.io/matrix-documentation/matrix-hal/examples/)

## Demos

These demos are only availiable if you downloaded HAL from source.

```
# Should already be in ~/matrix-creator-hal/build
cd demos

# run the everloop demo
./everloop_demo

# see the rest of the examples
ls -l
```
