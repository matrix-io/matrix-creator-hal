# MATRIX Hardware Abstraction Layer (HAL)

![Build Status](https://drone.matrix.one/api/badges/matrix-io/matrix-creator-hal/status.svg)

MATRIX Hardware Abstraction Layer (HAL) is an open source library for directly interfacing with the MATRIX device. MATRIX HAL consists of driver files written in C++ which enable the user to write low level programs in C++.

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

## Examples

Learn HAL with thoroughly commented examples.

[Examples](https://github.com/matrix-io/matrix-hal-examples)

## Demos

Run some of the demos below.

```
# Should already be in ~/matrix-creator-hal/build
cd demos

# run the everloop demo
./everloop_demo

# see the rest of the examples
ls -l
```
