# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/aarch64/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/aarch64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/raregod/Documents/5sem/graphics/graphicEditorOpenGL-4_iteration

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/raregod/Documents/5sem/graphics/graphicEditorOpenGL-4_iteration/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/graphicEditor.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/graphicEditor.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/graphicEditor.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/graphicEditor.dir/flags.make

CMakeFiles/graphicEditor.dir/graphicEditor_autogen/mocs_compilation.cpp.o: CMakeFiles/graphicEditor.dir/flags.make
CMakeFiles/graphicEditor.dir/graphicEditor_autogen/mocs_compilation.cpp.o: graphicEditor_autogen/mocs_compilation.cpp
CMakeFiles/graphicEditor.dir/graphicEditor_autogen/mocs_compilation.cpp.o: CMakeFiles/graphicEditor.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/raregod/Documents/5sem/graphics/graphicEditorOpenGL-4_iteration/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/graphicEditor.dir/graphicEditor_autogen/mocs_compilation.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/graphicEditor.dir/graphicEditor_autogen/mocs_compilation.cpp.o -MF CMakeFiles/graphicEditor.dir/graphicEditor_autogen/mocs_compilation.cpp.o.d -o CMakeFiles/graphicEditor.dir/graphicEditor_autogen/mocs_compilation.cpp.o -c /Users/raregod/Documents/5sem/graphics/graphicEditorOpenGL-4_iteration/cmake-build-debug/graphicEditor_autogen/mocs_compilation.cpp

CMakeFiles/graphicEditor.dir/graphicEditor_autogen/mocs_compilation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/graphicEditor.dir/graphicEditor_autogen/mocs_compilation.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/raregod/Documents/5sem/graphics/graphicEditorOpenGL-4_iteration/cmake-build-debug/graphicEditor_autogen/mocs_compilation.cpp > CMakeFiles/graphicEditor.dir/graphicEditor_autogen/mocs_compilation.cpp.i

CMakeFiles/graphicEditor.dir/graphicEditor_autogen/mocs_compilation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/graphicEditor.dir/graphicEditor_autogen/mocs_compilation.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/raregod/Documents/5sem/graphics/graphicEditorOpenGL-4_iteration/cmake-build-debug/graphicEditor_autogen/mocs_compilation.cpp -o CMakeFiles/graphicEditor.dir/graphicEditor_autogen/mocs_compilation.cpp.s

CMakeFiles/graphicEditor.dir/main.cpp.o: CMakeFiles/graphicEditor.dir/flags.make
CMakeFiles/graphicEditor.dir/main.cpp.o: /Users/raregod/Documents/5sem/graphics/graphicEditorOpenGL-4_iteration/main.cpp
CMakeFiles/graphicEditor.dir/main.cpp.o: CMakeFiles/graphicEditor.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/raregod/Documents/5sem/graphics/graphicEditorOpenGL-4_iteration/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/graphicEditor.dir/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/graphicEditor.dir/main.cpp.o -MF CMakeFiles/graphicEditor.dir/main.cpp.o.d -o CMakeFiles/graphicEditor.dir/main.cpp.o -c /Users/raregod/Documents/5sem/graphics/graphicEditorOpenGL-4_iteration/main.cpp

CMakeFiles/graphicEditor.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/graphicEditor.dir/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/raregod/Documents/5sem/graphics/graphicEditorOpenGL-4_iteration/main.cpp > CMakeFiles/graphicEditor.dir/main.cpp.i

CMakeFiles/graphicEditor.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/graphicEditor.dir/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/raregod/Documents/5sem/graphics/graphicEditorOpenGL-4_iteration/main.cpp -o CMakeFiles/graphicEditor.dir/main.cpp.s

