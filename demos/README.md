# MATRIX HAL DEMOS


## Pocketsphinx demo

### Dependencies 
```
echo "deb http://dev-packages.matrix.one/ stable main" | sudo tee -a /etc/apt/sources.list
sudo apt-get update && sudo apt-get install --yes pocketsphinx
```

### Building
``` 
git clone --recursive git@github.com:matrix-io/matrix-creator-hal.git
cd matrix-creator-hal
git checkout av/pocketsphinx_demo
mkdir build && cd build && cmake .. && make
```
### Prepare lenguage and phrases for recognition

1. On external PC or RasberryPi browser make text plane with this:
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
3. Dowload [TARXXXXX.tgz](http://www.speech.cs.cmu.edu/tools/product/1477415688_3g2372/TAR0288.tgz) to build/demo directory on HAL brach and descompress it like this:
```
cd demos
wget -ct0 http://www.speech.cs.cmu.edu/tools/product/1477415688_32372/TAR0288.tgz
mkdir assets
tar xf TAR0288.tgz -C assets
```

### Run DEMO:
```
./pocketsphinx_demo -dict assets/0288.dic -lm assets/0288.lm -inmic yes -adcdev mic_channel0

``` 

