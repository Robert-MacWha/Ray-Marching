#include "Loader.h"

int Loader::bufferCount = 0;

//* LOADERS
// load RawModel
RawModel Loader::loadToVAO(vector<float> positions, vector<int> indices) {

	GLuint vaoID = createVAO();

	// bind the data to the shader as buffers
	bindFloatBuffer(vaoID, positions, 3);

	bindIntBuffer(vaoID, indices, 1);

	return RawModel(vaoID, indices.size(), attribIDs);

}

// load TextureModel
RawModel Loader::loadToVAO(vector<float> positions, vector<float> textureCords, vector<int> indices) {

	GLuint vaoID = createVAO();

	// bind the data to the shader as buffers
	bindFloatBuffer(vaoID, positions, 3);
	bindFloatBuffer(vaoID, textureCords, 2);

	bindIntBuffer(vaoID, indices, 1);

	return RawModel(vaoID, indices.size(), attribIDs);

}

//* DEFAULT MODELS
RawModel Loader::defaultQuad() {

	vector<float> points = { -1.0, 1.0, 0.0,   -1.0, -1.0, 0.0,   1.0, -1.0, 0.0,   1.0, 1.0, 0.0 };
	vector<float> textureCords = { 0, 0,  0, 1,  1, 1,  1, 0 };
	vector<int> indices = { 0, 1, 3,  3, 1, 2 };

	return loadToVAO(points, textureCords, indices);

}

// helpers
GLuint Loader::createVAO() {

	// create the vao
	GLuint vaoID = 0;
	glGenVertexArrays(1, &vaoID);

	return vaoID;

}

// storing data for shaders
void Loader::bindFloatBuffer(int vaoID, vector<float> floats, int attribPerVertex) {

	// create the vbo
	GLuint vboID = 0;
	glGenBuffers(1, &vboID);

	// bind the data to the vbo
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, floats.size() * sizeof(float), floats.data(), GL_STATIC_DRAW);

	// activate the vao
	glBindVertexArray(vaoID);

	// bind the vbo to the vao
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glVertexAttribPointer(bufferCount, attribPerVertex, GL_FLOAT, GL_FALSE, 0, NULL);

	attribIDs.push_back(bufferCount);
	bufferCount += 1;

}

//! NOTE: If you want to use this to bind anything other than indices, you need to create a new function and replace GL_ELEMENT_ARRAY_BUFFER with GL_ARRAY_BUFFER (I think, try it anyways)
void Loader::bindIntBuffer(int vaoID, vector<int> integers, int attribPerVertex) {

	// create the vbo
	GLuint vboID = 0;
	glGenBuffers(1, &vboID);

	// bind the data to the vbo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, integers.size() * sizeof(int), integers.data(), GL_STATIC_DRAW);

	// activate the vao
	glBindVertexArray(vaoID);

	// bind the vbo to the vao
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);
	glVertexAttribPointer(bufferCount, attribPerVertex, GL_INT, GL_FALSE, 0, NULL);
	
	attribIDs.push_back(bufferCount);
	bufferCount += 1;

}