CMakeFiles/graphicEditor.dir/MyGLWidget.cpp.o: CMakeFiles/graphicEditor.dir/flags.make
CMakeFiles/graphicEditor.dir/MyGLWidget.cpp.o: /Users/raregod/Documents/5sem/graphics/graphicEditorOpenGL-4_iteration/MyGLWidget.cpp
CMakeFiles/graphicEditor.dir/MyGLWidget.cpp.o: CMakeFiles/graphicEditor.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/raregod/Documents/5sem/graphics/graphicEditorOpenGL-4_iteration/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/graphicEditor.dir/MyGLWidget.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/graphicEditor.dir/MyGLWidget.cpp.o -MF CMakeFiles/graphicEditor.dir/MyGLWidget.cpp.o.d -o CMakeFiles/graphicEditor.dir/MyGLWidget.cpp.o -c /Users/raregod/Documents/5sem/graphics/graphicEditorOpenGL-4_iteration/MyGLWidget.cpp

CMakeFiles/graphicEditor.dir/MyGLWidget.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/graphicEditor.dir/MyGLWidget.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/raregod/Documents/5sem/graphics/graphicEditorOpenGL-4_iteration/MyGLWidget.cpp > CMakeFiles/graphicEditor.dir/MyGLWidget.cpp.i

CMakeFiles/graphicEditor.dir/MyGLWidget.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/graphicEditor.dir/MyGLWidget.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/raregod/Documents/5sem/graphics/graphicEditorOpenGL-4_iteration/MyGLWidget.cpp -o CMakeFiles/graphicEditor.dir/MyGLWidget.cpp.s

# Object files for target graphicEditor
graphicEditor_OBJECTS = \
"CMakeFiles/graphicEditor.dir/graphicEditor_autogen/mocs_compilation.cpp.o" \
"CMakeFiles/graphicEditor.dir/main.cpp.o" \
"CMakeFiles/graphicEditor.dir/MyGLWidget.cpp.o"

# External object files for target graphicEditor
graphicEditor_EXTERNAL_OBJECTS =

graphicEditor: CMakeFiles/graphicEditor.dir/graphicEditor_autogen/mocs_compilation.cpp.o
graphicEditor: CMakeFiles/graphicEditor.dir/main.cpp.o
graphicEditor: CMakeFiles/graphicEditor.dir/MyGLWidget.cpp.o
graphicEditor: CMakeFiles/graphicEditor.dir/build.make
graphicEditor: /opt/homebrew/lib/QtOpenGLWidgets.framework/Versions/A/QtOpenGLWidgets
graphicEditor: /opt/homebrew/lib/QtWidgets.framework/Versions/A/QtWidgets
graphicEditor: /opt/homebrew/lib/QtOpenGL.framework/Versions/A/QtOpenGL
graphicEditor: /opt/homebrew/lib/QtGui.framework/Versions/A/QtGui
graphicEditor: /opt/homebrew/lib/QtCore.framework/Versions/A/QtCore
graphicEditor: CMakeFiles/graphicEditor.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/raregod/Documents/5sem/graphics/graphicEditorOpenGL-4_iteration/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable graphicEditor"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/graphicEditor.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/graphicEditor.dir/build: graphicEditor
.PHONY : CMakeFiles/graphicEditor.dir/build

CMakeFiles/graphicEditor.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/graphicEditor.dir/cmake_clean.cmake
.PHONY : CMakeFiles/graphicEditor.dir/clean

CMakeFiles/graphicEditor.dir/depend:
	cd /Users/raregod/Documents/5sem/graphics/graphicEditorOpenGL-4_iteration/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/raregod/Documents/5sem/graphics/graphicEditorOpenGL-4_iteration /Users/raregod/Documents/5sem/graphics/graphicEditorOpenGL-4_iteration /Users/raregod/Documents/5sem/graphics/graphicEditorOpenGL-4_iteration/cmake-build-debug /Users/raregod/Documents/5sem/graphics/graphicEditorOpenGL-4_iteration/cmake-build-debug /Users/raregod/Documents/5sem/graphics/graphicEditorOpenGL-4_iteration/cmake-build-debug/CMakeFiles/graphicEditor.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/graphicEditor.dir/depend

