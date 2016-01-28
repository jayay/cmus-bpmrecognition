# cmus-bpmrecognition

! Under development !

Since the free music player is now able to deal with BPM values I thought it might be useful to recognize them.

## About

This project uses libSoundTouch for the recognition. (More to come soon).

There's a lot of code taken from the cmus project, such as reading/writing the cache, decoders (even the decoder libraries from cmus are being used).


## Building
Install [SoundTouch](http://www.surina.net/soundtouch/sourcecode.html) with integer support first:
```
./bootstrap
./configure -–enable-integer-samples
make
sudo make install
```

Then this project:
```
git clone https://github.com/jayay/cmus-bpmrecognition
cd cmus-bpmrecogntion
./configure
make
```
There is no `make install` step ATM.

## Usage
```
./build/cmus-bpmrecognition
```

It then starts to read the file `lib.pl`, then saves the bpm value into the `cache` file.

## License
The project is licensed under GPLv2.
