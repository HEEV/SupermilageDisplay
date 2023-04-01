# Setup Windows Development Environment
Currently, the only two tested editors on Windows are [VSCode](https://code.visualstudio.com/) and [Visual Studio](https://visualstudio.microsoft.com/). If you are succesful in setting up another environment, please document the process here. If you have no experience in either, VSCode is recommended.
> ***TIP*** To open a terminal in a directory, click on the file address box at the top of the File Explorer, type cmd, and press enter.
## Environment Setup
### Install Plugins
#### VSCode Plugins
The following plugins are needed for use with VSCode. These can be downloaded under the extensions tab.
- C/C++ Extension Pack
- CMake
- CMake Tools
#### Visual Studio Plugins
If downloading Visual Studio for the first time, make sure to select these plugins. If already downloaded, reopen the installer and click modify to select them.
- Desktop development with C++
- Linux and embedded development with C++ (optional)
### Download Git
[Download the standalone installer](https://git-scm.com/download/win) and follow the directions. Accept all of the recommended settings except for the default editor. Select whichever is your prefrence (VSCode is a good option).
### Register SSH Keys to Github Account
Follow [these instructions](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent) to create an SSH key. When asked what to name the file, press enter. This will give the file the default name. **When asked to create a password, press enter.** Currently, there is a glitch with the Windows implementation of SSH that causes an authentication error with git submodules when a password is set. You may stop when you reach the section titled "Generating a new SSH key for a hardware security key". To add the key to your account, follow [these directions](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/adding-a-new-ssh-key-to-your-github-account), starting in the "Adding a new SSH key to your account" section. To validate this was successful, run the following command:
```posh
ssh git@github.com
```
When asked to confirm the key, type yes and press enter.
### Download the Project
To download the project, first navigate to the desired download location and open a terminal. After this, run the following command:
```posh
git clone git@github.com:HEEV/SupermilageDisplay.git --recursive
```
### Download VCPKG and libusb
[Follow these instructions](https://vcpkg.io/en/getting-started.html) to install VCPKG through step 2 (the integration step is not necessary). Once installed, navigate to the VCPKG directory and open a terminal. Run the following command to install libusb:
```posh
./vcpkg install libusb
```
Once this has finished, follow the instructions for your chosen IDE to allow it to use VCPKG libraries.
#### VSCode
Open the project in VSCode and create the file .vscode/settings.json. In this file, paste the following, but replace [vcpkg root] with the download location of VCPKG.
```json
{
  "cmake.configureSettings": {
    "CMAKE_TOOLCHAIN_FILE": "[vcpkg root]/scripts/buildsystems/vcpkg.cmake"
  }
}
```
#### Visual Studio
Under the project tab, click on "CMake Settings for SuperMileageDisplay". Under the CMake toolchain file setting, paste the following code but replace [vcpkg root] with the download location of VCPKG.
```posh
[vcpkg root]/scripts/buildsystems/vcpkg.cmake
```
## Build the Project
The project is ready to be compiled! To do so, click on the play button at the bottom of the screen (VScode) or at the top (Visual Studio). Note that the play button at the top right in VSCode will not work.
