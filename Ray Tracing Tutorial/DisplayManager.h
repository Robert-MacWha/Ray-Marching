
#pragma once

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>    // include GLEW and newest version of GL on windows
#include <GLFW/glfw3.h> // GL helper library

#include <iostream>

using namespace std;

class DisplayManager
{

private:
	
	static GLFWwindow* WINDOW;

public:

	static int WIDTH;
	static int HEIGHT;
	static const char* WINDOW_TITLE;

	static void createDisplay(int w, int h, const char* title);

	static void clearDisplay();
	static void updateDisplay();

	static bool getKey(int key);

	static bool requestWindowClose();
	static void closeDisplay();

};
