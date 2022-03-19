# SuperMileageDisplay

## Recommended Environements:
- Windows: Visual Studio 2019 or newer with the Linux Developemnt plugin

## Downloading
Navigate into the location you would like your dev environment to be in and run the following commands to download the code and all libraries. 
```
--New 
To Install this Github
-----------------------
--Clones the top level repository
git clone https://github.com/HEEV/SupermilageDisplay.git

--This installs the linked repos. These commands must be exicuted in the directory containing this Read me file 
git submodule init
git submodule update --init

The ptheads library
-------------------
--pthreads is an exteranl dependancy, the easyest way is through the tool "vcpackage"
--link to repos is found here 
--walk through repos install directions

--Install pthreads
--------------------
--open cmd terminal with Administrator privilages then run
vcpkg install pthreads:x64-windows

```
 When this is finished, open the parent folder with your IDE (for VS, you can right click and open with). If you are using Visual Studio, search for "connection manager" in 
 the search bar at the top of the screen and add the IP of the Pi you are working with.
