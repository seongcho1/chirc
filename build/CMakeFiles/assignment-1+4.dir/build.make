# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.23

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Users/seongcho/.brew/Cellar/cmake/3.23.2/bin/cmake

# The command to remove a file.
RM = /Users/seongcho/.brew/Cellar/cmake/3.23.2/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/seongcho/5Circle/chirc

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/seongcho/5Circle/chirc/build

# Utility rule file for assignment-1+4.

# Include any custom commands dependencies for this target.
include CMakeFiles/assignment-1+4.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/assignment-1+4.dir/progress.make

CMakeFiles/assignment-1+4: chirc
	/bin/sh -c 'pytest --chirc-rubric ../tests/rubrics/assignment-1+4.json ../tests/; exit 0'
	../tests/grade.py ../tests/rubrics/assignment-1+4.json

assignment-1+4: CMakeFiles/assignment-1+4
assignment-1+4: CMakeFiles/assignment-1+4.dir/build.make
.PHONY : assignment-1+4

# Rule to build all files generated by this target.
CMakeFiles/assignment-1+4.dir/build: assignment-1+4
.PHONY : CMakeFiles/assignment-1+4.dir/build

CMakeFiles/assignment-1+4.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/assignment-1+4.dir/cmake_clean.cmake
.PHONY : CMakeFiles/assignment-1+4.dir/clean

CMakeFiles/assignment-1+4.dir/depend:
	cd /Users/seongcho/5Circle/chirc/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/seongcho/5Circle/chirc /Users/seongcho/5Circle/chirc /Users/seongcho/5Circle/chirc/build /Users/seongcho/5Circle/chirc/build /Users/seongcho/5Circle/chirc/build/CMakeFiles/assignment-1+4.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/assignment-1+4.dir/depend

