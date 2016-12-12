# MATRIX HAL DEMOS


## Pocketsphinx demo

### Dependencies 

Before, please install MALOS and perform device reboot. For more details: [Getting Started Guide](https://github.com/matrix-io/matrix-creator-quickstart/wiki/2.-Getting-Started)
```
echo "deb http://packages.matrix.one/matrix-creator/ ./" | sudo tee --append /etc/apt/sources.list
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install matrix-creator-init matrix-creator-malos cmake g++ git libzmq3-dev --no-install-recommends
reboot
```
Add some dependencies and pocketsphinx package
```
echo "deb http://dev-packages.matrix.one/ stable main" | sudo tee -a /etc/apt/sources.list
sudo apt-get update
sudo apt-get install libblas-dev liblapack-dev pocketsphinx --no-install-recommends --yes 
```

### Building
``` 
git clone https://github.com/matrix-io/matrix-creator-hal.git hal
cd matrix-creator-hal
git checkout av/pocketsphinx_demo
mkdir build && cd build && cmake .. && make
```
### Prepare lenguage and phrases for recognition

1. Make a text plane with this:
``` 
matrix everloop
matrix arc
matrix green
matrix clear
matrix stop
matrix shutdown now
matrix get ipaddress
``` 
2. Upload this file to [Sphinx Knowledge Base Tool](http://www.speech.cs.cmu.edu/tools/lmtool-new.html) and compile knowledge base.
3. Dowload [TARXXXXX.tgz](http://www.speech.cs.cmu.edu/tools/product/1477415688_3g2372/TAR0288.tgz) to build directory on HAL branch and descompress it like this:
```
cd demos
wget -ct0 http://www.speech.cs.cmu.edu/tools/product/1477415688_32372/TAR0288.tgz
mkdir assets
tar xf TAR0288.tgz -C assets
```

### Run DEMO:
on matrix-creator-hal/build/demos:
```
./pocketsphinx_demo -dict assets/0288.dic -lm assets/0288.lm -inmic yes -adcdev mic_channel8

``` 

## IP Address DEMO
on matrix-creator-hal/build/demos:
```
./ipaddress_demo
```
IP Address demo print the last integer of wlan0 IP address, for example if RasberryPi has a 192.168.0.135 IP the everloop ring show it like this:
> [yellow led] one blue led    (1)
> [yellow led] three blue leds (3)
> [yellow led] five blue leds  (5)
