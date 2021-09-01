Copy this folder into the projects/c++ directory.  To create a new opengl project open visual studio 2019 and create a empty project in the copied folder.
Visual studio will then create a project folder beside the external resources folder.

To link opengl to the project:
 - Create a main.cpp file
 - Right click on the project name under the solution window and go to properties.
 - Select 'all configurations' as the configuration and 'x64' as the platform
 - Open the c/c++/general tab on the left side
 - Select 'Additional include directories' and click edit
 - Copy in the following:
$(SolutionDir)/../external resources/GLEW/include
$(SolutionDir)/../external resources/GLFW/include

 - Next go to linker/general
 - Select 'Additional Library Directories' and click edit
 - Copy in the following:
$(SolutionDir)/../external resources/GLEW/lib/Release/x64
$(SolutionDir)/../external resources/GLFW/lib-vc2019

 - Finally go to linker/input
 - Select 'Additional Dependencies' and click edit
 - Copy in the following:
opengl32.lib
glew32s.lib
glfw3.lib

 - see if everything works by pasting in these lines of code:
#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

 - Note: If the include doesn't work try to change the solution platform to x64 (dropdown beside the run button)