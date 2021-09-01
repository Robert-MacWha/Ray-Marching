#pragma once

#define GLEW_STATIC
#include <GL/glew.h>    // include GLEW and newest version of GL on windows
#include <GLFW/glfw3.h> // GL helper library

#include <iostream>
#include <fstream>

using namespace std;

class ComputeShader
{

private:

	const char* shaderPath;
	
	GLuint programID;
	GLuint shaderID;

	string readFromFile(const char* shaderPath);
	GLuint createShaderProgram();

	//* DEBUG INFO
	void shaderHasCompiled(GLuint shaderID);
	void printShaderInfoLog(GLuint shaderID);
	void programHasLinked(GLuint programID);
	void printProgramInfoLog(GLuint programID);

public:

	ComputeShader(const char* shaderPath);
	void use();
	void dispatch(GLuint x, GLuint y, GLuint z);
	void reload();

	GLint getUniformLocation(const char* uniform);


};

