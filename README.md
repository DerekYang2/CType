<h1 align="center">
  <br>
  <a href="https://github.com/DerekYang2/CType"><img src="https://github.com/DerekYang2/CType/assets/115889767/0f7fd2d9-1161-46f4-8aa9-e1c6a3c2753f" width=300px></a>
</h1>

<h3 align="center">Minimalistic Typing Test in C/C++</h3>

<p align="center">
  <a href="#about">About</a> •
  <a href="#behavior">Behavior</a> •
  <a href="#features">Features</a> •
  <a href="#build">Build</a> •
  <a href="#installation">Installation</a> •
  <a href="#limitations">Limitations</a> •
  <a href="#license">License</a> 
</p>

<p align="center">
  <img src="./Sample.webp" alt="Sample Webp" />
</p>

---

## About 
A dependency free typing test application that aims to be a C++ implementation of Monkeytype. Contains the fundamental features including custom test settings, user settings, themes, dictionaries, fonts, sounds, screenshots, and various UI features including hovering hints, toggles, scrolling, interactive results graph, text input boxes, etc. Written from scratch in C++ with OpenGL drawing functions provided by [Raylib](https://github.com/raysan5/raylib) and a simple json parser provided by [RSJ](https://github.com/subh83/RSJp-cpp). As a result, executable runs without any setup on most Windows platforms as DLLs come with Windows 10 and 11. Linux is also supported with installation of a few libraries. The UI is written from scratch (see `src/ui`) and is thus unfortunately very custom to this project. 

## Behavior
Test has similar behavior to Monkeytype: pressing space too early will jump to the next word, backspace is ignore if previous word is correctly typed, space is ignored if you haven't started the next word, etc. Test statistics are calculated as follows:

<center>
<table>
  <tr>
    <th>Statistic</th>
    <th>Description</th>
  </tr>
  <tr>
    <td>WPM</td>
    <td>Number of characters typed in <strong>only correct words</strong> divided by seconds elapsed (characters/second) converted to words/minute. All characters from an incorrectly typed word are ignored.</td>
  </tr>
  <tr>
    <td>Accuracy</td>
    <td>Percentage of characters that were typed correctly, regardless  of whether it was in a correctly typed word.</td>
  </tr>
  <tr>
    <td>Raw WPM</td>
    <td>Raw WPM is the WPM of the full test not counting incorrect characters.</td>
  </tr>
  <tr>
    <td>Consistency</td>
    <td>Calculated using the coefficient of variation mapped from 0% to 100% by dividing the coefficient by a worst case variation.</td>
  </tr>
  <tr>
    <td>Instantaneous WPM</td>
    <td>The WPM from the last 5 seconds (displayed as <code>instant</code> in graph results hint).</td>
  </tr>
</table>
</center>

## Features
CType has many features despite being implemented from scratch, including:
- Executable directly runs with no dependencies (on Windows)
- Can theoretically compile on all platforms Raylib compiles on
- Extremely lightweight and offline
- Input custom test time, set punctuation and numbers settings
- Real-time setting updating and saving
- Smooth scrolling tape mode, adaptive to instantaneous WPM
- Theme loading with almost all Monkeytype CSS themes
- Font loading with most Monkeytype fonts and extra commonly used fonts
- Custom dictionary for tests with ASCII Monkeytype dictionaries included
- Sound effect loading with Monkeytype custom sounds
- Asset folder loading - easily drag in your own custom assets
- Signed distance fields fragment shader for anti-aliased text
- Embedded default font, images, and other resources as header files with an exporter program
- Simple multi-threaded asset loading
- Full test results graph with Monkeytype statistics and interactions (hover)
- Screenshots that save to dedicated folder
- Json data and setting loading 

## Build 
The only dependency is Raylib, which already has static libraries for Windows and Linux included in the `lib` folder. The makefile currently automatically detects Windows versus Linux operating systems. `libraylib.a` is a static library for raylib version 4.5, with custom tweaks for blending modes. The alpha blending in Raylib is drastically improved with a custom function as outlined in https://github.com/raysan5/raylib/pull/2741. This significantly improves font rendering, removing the thin black line around textures. 

### Windows
Call `mingw32-make all` in the root directory of the project to compile the executable. Call `mingw32-make run` to run the `.exe`, or run it directly by other means. If you want to compile and run the `.exe` in one command, call `mingw32-make default`. 
- Call `mingw32-make clean` to remove all old object files.
- Call `mingw32-make release` after cleaning to compile a release version of the `.exe`. The folders `data`, `fonts`, `languages`, `screenshots`, `sounds`, `themes` should be in the same root folder as the executable. Include any assets of your choice, as long as they are the right file format. The executable is then ready to run!

Note that some may use `make` instead of `mingw32-make` which is typically installed from MSYS2. Use whatever Makefile command you prefer.

### Linux

For Linux, libraries required by Raylib must be installed:  https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux. For example, on Ubuntu, run:
```
sudo apt install libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev
```
Makefile commands are the same as that of windows.

## Running
If you just want to try the application, visit https://github.com/DerekYang2/CType/releases for instructions. Download the zipped folder which contains the executable and proper folder structure. 

For Windows 10 and 11, the .exe file is standalone -- you should be able to directly start using the application. For now, Linux may require a bit of installing and setup.

## Limitations
- Only regular ASCII character support for now.
- Screen scaling keeps a 1920x1080 ratio with black padding to fill the rest, possibly add custom ratio.
- Ideally, there should have been a better layout management system for dynamic aspect ratio and screen scaling.

## License
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
