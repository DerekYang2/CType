<h1 align="center">
  <br>
  <a href="https://github.com/DerekYang2/CType"><img src="https://github.com/DerekYang2/CType/assets/115889767/0f7fd2d9-1161-46f4-8aa9-e1c6a3c2753f"></a>
</h1>

<h4 align="center">CType: Minimalistic Typing Test in C/C++</h4>

---

## About 
Long term project in progress.

A low-level implementation and extremely lightweight typing test in the likeness of Monkeytype. Written from scratch with OpenGL drawing functions provided by https://github.com/raysan5/raylib. As a result, executable runs without further setup on most Windows platforms as DLLs come with Windows 10 and 11. Tested that it is lightweight enough to reach above 60 fps on your dying 2-core 2GB RAM chromebook.
Test has similar behavior to Monkeytype, where pressing space too early will jump to the next word and WPM is calculated using the number of characters in correct words only rather than following the formula that uses accuracy.

## Screenshots
![image](https://github.com/DerekYang2/CType/assets/115889767/6265a83f-5b90-4fa3-beb1-46b3de08b542)
![image](https://github.com/DerekYang2/CType/assets/115889767/618e4504-f643-4e37-8663-f70c8047c33f)

## Features
- Executable directly runs with no dependencies (on Windows)
- Extremely lightweight
- Completely offline
- Input custom test time
- Realtime setting updating and saving
- Smooth scrolling tape mode, adaptive to instantaneous WPM
- Theme loading with all Monkeytype CSS themes
- Signed distance fields fragment shader for anti-aliased text
- Embedded TTF fonts, images, and other resources as header files
- Full test results graph with Monkeytype statistics and graph interaction

## Build 
Dependencies are Raylib and Native File Dialogue, which have libraries included. The remaining DLLs are automatically included with Windows are are linked by Makefile. Run `make all` or `mingw32-make all` in root directory of project to call on the `all` recipe. As of now, only works for Windows though can be easily adapted to work with Linux. 

## Issues and Limitations
- Weak antialiasing for shapes and lines, especially evident with circular shapes and interpolation line in results graph. Interpolation line uses a triangle fan to draw a cubic spline with some thickness. Solution requires tweaks for the OpenGL functions in Raylib library. Nvidia control panel FXAA fixes these jagged curves yet ruins the color for small text. Fragment shader FXAA also fixes curves yet is much worse by completely removing pixels, thin lines, etc. 
- Light, small text have thin black outlines which make them difficult to read. Not an issue with light text on dark background. Only an issue for themes with small, light text on a light background.
- Window title bar is by default white and not customizable through Raylib.
- Native file dialog default folder path results in error.
- Only regular ASCII character support for now.
- Screen scaling keeps a 1920x1080 ratio with black padding to fill the rest, possibly add custom ratio.
