#!/bin/bash

keyphrase="$1"
MIC=$2
threshold="1e-20"

if [ "$keyphrase" = "" ]; then
    keyphrase="HEY MATRIX"
fi

if [ "$MIC" = "" ]; then
    MIC=8
fi

sphinx=../build/demos/pocketsphinx_demo 
$sphinx -keyphrase "$keyphrase" -kws_threshold $threshold -inmic yes -adcdev mic_channel$MIC

