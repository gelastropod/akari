## Akari

### Requirements
CMake is needed for this project. First, make sure you have [CMake](https://cmake.org/) installed.
To solve the SAT problem, this project uses [Kissat](https://github.com/arminbiere/kissat).

### Usage
Build the project:
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

Now, you should end up with a `main` binary in the `build` directory you just created. You can move this binary anywhere you wish, but make sure the `kissat` binary is in the same directory as it.

To run the program, simply run `./main [akari puzzle number]`. The program should output a grid of 0 and 1s, where each '1' represents the location of a light (akari).
