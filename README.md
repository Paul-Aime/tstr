# Projet traitement du signal en temps réel

## Prerequisites

- RtAudio
	- Install ALSA Development Kit : `sudo apt-get install libasound-dev`
	- [Download RtAudio](https://www.music.mcgill.ca/~gary/rtaudio/)
	- Put it in `./libs` and install it
	- Check in `./CmakeFileLists.txt` that `RTAUDIO_DIR` variable is correctly set to corresponding path.

## Usage

```bash
rm -rf build
mkdir build
cd build
cmake ..
make
./reverb 1 44100
```

## Effet *reverb* à convolution

## Effet de type *limiter*