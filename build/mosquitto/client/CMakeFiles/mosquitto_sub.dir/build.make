# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/joshua/Documents/repos/SupermilageDisplay

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/joshua/Documents/repos/SupermilageDisplay/build

# Include any dependencies generated for this target.
include mosquitto/client/CMakeFiles/mosquitto_sub.dir/depend.make

# Include the progress variables for this target.
include mosquitto/client/CMakeFiles/mosquitto_sub.dir/progress.make

# Include the compile flags for this target's objects.
include mosquitto/client/CMakeFiles/mosquitto_sub.dir/flags.make

mosquitto/client/CMakeFiles/mosquitto_sub.dir/sub_client.c.o: mosquitto/client/CMakeFiles/mosquitto_sub.dir/flags.make
mosquitto/client/CMakeFiles/mosquitto_sub.dir/sub_client.c.o: ../mosquitto/client/sub_client.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/joshua/Documents/repos/SupermilageDisplay/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object mosquitto/client/CMakeFiles/mosquitto_sub.dir/sub_client.c.o"
	cd /home/joshua/Documents/repos/SupermilageDisplay/build/mosquitto/client && /usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/mosquitto_sub.dir/sub_client.c.o   -c /home/joshua/Documents/repos/SupermilageDisplay/mosquitto/client/sub_client.c

mosquitto/client/CMakeFiles/mosquitto_sub.dir/sub_client.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/mosquitto_sub.dir/sub_client.c.i"
	cd /home/joshua/Documents/repos/SupermilageDisplay/build/mosquitto/client && /usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/joshua/Documents/repos/SupermilageDisplay/mosquitto/client/sub_client.c > CMakeFiles/mosquitto_sub.dir/sub_client.c.i

mosquitto/client/CMakeFiles/mosquitto_sub.dir/sub_client.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/mosquitto_sub.dir/sub_client.c.s"
	cd /home/joshua/Documents/repos/SupermilageDisplay/build/mosquitto/client && /usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/joshua/Documents/repos/SupermilageDisplay/mosquitto/client/sub_client.c -o CMakeFiles/mosquitto_sub.dir/sub_client.c.s

mosquitto/client/CMakeFiles/mosquitto_sub.dir/sub_client_output.c.o: mosquitto/client/CMakeFiles/mosquitto_sub.dir/flags.make
mosquitto/client/CMakeFiles/mosquitto_sub.dir/sub_client_output.c.o: ../mosquitto/client/sub_client_output.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/joshua/Documents/repos/SupermilageDisplay/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object mosquitto/client/CMakeFiles/mosquitto_sub.dir/sub_client_output.c.o"
	cd /home/joshua/Documents/repos/SupermilageDisplay/build/mosquitto/client && /usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/mosquitto_sub.dir/sub_client_output.c.o   -c /home/joshua/Documents/repos/SupermilageDisplay/mosquitto/client/sub_client_output.c

mosquitto/client/CMakeFiles/mosquitto_sub.dir/sub_client_output.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/mosquitto_sub.dir/sub_client_output.c.i"
	cd /home/joshua/Documents/repos/SupermilageDisplay/build/mosquitto/client && /usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/joshua/Documents/repos/SupermilageDisplay/mosquitto/client/sub_client_output.c > CMakeFiles/mosquitto_sub.dir/sub_client_output.c.i

mosquitto/client/CMakeFiles/mosquitto_sub.dir/sub_client_output.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/mosquitto_sub.dir/sub_client_output.c.s"
	cd /home/joshua/Documents/repos/SupermilageDisplay/build/mosquitto/client && /usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/joshua/Documents/repos/SupermilageDisplay/mosquitto/client/sub_client_output.c -o CMakeFiles/mosquitto_sub.dir/sub_client_output.c.s

mosquitto/client/CMakeFiles/mosquitto_sub.dir/client_shared.c.o: mosquitto/client/CMakeFiles/mosquitto_sub.dir/flags.make
mosquitto/client/CMakeFiles/mosquitto_sub.dir/client_shared.c.o: ../mosquitto/client/client_shared.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/joshua/Documents/repos/SupermilageDisplay/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object mosquitto/client/CMakeFiles/mosquitto_sub.dir/client_shared.c.o"
	cd /home/joshua/Documents/repos/SupermilageDisplay/build/mosquitto/client && /usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/mosquitto_sub.dir/client_shared.c.o   -c /home/joshua/Documents/repos/SupermilageDisplay/mosquitto/client/client_shared.c

mosquitto/client/CMakeFiles/mosquitto_sub.dir/client_shared.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/mosquitto_sub.dir/client_shared.c.i"
	cd /home/joshua/Documents/repos/SupermilageDisplay/build/mosquitto/client && /usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/joshua/Documents/repos/SupermilageDisplay/mosquitto/client/client_shared.c > CMakeFiles/mosquitto_sub.dir/client_shared.c.i

