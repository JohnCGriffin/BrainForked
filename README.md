# BrainForked - Fast BF interpreter in C++11

This is almost standard C++, except for the use of clang++/g++ computed labels extension.  At the
time of writing (Aug 2020), it is the fastest non-JIT interpreter for the commonly referenced mandelbrot and hanoi
BF programs.

## Building

Step 1 - The Makefile is set to use **clang++**.  Change to **g++** if you prefer.

Step 2 - On a Unix machine, use ```make```.  

Step 3 - ```make test`` to try it on the included mandelbrot.bf, 
        a common exercise in BF land.

