# pizz8

Fast C++ Chip-8 interpreter implemented in SDL2</br>

This has been done in a very small amount of time ~~across 2 days~~ (actually took more time to fix stuff), ~~could be called a "speedrun" if you want~~</br>

## Installation and usage

Have CMake, Make and SDL2 installed. Run `cmake -B build/` in the root directory and then `make -j -C build/`</br>

To launch a rom, run `./build/chip-8 <program> [options]`</br>

Options:

```
--delay/-d:
    -Specify the delay (in milliseconds, default: 16 {don't use 0}) between frames in ms

--scale/-s:
    -Specify a window scale (default: 10)
```

![Corax89's test passing](opcode.png)
![BestCoder test passing](bc_test.png)
![Trip8 looking good](trip8.gif)
