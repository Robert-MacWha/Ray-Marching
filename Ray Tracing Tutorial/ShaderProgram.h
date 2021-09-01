
#pragma once

#include <GL/glew.h>    // include GLEW and newest version of GL on windows
#include <GLFW/glfw3.h> // GL helper library

#include "RawModel.h"
#include <iostream>
#include <fstream>

class ShaderProgram
{

private:

	GLuint programID;
	GLuint vs; // vertex shader
	GLuint fs; // fragment shader

	const char* vertexShaderPath;
	const char* fragmentShaderPath;

	std::string readFromFile(const char* pathToFile);

	GLuint createShaderProgram();
	virtual void bindAttribs() = 0;

	// debugging
	void shaderHasCompiled(GLuint shaderID);
	void printShaderInfoLog(GLuint shaderID);
	void programHasLinked(GLuint programID);
	void printProgramInfoLog(GLuint programID);

public:

	//! TODO: Write a file loader script so the path to the shaders are passed instead of the shaders as strings
	ShaderProgram(const char* vsPath, const char* fsPath);
	void start();
	void reload();

	// getters
	GLuint getProgramID();
	GLuint getVertexShaderID();
	GLuint getFragmentShaderID();

};
