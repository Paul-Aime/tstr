# Projet traitement du signal en temps réel

## Prerequisites

- RtAudio
	- Install ALSA Development Kit : `sudo apt-get install libasound-dev`
	- [Download RtAudio](https://www.music.mcgill.ca/~gary/rtaudio/)
	- 

- Run duplex test :
	- Check devices : `./test$ LD_LIBRARY_PATH=../.libs/ ./audioprobe`
	- in `./tests/duplex.cpp`, in `main`, change `oDevice` and `iDevice` by suitables ID, based on `./audioprobe` output.
	- `./test$ LD_LIBRARY_PATH=../.libs/ ./duplex 1 44100`

## Effet *reverb* à convolution

## Effet de type *limiter*