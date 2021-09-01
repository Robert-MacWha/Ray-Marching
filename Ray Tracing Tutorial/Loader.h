#pragma once

#define GLEW_STATIC
#include <GL/glew.h>    // include GLEW and newest version of GL on windows
#include <GLFW/glfw3.h> // GL helper library

#include <vector>

#include "RawModel.h"

using namespace std;

class Loader {

private:

	static int bufferCount;
	vector<int> attribIDs;

	GLuint createVAO();
	void bindFloatBuffer(int vaoID, vector<float> floats, int attribPerVertex);
	void bindIntBuffer(int vaoID, vector<int> integers, int attribPerVertex);

public:

	RawModel loadToVAO(vector<float> positions, vector<int> indices);
	RawModel loadToVAO(vector<float> positions, vector<float> textureCords, vector<int> indices);
	
	// defaults
	RawModel defaultQuad();

};