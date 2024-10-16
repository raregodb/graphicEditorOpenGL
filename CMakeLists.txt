cmake_minimum_required(VERSION 3.27)
project(graphicRedactor)

set(CMAKE_CXX_STANDARD 14)


# Tell cmake where Qt is located
set(Qt_DIR "/opt/homebrew/opt/qt")

# Tell cmake to find the modules Qt6Core and Qt6widgets
find_package(Qt6 COMPONENTS Core Widgets OpenGLWidgets REQUIRED)


# Execute compilation command
set(SOURCE_FILES main.cpp)

add_executable(graphicRedactor main.cpp)

# Link the library to the executable
target_link_libraries(graphicRedactor Qt6::Core Qt6::Widgets Qt6::OpenGL Qt6::OpenGLWidgets)

# Add header file
set(GLEW_H /opt/homebrew/Cellar/glew/2.2.0_1/include/GL)
set(GLFW_H /opt/homebrew/Cellar/glfw/3.4/include/GLFW)
include_directories(${GLEW_H} ${GLFW_H})

# Add target link
set(GLEW_LINK /opt/homebrew/opt/glew/lib/libGLEW.2.2.dylib)
set(GLFW_LINK /opt/homebrew/Cellar/glfw/3.4/lib/libglfw.3.dylib)
link_libraries(${OPENGL} ${GLEW_LINK} ${GLFW_LINK})

set_target_properties(graphicRedactor PROPERTIES AUTOMOC ON)

if (APPLE)
    target_link_libraries(graphicRedactor "-framework OpenGL")
    target_link_libraries(graphicRedactor "-framework GLUT")
    target_link_libraries(graphicRedactor "-framework QtOpenGL")
endif()


