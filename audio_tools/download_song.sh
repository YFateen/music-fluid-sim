#!/usr/bin/env bash


echo Run this from project root!
echo $1
echo $2

rm -rf res/$2
mkdir res/$2
cd res/$2

youtube-dl --extract-audio --audio-format mp3 --output  "$2.%(ext)s" $1
cd ../../
python3 audio_tools/prepare_audio.py res/$2/$2.mp3 res/$2/$2_8b_200Hz -r 200

cd cmake-build-debug/
cd build/

echo Run this command!!!!
echo ./draw ../res/$2/$2.mp3 ../res/$2/$2_8b_200Hz
./draw ../res/$2/$2.mp3 ../res/$2/$2_8b_200Hz
echo Run this command!!!!
echo ./draw ../res/$2/$2.mp3 ../res/$2/$2_8b_200Hz
