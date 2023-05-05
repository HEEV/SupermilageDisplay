# Setup Linux (Ubuntu) Development Environment
Currently, the only tested IDE is VSCode. If you get another to work, please document the environment setup here. Note that this process can be followed on the Raspberry Pis if needed.
## Environment Setup
### VSCode Plugins
The following plugins are needed for use with VSCode. These can be downloaded under the extensions tab.
- C/C++ Extension Pack
- CMake
- CMake Tools
### Register SSH Keys to Github Account
Follow [these instructions](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent) to create an SSH key. When asked what to name the file, press enter. This will give the file the default name. When asked to create a password, press enter. You may stop when you reach the section titled "Generating a new SSH key for a hardware security key". To add the key to your account, follow [these directions](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/adding-a-new-ssh-key-to-your-github-account), starting in the "Adding a new SSH key to your account" section. To validate this was successful, run the following command:
```bash
ssh git@github.com
```
When asked to confirm the key, type yes and press enter.
### Download the Project
To download the project, first navigate to the desired download location and open a terminal. After this, run the following command:
```bash
git clone git@github.com:HEEV/SupermilageDisplay.git --recursive
```
### Download libusb
To download libusb, run the following commands:
```bash
sudo apt update && \
sudo apt install libusb-1.0-0-dev -y
```
## Build the Project
The project is ready to be compiled! To do so, click on the play button at the bottom of the screen (VScode) or at the top (Visual Studio). Note that the play button at the top right in VSCode will not work.
