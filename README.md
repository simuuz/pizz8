# pizza8

Simple C++ Chip-8 interpreter implemented in SDL2</br>

This has been done in a very small amount of time ~~across 2 days~~ (actually took more time to fix stuff), ~~could be called a "speedrun" if you want~~</br>

## Installation and usage

Have CMake, Make and SDL2 installed. Run `cmake -B build/` in the root directory and then `make -j$(nproc --all) -C build/`</br>

To launch a rom, create a "roms" (or any name actually) directory inside the root directory and then run `./build/chip-8 <roms folder>/<the rom>`</br>

**Do note this has been tested only on Linux**

![Corax89's test passing](opcode.png)
![BestCoder test passing](bc_test.png)
![Trip8 looking good](trip8.gif)

## Future plans

Support for SuperChip/CHIP-48, hopefully
