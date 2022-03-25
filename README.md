# SuperMileageDisplay

## Recommended Environements:
- Windows: Visual Studio 2019 or newer with the Linux Developemnt plugin

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

## The ptheads library:
pthreads is an exteranl dependancy, the easyest way is through the tool "vcpackage"
Link to repos is found here https://github.com/Microsoft/vcpkg?msclkid=4263e668a79411ec8c22e54abcc67492&adlt=strict&toWww=1&redig=049C95C4D5C342F6BE12D46767C03E61
Walk through repos install directions

To Install pthreads open cmd terminal with Administrator privilages then run
```
vcpkg install pthreads:x64-windows
```
 When this is finished, open the parent folder with your IDE (for VS, you can right click and open with). If you are using Visual Studio, search for "connection manager" in 
 the search bar at the top of the screen and add the IP of the Pi you are working with.
