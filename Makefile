# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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
CMAKE_SOURCE_DIR = /home/josh/Documents/PascalCompiler

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/josh/Documents/PascalCompiler

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/josh/Documents/PascalCompiler/CMakeFiles /home/josh/Documents/PascalCompiler/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/josh/Documents/PascalCompiler/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named PascalCompiler

# Build rule for target.
PascalCompiler: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 PascalCompiler
.PHONY : PascalCompiler

# fast build rule for target.
PascalCompiler/fast:
	$(MAKE) -f CMakeFiles/PascalCompiler.dir/build.make CMakeFiles/PascalCompiler.dir/build
.PHONY : PascalCompiler/fast

plc.o: plc.cpp.o

.PHONY : plc.o

# target to build an object file
plc.cpp.o:
	$(MAKE) -f CMakeFiles/PascalCompiler.dir/build.make CMakeFiles/PascalCompiler.dir/plc.cpp.o
.PHONY : plc.cpp.o

plc.i: plc.cpp.i

.PHONY : plc.i

# target to preprocess a source file
plc.cpp.i:
	$(MAKE) -f CMakeFiles/PascalCompiler.dir/build.make CMakeFiles/PascalCompiler.dir/plc.cpp.i
.PHONY : plc.cpp.i

plc.s: plc.cpp.s

.PHONY : plc.s

# target to generate assembly for a file
plc.cpp.s:
	$(MAKE) -f CMakeFiles/PascalCompiler.dir/build.make CMakeFiles/PascalCompiler.dir/plc.cpp.s
.PHONY : plc.cpp.s

src/Administration.o: src/Administration.cpp.o

.PHONY : src/Administration.o

# target to build an object file
src/Administration.cpp.o:
	$(MAKE) -f CMakeFiles/PascalCompiler.dir/build.make CMakeFiles/PascalCompiler.dir/src/Administration.cpp.o
.PHONY : src/Administration.cpp.o

src/Administration.i: src/Administration.cpp.i

.PHONY : src/Administration.i

# target to preprocess a source file
src/Administration.cpp.i:
	$(MAKE) -f CMakeFiles/PascalCompiler.dir/build.make CMakeFiles/PascalCompiler.dir/src/Administration.cpp.i
.PHONY : src/Administration.cpp.i

src/Administration.s: src/Administration.cpp.s

.PHONY : src/Administration.s

# target to generate assembly for a file
src/Administration.cpp.s:
	$(MAKE) -f CMakeFiles/PascalCompiler.dir/build.make CMakeFiles/PascalCompiler.dir/src/Administration.cpp.s
.PHONY : src/Administration.cpp.s

src/Parser.o: src/Parser.cpp.o

.PHONY : src/Parser.o

# target to build an object file
src/Parser.cpp.o:
	$(MAKE) -f CMakeFiles/PascalCompiler.dir/build.make CMakeFiles/PascalCompiler.dir/src/Parser.cpp.o
.PHONY : src/Parser.cpp.o

src/Parser.i: src/Parser.cpp.i

.PHONY : src/Parser.i

# target to preprocess a source file
src/Parser.cpp.i:
	$(MAKE) -f CMakeFiles/PascalCompiler.dir/build.make CMakeFiles/PascalCompiler.dir/src/Parser.cpp.i
.PHONY : src/Parser.cpp.i

src/Parser.s: src/Parser.cpp.s

.PHONY : src/Parser.s

# target to generate assembly for a file
src/Parser.cpp.s:
	$(MAKE) -f CMakeFiles/PascalCompiler.dir/build.make CMakeFiles/PascalCompiler.dir/src/Parser.cpp.s
.PHONY : src/Parser.cpp.s

src/Scanner.o: src/Scanner.cpp.o

.PHONY : src/Scanner.o

# target to build an object file
src/Scanner.cpp.o:
	$(MAKE) -f CMakeFiles/PascalCompiler.dir/build.make CMakeFiles/PascalCompiler.dir/src/Scanner.cpp.o
.PHONY : src/Scanner.cpp.o

src/Scanner.i: src/Scanner.cpp.i

.PHONY : src/Scanner.i

# target to preprocess a source file
src/Scanner.cpp.i:
	$(MAKE) -f CMakeFiles/PascalCompiler.dir/build.make CMakeFiles/PascalCompiler.dir/src/Scanner.cpp.i
.PHONY : src/Scanner.cpp.i

src/Scanner.s: src/Scanner.cpp.s

.PHONY : src/Scanner.s

# target to generate assembly for a file
src/Scanner.cpp.s:
	$(MAKE) -f CMakeFiles/PascalCompiler.dir/build.make CMakeFiles/PascalCompiler.dir/src/Scanner.cpp.s
.PHONY : src/Scanner.cpp.s

src/SymbolTable.o: src/SymbolTable.cpp.o

.PHONY : src/SymbolTable.o

# target to build an object file
src/SymbolTable.cpp.o:
	$(MAKE) -f CMakeFiles/PascalCompiler.dir/build.make CMakeFiles/PascalCompiler.dir/src/SymbolTable.cpp.o
.PHONY : src/SymbolTable.cpp.o

src/SymbolTable.i: src/SymbolTable.cpp.i

.PHONY : src/SymbolTable.i

# target to preprocess a source file
src/SymbolTable.cpp.i:
	$(MAKE) -f CMakeFiles/PascalCompiler.dir/build.make CMakeFiles/PascalCompiler.dir/src/SymbolTable.cpp.i
.PHONY : src/SymbolTable.cpp.i

src/SymbolTable.s: src/SymbolTable.cpp.s

.PHONY : src/SymbolTable.s

# target to generate assembly for a file
src/SymbolTable.cpp.s:
	$(MAKE) -f CMakeFiles/PascalCompiler.dir/build.make CMakeFiles/PascalCompiler.dir/src/SymbolTable.cpp.s
.PHONY : src/SymbolTable.cpp.s

src/Token.o: src/Token.cpp.o

.PHONY : src/Token.o

# target to build an object file
src/Token.cpp.o:
	$(MAKE) -f CMakeFiles/PascalCompiler.dir/build.make CMakeFiles/PascalCompiler.dir/src/Token.cpp.o
.PHONY : src/Token.cpp.o

src/Token.i: src/Token.cpp.i

.PHONY : src/Token.i

# target to preprocess a source file
src/Token.cpp.i:
	$(MAKE) -f CMakeFiles/PascalCompiler.dir/build.make CMakeFiles/PascalCompiler.dir/src/Token.cpp.i
.PHONY : src/Token.cpp.i

src/Token.s: src/Token.cpp.s

.PHONY : src/Token.s

# target to generate assembly for a file
src/Token.cpp.s:
	$(MAKE) -f CMakeFiles/PascalCompiler.dir/build.make CMakeFiles/PascalCompiler.dir/src/Token.cpp.s
.PHONY : src/Token.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... PascalCompiler"
	@echo "... edit_cache"
	@echo "... plc.o"
	@echo "... plc.i"
	@echo "... plc.s"
	@echo "... src/Administration.o"
	@echo "... src/Administration.i"
	@echo "... src/Administration.s"
	@echo "... src/Parser.o"
	@echo "... src/Parser.i"
	@echo "... src/Parser.s"
	@echo "... src/Scanner.o"
	@echo "... src/Scanner.i"
	@echo "... src/Scanner.s"
	@echo "... src/SymbolTable.o"
	@echo "... src/SymbolTable.i"
	@echo "... src/SymbolTable.s"
	@echo "... src/Token.o"
	@echo "... src/Token.i"
	@echo "... src/Token.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

