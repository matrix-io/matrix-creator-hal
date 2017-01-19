#!/bin/bash

keyphrase="$1"
MIC=$2

if [ "$keyphrase" = "" ]; then
    keyphrase="HEY MATRIX"
fi

if [ "$MIC" = "" ]; then
    MIC=8
fi

sphinx=../build/demos/pocketsphinx_demo 
$sphinx -keyphrase "$keyphrase" -kws_threshold 1e-20  -inmic yes -adcdev mic_channel$MIC

