# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.19

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\persiansph\Persiansph\persiansph\persiansph\lapack-3.5.0\INSTALL

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\persiansph\Persiansph\persiansph\persiansph\LapackBuild\INSTALL

# Include any dependencies generated for this target.
include CMakeFiles/secondtst_INT_ETIME.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/secondtst_INT_ETIME.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/secondtst_INT_ETIME.dir/flags.make

CMakeFiles/secondtst_INT_ETIME.dir/second_INT_ETIME.f.obj: CMakeFiles/secondtst_INT_ETIME.dir/flags.make
CMakeFiles/secondtst_INT_ETIME.dir/second_INT_ETIME.f.obj: D:/persiansph/Persiansph/persiansph/persiansph/lapack-3.5.0/INSTALL/second_INT_ETIME.f
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --progress-dir=D:\persiansph\Persiansph\persiansph\persiansph\LapackBuild\INSTALL\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building Fortran object CMakeFiles/secondtst_INT_ETIME.dir/second_INT_ETIME.f.obj"
	F:\MinGW\bin\mingw32-gfortran.exe $(Fortran_DEFINES) $(Fortran_INCLUDES) $(Fortran_FLAGS) -c D:\persiansph\Persiansph\persiansph\persiansph\lapack-3.5.0\INSTALL\second_INT_ETIME.f -o CMakeFiles\secondtst_INT_ETIME.dir\second_INT_ETIME.f.obj

CMakeFiles/secondtst_INT_ETIME.dir/second_INT_ETIME.f.i: cmake_force
	@echo Preprocessing Fortran source to CMakeFiles/secondtst_INT_ETIME.dir/second_INT_ETIME.f.i
	F:\MinGW\bin\mingw32-gfortran.exe $(Fortran_DEFINES) $(Fortran_INCLUDES) $(Fortran_FLAGS) -E D:\persiansph\Persiansph\persiansph\persiansph\lapack-3.5.0\INSTALL\second_INT_ETIME.f > CMakeFiles\secondtst_INT_ETIME.dir\second_INT_ETIME.f.i

CMakeFiles/secondtst_INT_ETIME.dir/second_INT_ETIME.f.s: cmake_force
	@echo Compiling Fortran source to assembly CMakeFiles/secondtst_INT_ETIME.dir/second_INT_ETIME.f.s
	F:\MinGW\bin\mingw32-gfortran.exe $(Fortran_DEFINES) $(Fortran_INCLUDES) $(Fortran_FLAGS) -S D:\persiansph\Persiansph\persiansph\persiansph\lapack-3.5.0\INSTALL\second_INT_ETIME.f -o CMakeFiles\secondtst_INT_ETIME.dir\second_INT_ETIME.f.s

CMakeFiles/secondtst_INT_ETIME.dir/secondtst.f.obj: CMakeFiles/secondtst_INT_ETIME.dir/flags.make
CMakeFiles/secondtst_INT_ETIME.dir/secondtst.f.obj: D:/persiansph/Persiansph/persiansph/persiansph/lapack-3.5.0/INSTALL/secondtst.f
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --progress-dir=D:\persiansph\Persiansph\persiansph\persiansph\LapackBuild\INSTALL\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building Fortran object CMakeFiles/secondtst_INT_ETIME.dir/secondtst.f.obj"
	F:\MinGW\bin\mingw32-gfortran.exe $(Fortran_DEFINES) $(Fortran_INCLUDES) $(Fortran_FLAGS) -c D:\persiansph\Persiansph\persiansph\persiansph\lapack-3.5.0\INSTALL\secondtst.f -o CMakeFiles\secondtst_INT_ETIME.dir\secondtst.f.obj

CMakeFiles/secondtst_INT_ETIME.dir/secondtst.f.i: cmake_force
	@echo Preprocessing Fortran source to CMakeFiles/secondtst_INT_ETIME.dir/secondtst.f.i
	F:\MinGW\bin\mingw32-gfortran.exe $(Fortran_DEFINES) $(Fortran_INCLUDES) $(Fortran_FLAGS) -E D:\persiansph\Persiansph\persiansph\persiansph\lapack-3.5.0\INSTALL\secondtst.f > CMakeFiles\secondtst_INT_ETIME.dir\secondtst.f.i

CMakeFiles/secondtst_INT_ETIME.dir/secondtst.f.s: cmake_force
	@echo Compiling Fortran source to assembly CMakeFiles/secondtst_INT_ETIME.dir/secondtst.f.s
	F:\MinGW\bin\mingw32-gfortran.exe $(Fortran_DEFINES) $(Fortran_INCLUDES) $(Fortran_FLAGS) -S D:\persiansph\Persiansph\persiansph\persiansph\lapack-3.5.0\INSTALL\secondtst.f -o CMakeFiles\secondtst_INT_ETIME.dir\secondtst.f.s

# Object files for target secondtst_INT_ETIME
secondtst_INT_ETIME_OBJECTS = \
"CMakeFiles/secondtst_INT_ETIME.dir/second_INT_ETIME.f.obj" \
"CMakeFiles/secondtst_INT_ETIME.dir/secondtst.f.obj"

# External object files for target secondtst_INT_ETIME
secondtst_INT_ETIME_EXTERNAL_OBJECTS =

secondtst_INT_ETIME.exe: CMakeFiles/secondtst_INT_ETIME.dir/second_INT_ETIME.f.obj
secondtst_INT_ETIME.exe: CMakeFiles/secondtst_INT_ETIME.dir/secondtst.f.obj
secondtst_INT_ETIME.exe: CMakeFiles/secondtst_INT_ETIME.dir/build.make
secondtst_INT_ETIME.exe: CMakeFiles/secondtst_INT_ETIME.dir/objects1.rsp
secondtst_INT_ETIME.exe: CMakeFiles/secondtst_INT_ETIME.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --progress-dir=D:\persiansph\Persiansph\persiansph\persiansph\LapackBuild\INSTALL\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking Fortran executable secondtst_INT_ETIME.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\secondtst_INT_ETIME.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/secondtst_INT_ETIME.dir/build: secondtst_INT_ETIME.exe

.PHONY : CMakeFiles/secondtst_INT_ETIME.dir/build

CMakeFiles/secondtst_INT_ETIME.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\secondtst_INT_ETIME.dir\cmake_clean.cmake
.PHONY : CMakeFiles/secondtst_INT_ETIME.dir/clean

CMakeFiles/secondtst_INT_ETIME.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\persiansph\Persiansph\persiansph\persiansph\lapack-3.5.0\INSTALL D:\persiansph\Persiansph\persiansph\persiansph\lapack-3.5.0\INSTALL D:\persiansph\Persiansph\persiansph\persiansph\LapackBuild\INSTALL D:\persiansph\Persiansph\persiansph\persiansph\LapackBuild\INSTALL D:\persiansph\Persiansph\persiansph\persiansph\LapackBuild\INSTALL\CMakeFiles\secondtst_INT_ETIME.dir\DependInfo.cmake
.PHONY : CMakeFiles/secondtst_INT_ETIME.dir/depend
