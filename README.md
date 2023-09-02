<h1 align="center">
  <br>
  <a href="https://github.com/DerekYang2/CType"><img src="https://github.com/DerekYang2/CType/assets/115889767/0f7fd2d9-1161-46f4-8aa9-e1c6a3c2753f"></a>
</h1>

<h4 align="center">CType: Minimalistic Typing Test in C/C++</h4>

---

## About 
Long term project in progress. 

A dependency free typing test application inspired by Monkeytype. Contains many fundamental features including custom test settings, user settings, themes, dictionaries, fonts, sounds, screenshots, and various UI features including hovering hints, toggles, scrolling, interactive results graph, text input boxes, etc. Written from scratch in C++ with OpenGL drawing functions provided by https://github.com/raysan5/raylib. As a result, executable runs without any setup on most Windows platforms as DLLs come with Windows 10 and 11. Linux is also supported with installation of a few libraries. Tested to be lightweight enough to easily reach above 60 fps on your dying 2-core 2GB RAM chromebook.

## Behavior
Test has similar behavior to Monkeytype, where pressing space too early will jump to the next word and WPM is calculated using the number of characters in correct words only.

**TODO: Add specifics behind other stat calculations**

## Screenshots
![image](https://github.com/DerekYang2/CType/assets/115889767/6265a83f-5b90-4fa3-beb1-46b3de08b542)
![image](https://github.com/DerekYang2/CType/assets/115889767/618e4504-f643-4e37-8663-f70c8047c33f)

## Features
- Executable directly runs with no dependencies (on Windows)
- Can theoretically compile on all platforms Raylib compiles on
- Extremely lightweight and offline
- Input custom test time
- Real-time setting updating and saving
- Smooth scrolling tape mode, adaptive to instantaneous WPM
- Theme loading with all Monkeytype CSS themes
- Font loading with most Monkeytype fonts and extra commonly used fonts
- Custom dictionary for tests with ASCII Monkeytype dictionaries included
- Sound effect loading with Monkeytype custom sounds
- Asset folder loading - easily drag in your own custom assets
- Signed distance fields fragment shader for anti-aliased text
- Embedded default font, images, and other resources as header files (`.h`)
- Simple multi-threaded asset loading
- Full test results graph with Monkeytype statistics and interactions (hover)
- Screenshots that save to dedicated folder
- Json data and setting loading 

## Build 
The only dependency is Raylib, which already has static libraries for Windows and Linux included in the `lib` folder. The makefile currently automatically detects Windows versus Linux operating systems. Feel free to make custom tweaks.

### Windows
Call `mingw32-make all` in the root directory of the project to copmile the .exe. Call `mingw32-make run` to run the .exe, or run it directly by other means. If you want to compile and run the .exe in one command, call `mingw32-make default`. 
- Call `mingw32-make clean` to remove the old object files and recompile everything. 
- Call `mingw32-make release` after cleaning to compile a release version of the .exe. This will be a bit slower due to static compilation.

### Linux

For Linux, libraries required by Raylib must be installed:  https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux. For example, on Ubuntu, run:
```
sudo apt install libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev
```
The commands are the same as windows, however use `make` instead of `mingw32-make`.

## Running
If you just want to try the application, visit https://github.com/DerekYang2/CType/releases for instructions. Download the zipped folder which contains the executable and proper folder structure. The location you place the master folder does not matter.

For Windows 10 and 11, you will likely be able to directly start using the application. Linux may require a bit of setup.

## Issues and Limitations
- Weak antialiasing for shapes and lines, especially evident with circular shapes and interpolation line in results graph. Solution will require tweaks in the OpenGL functions of the Raylib library. Nvidia control panel FXAA fixes these jagged curves yet ruins the color for small text. A fragment shader FXAA also fixes curves, yet is much worse by completely removing pixels, thin lines, etc. 
- Issue with Raylib SDF font loading: loaded text have very thin black outlines which can make them difficult to read. Only noticeable for reading thin text for themes with light text on a light background. 
- Only regular ASCII character support for now.
- Screen scaling keeps a 1920x1080 ratio with black padding to fill the rest, possibly add custom ratio.
