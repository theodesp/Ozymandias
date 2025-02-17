# Ozymandias

This is a fork of the **Julius/Augustus** project with the aim of making it work with _Pharaoh_ instead of _Caesar 3_.

The work is still in progress, so any help or support is appreciated. You can more or less load
original save games and play the first few missions in the campaign without major issues.

If you're looking for _Caesar 3_'s vanilla experience with modern machine compatibility,
check out [Julius](https://github.com/bvschaik/julius).<br>
If you want additional features and visual/UI improvements, and don't care about strict
compatibility with the original save files, check out [Augustus](https://github.com/Keriew/augustus).<br>
For the original game, check out the page on [Steam](https://store.steampowered.com/app/564530/Pharaoh__Cleopatra/)
or [GOG](https://www.gog.com/en/game/pharaoh_cleopatra).<br>
For the official upcoming remaster (unrelated to Ozymandias) called _A New Era_, check out the Steam page [here](https://store.steampowered.com/app/1351080/Pharaoh_A_New_Era/).

## Running the game

| Platform       | Latest release | Unstable build |
| -------------- | -------------- | -------------- |
| Windows        | -              | -              |
| Linux AppImage | -              | -              |
| Mac            | -              | -              |

After downloading the most recent binaries from above or building them from source,
start Ozymandias and it will ask you to point to an original Pharaoh installation folder.

Ozymandias, like Julius and Augustus, requires the original assets (graphics, sounds, etc)
from an unmodified game installation to run, in this case it needs _Pharaoh_ **as well as the _Cleopatra_ expansion.**

Note that you must have permission to write in the game data directory as the saves will be
stored there; also, your game must be patched to last version to run Ozymandias.

## Building Ozymandias from source

### Windows

- Clone the repository
- Download [SDL2](https://github.com/libsdl-org/SDL/releases/tag/release-2.24.0) and extract it to folder ext/SDL2
- Download [SDL2_mixer](https://github.com/libsdl-org/SDL_mixer/releases/tag/release-2.6.2) and extract it to folder ext/SDL2
- Download [SDL2_image](https://github.com/libsdl-org/SDL_image/releases/tag/release-2.6.2) and extract it to folder ext/SDL2

To build with your favorite IDE, just import the cmakelists.txt file as a project and build from there. Otherwise, you can use [MinGW-w64](https://www.mingw-w64.org/downloads/) to build via CLI:

- Install MinGW-w64 (either manually, or via [MSYS2](https://www.msys2.org/))
- Install [CMake](https://cmake.org/download/#latest)

- From the project's root folder execute, in order:
  
  ```
  mkdir build
  cd build
  cmake .. -G "MingGW Makefiles"
  mingw32-make
  ```

To run the engine, you'll also need the necessary dynamic libraries in the same folder as the executable, unless the project is set to build with static linkage.

- Navigate to the SDL2, SDL2_mixer and SDL2_image extracted files, and locate the .dll files inside the `bin` folder of the correct architecture (e.g. `SDL2_image-2.6.2/x86_64-w64-mingw32/bin/`)

- Copy the .dll files from each of the above to the `build` folder

### Linux

Only tested on Manjaro, but under Ubuntu or similar you should install the same packages:

- Clone the repository

- Install (You will need to enable AUR on Manjaro):
  
  - CMake
  - g++
  - sdl2
  - sdl2_mixer
  - sdl2_image

- From the root folder execute:
  
  ```
  mkdir build
  cd build
  cmake ..
  make
  ```

### MacOS

Currently not tested on Mac, but you can try the instructions from Julius. They should be similar besides the additional need for SDL2_image. https://github.com/bvschaik/julius/wiki/Building-Julius

(Feel free to add a pull request to add additional Mac instructions here.)