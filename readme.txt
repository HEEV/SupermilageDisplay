SUPERMILAGE DISPLAY

SYNOPSIS:
	This is a GUI application built using the JUCE C++ library (https://juce.com)
to serve as a data display for both the Supermilage Car as well as the ground crew for
Cedarville University's Supermilage competition team.

BUILD INSTRUCTIONS:
	JUCE projects are built using the Projucer, an application used to export
JUCE project files into a build system (Visual Studio, Makefile, XCode, etc.).
As of right now, only the source code is tracked in git (this may change in the
future; if so, this text should likewise change). So, to build, I would
recommend making a Projucer project on your local machine with the source files
contained in this repo (see src/ directory) and exporting to your build system
of choice. DO NOT upload an exported Projucer project to this repo -- I'm
warning you right now, they're massive (upwards of 1 GB)!
	Download Projucer from the website (https://juce.com/discover/projucer), 
and select your prefered buildsystem (you could do this in your same repo folder, 
just make sure to add the build to your gitignore!). Make sure in your global
variables settings in Projucer that the path to your downloaded JUCE library files
is accurate! Generate a new project, and delete that which projucer
generates inside your selected IDE's build folder. Rename your project in Projucer,
and then save. From there, build in your IDE/build system.
	***You also need the JUCE GUI extras to build this project***

CURRENTLY WORKING:
	- A "Speedometer" widget to display speeds and the like.

NEEDS WORK:
	- The display layout.
	- A progress bar widget for lap progress.
