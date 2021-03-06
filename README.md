# CHIP8-Emulator
A cross-platform CHIP-8 VM/interpreter written as a way to experiment with Cmake, QT and dabble into emulators

The emulator doesn't play sound, altho the relevant opcodes are there (learning pcm is for another time)

![screen-gif](./ReadmeAssets/Chip8-woking.gif)

### Installing
Windows users can download a portable install [here](https://github.com/RikFabri/CHIP8-Emulator/releases).<br>
I don't provide any linux binaries at the moment, but I have compiled it on manjaro myself to make sure it works.


### Input
Chip-8 programs can react to A-F and 0-9, For example, the brix and breakout keybindings are 4 and 6 for left and right.

1	2	3	C<br>
4	5	6	D<br>
7	8	9	E<br>
A	0	B	F<br>

### Roms
The emulator comes with a bunch of roms, they're originally from [here](https://www.zophar.net/pdroms/chip8/chip-8-games-pack.html).

### Special thanks
The [wiki](https://en.wikipedia.org/wiki/CHIP-8) page and [Thomas P. Greene's technical reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#3.1) were a huge help.