mosquitto/client/CMakeFiles/mosquitto_sub.dir/client_shared.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/mosquitto_sub.dir/client_shared.c.s"
	cd /home/joshua/Documents/repos/SupermilageDisplay/build/mosquitto/client && /usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/joshua/Documents/repos/SupermilageDisplay/mosquitto/client/client_shared.c -o CMakeFiles/mosquitto_sub.dir/client_shared.c.s

mosquitto/client/CMakeFiles/mosquitto_sub.dir/client_props.c.o: mosquitto/client/CMakeFiles/mosquitto_sub.dir/flags.make
mosquitto/client/CMakeFiles/mosquitto_sub.dir/client_props.c.o: ../mosquitto/client/client_props.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/joshua/Documents/repos/SupermilageDisplay/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object mosquitto/client/CMakeFiles/mosquitto_sub.dir/client_props.c.o"
	cd /home/joshua/Documents/repos/SupermilageDisplay/build/mosquitto/client && /usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/mosquitto_sub.dir/client_props.c.o   -c /home/joshua/Documents/repos/SupermilageDisplay/mosquitto/client/client_props.c

mosquitto/client/CMakeFiles/mosquitto_sub.dir/client_props.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/mosquitto_sub.dir/client_props.c.i"
	cd /home/joshua/Documents/repos/SupermilageDisplay/build/mosquitto/client && /usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/joshua/Documents/repos/SupermilageDisplay/mosquitto/client/client_props.c > CMakeFiles/mosquitto_sub.dir/client_props.c.i

mosquitto/client/CMakeFiles/mosquitto_sub.dir/client_props.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/mosquitto_sub.dir/client_props.c.s"
	cd /home/joshua/Documents/repos/SupermilageDisplay/build/mosquitto/client && /usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/joshua/Documents/repos/SupermilageDisplay/mosquitto/client/client_props.c -o CMakeFiles/mosquitto_sub.dir/client_props.c.s

# Object files for target mosquitto_sub
mosquitto_sub_OBJECTS = \
"CMakeFiles/mosquitto_sub.dir/sub_client.c.o" \
"CMakeFiles/mosquitto_sub.dir/sub_client_output.c.o" \
"CMakeFiles/mosquitto_sub.dir/client_shared.c.o" \
"CMakeFiles/mosquitto_sub.dir/client_props.c.o"

# External object files for target mosquitto_sub
mosquitto_sub_EXTERNAL_OBJECTS =

mosquitto/client/mosquitto_sub: mosquitto/client/CMakeFiles/mosquitto_sub.dir/sub_client.c.o
mosquitto/client/mosquitto_sub: mosquitto/client/CMakeFiles/mosquitto_sub.dir/sub_client_output.c.o
mosquitto/client/mosquitto_sub: mosquitto/client/CMakeFiles/mosquitto_sub.dir/client_shared.c.o
mosquitto/client/mosquitto_sub: mosquitto/client/CMakeFiles/mosquitto_sub.dir/client_props.c.o
mosquitto/client/mosquitto_sub: mosquitto/client/CMakeFiles/mosquitto_sub.dir/build.make
mosquitto/client/mosquitto_sub: mosquitto/lib/libmosquitto_static.a
mosquitto/client/mosquitto_sub: mosquitto/client/CMakeFiles/mosquitto_sub.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/joshua/Documents/repos/SupermilageDisplay/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking C executable mosquitto_sub"
	cd /home/joshua/Documents/repos/SupermilageDisplay/build/mosquitto/client && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mosquitto_sub.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
mosquitto/client/CMakeFiles/mosquitto_sub.dir/build: mosquitto/client/mosquitto_sub

.PHONY : mosquitto/client/CMakeFiles/mosquitto_sub.dir/build

mosquitto/client/CMakeFiles/mosquitto_sub.dir/clean:
	cd /home/joshua/Documents/repos/SupermilageDisplay/build/mosquitto/client && $(CMAKE_COMMAND) -P CMakeFiles/mosquitto_sub.dir/cmake_clean.cmake
.PHONY : mosquitto/client/CMakeFiles/mosquitto_sub.dir/clean

mosquitto/client/CMakeFiles/mosquitto_sub.dir/depend:
	cd /home/joshua/Documents/repos/SupermilageDisplay/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/joshua/Documents/repos/SupermilageDisplay /home/joshua/Documents/repos/SupermilageDisplay/mosquitto/client /home/joshua/Documents/repos/SupermilageDisplay/build /home/joshua/Documents/repos/SupermilageDisplay/build/mosquitto/client /home/joshua/Documents/repos/SupermilageDisplay/build/mosquitto/client/CMakeFiles/mosquitto_sub.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : mosquitto/client/CMakeFiles/mosquitto_sub.dir/depend

