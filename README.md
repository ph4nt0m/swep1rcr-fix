# swep1rcr-fix
Fixes the black/white screen bug in **Star Wars Episode I: Racer**.

![img1](https://user-images.githubusercontent.com/18725448/34607938-3e03cb92-f216-11e7-8a20-8373af3881d7.png)
![img1_fix](https://user-images.githubusercontent.com/18725448/34607939-3e18f896-f216-11e7-8873-6149c46ba98f.png)

![img2](https://user-images.githubusercontent.com/18725448/34607940-3e326b96-f216-11e7-84e6-2d88443740f3.png)
![img2_fix](https://user-images.githubusercontent.com/18725448/34607941-3e47a5ce-f216-11e7-8d86-268b2b53268c.png)

## Building from Source
To build this project, you will need Microsoft Visual Studio 2015 or later (2013 might work aswell, but this has not been confirmed).

**Note**: The source code of this project is not portable and will therefore not compile with GCC/MinGW.

Clone the repository into a local folder:
```
git clone https://github.com/ph4nt0m/swep1rcr-fix.git
```
Next, create a `build` folder and run [CMake](https://cmake.org/) from there:
```
cd swep1rcr-fix
mkdir build
cd build
cmake ..
cmake --build . --config Release
```
After that, you'll find the DLL in `swep1rcr_fix/bin/Release/`.

### Alternative
If you do not want to use CMake, you can also create your own DLL-Project within Visual Studio and add all of the source files to it. Just make sure to include the module definition file (`exports.def`) under `Configuration Properties -> Linker -> Input`.

## Installation
Just copy the `ddraw.dll` into the folder which contains the main executable `SWEP1RCR.EXE`.

## How It Works
This fix consists of a proxy DLL for DirectDraw. When the game tries to load the original `ddraw.dll` (located in the system directory), it will find and use the proxy DLL instead. This allows to intercept any library call and modify certain objects before handing over to the original `ddraw.dll`.

To fix the black/white screen bug that has been reported on newer systems, the depth buffer has to be cleared before rendering a new frame. I discovered this fix some years ago (see [discussion](https://sourceforge.net/p/dxwnd/discussion/general/thread/444b7535/)) and subsequently it was integrated into the well known [DxWnd](https://sourceforge.net/projects/dxwnd/).

Nevertheless, in some cases it might be easier to simply use this DLL file if you do not need all the other features.
