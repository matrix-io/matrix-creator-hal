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
sudo apt-get install libblas-dev liblapack-dev pocketsphinx --no-install-recommends
```

### Building
``` 
git clone https://github.com/matrix-io/matrix-creator-hal.git hal
cd matrix-creator-hal
git checkout av/pocketsphinx_demo
mkdir build && cd build && cmake .. && make
```

### Install testing commands:
Download sample language and dictionary from [here](https://drive.google.com/file/d/0B3lA7p7SjZu-YUJxYmIwcnh4Qlk/view?usp=sharing) or make new models (explanation below) then extract it:
```
cd demos
mkdir assets
tar xf TAR6706.tgz -C assets
```
### Run DEMO:
on matrix-creator-hal/build/demos:
```
./pocketsphinx_demo -keyphrase "MATRIX" -kws_threshold 1e-20 -dict assets/6706.dic -lm assets/6706.lm -inmic yes -adcdev mic_channel8
``` 
- *mic_channel8* (all microphones array)
- *mic_channelX* (only X microphone)

#### (optional) Custom lenguage and phrases for recognition 

+ Make a text plane like this: 
``` 
matrix
everloop
arc 
clear
stop
shutdown
now
ipaddress
matrix everloop
matrix clear
matrix stop
matrix ipaddress
matrix game time
matrix one minute
matrix two minutes
matrix three minutes
matrix four minutes
matrix five minutes
matrix ten seconds
matrix ten minutes
```

+ Upload this file to [Sphinx Knowledge Base Tool](http://www.speech.cs.cmu.edu/tools/lmtool-new.html) and compile knowledge base.

+ Dowload *TARXXXXX.tgz* and upgrade assets: 


## IP Address DEMO
on matrix-creator-hal/build/demos:
```
./ipaddress_demo
```
IP Address demo print the last integer of wlan0 IP address, for example if RasberryPi has a 192.168.0.135 IP the everloop ring show it like this:
> [yellow led] one blue led    (1)
> [yellow led] three blue leds (3)
> [yellow led] five blue leds  (5)

## Timer DEMO
on matrix-creator-hal/build/demos:
```
./timer_demo
```
optional options:
```
./timer_demo seconds
```
