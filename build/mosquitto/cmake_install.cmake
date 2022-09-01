# Install script for directory: /home/joshua/Documents/repos/SupermilageDisplay/mosquitto

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/etc/mosquitto" TYPE FILE FILES
    "/home/joshua/Documents/repos/SupermilageDisplay/mosquitto/mosquitto.conf"
    "/home/joshua/Documents/repos/SupermilageDisplay/mosquitto/aclfile.example"
    "/home/joshua/Documents/repos/SupermilageDisplay/mosquitto/pskfile.example"
    "/home/joshua/Documents/repos/SupermilageDisplay/mosquitto/pwfile.example"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/joshua/Documents/repos/SupermilageDisplay/build/mosquitto/libmosquitto.pc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/joshua/Documents/repos/SupermilageDisplay/build/mosquitto/libmosquittopp.pc")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/joshua/Documents/repos/SupermilageDisplay/build/mosquitto/lib/cmake_install.cmake")
  include("/home/joshua/Documents/repos/SupermilageDisplay/build/mosquitto/client/cmake_install.cmake")
  include("/home/joshua/Documents/repos/SupermilageDisplay/build/mosquitto/src/cmake_install.cmake")
  include("/home/joshua/Documents/repos/SupermilageDisplay/build/mosquitto/apps/cmake_install.cmake")
  include("/home/joshua/Documents/repos/SupermilageDisplay/build/mosquitto/plugins/cmake_install.cmake")

endif()

