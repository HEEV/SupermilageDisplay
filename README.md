# SuperMileageDisplay

## Recommended Environements:
- Windows: Visual Studio 2019 or newer with the Linux Developemnt plugin or VSCode with the CMake and C++ plugins.
- Linux: VSCode with CMake and C++ plugins.

## Downloading
Navigate into the location you would like your dev environment to be in and run the following commands to download the code and all libraries. 

## To Install this Github:
Clones the top level repository
```
git clone https://github.com/HEEV/SupermilageDisplay.git
```

This installs the linked repos. These commands must be exicuted in the directory containing this Readme file 
```
git submodule init
git submodule update --init --recursive
```

## Windows for Visual Studio 2019+
### The ptheads library:
The library pthreads is a required dependency that needs installed to compile on Windows. The recommended installation process is to install Microsoft's package manager, vcpackage, by following the directions on their [repo](https://github.com/Microsoft/vcpkg?msclkid=4263e668a79411ec8c22e54abcc67492&adlt=strict&toWww=1&redig=049C95C4D5C342F6BE12D46767C03E61).

To Install pthreads open cmd terminal with Administrator privilages in the same directory as you installed vcpackage and run:
```
vcpkg install pthreads:x64-windows
```
 When this is finished, open the parent folder with your IDE (for VS, you can right click and open with). If you are using Visual Studio, search for "connection manager" in the search bar at the top of the screen and add the IP of the Pi you are working with.
 
 ## Linux
 ### Install toolchains
 Install GCC or clang as well as cmake. Note that the compiler must support C++20. Currently, the version of clang installed by apt is not supported. CMake must be at least version 3.15.
 ### Install Dependencies
 Follow the instructions in [this JUCE documentation](https://github.com/juce-framework/JUCE/blob/master/docs/Linux%20Dependencies.md). Depending on your distribution, OpenSSL mat also be required. Ubuntu comes with this be default.
 
