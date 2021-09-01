#pragma once
#include <GL/glew.h>    // include GLEW and newest version of GL on windows
#include <GLFW/glfw3.h> // GL helper library
#include "stb_image.h"

#include <iostream>

using namespace std;

class TextureManager
{

public:

	static GLuint createTexture(int w, int h, int binding);
	static GLuint loadTexture(const char* path, int binding);
	static GLuint createColorTexture(float r, float b, float g);

};
