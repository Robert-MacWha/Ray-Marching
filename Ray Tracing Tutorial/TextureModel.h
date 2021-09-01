#pragma once

#define GLEW_STATIC
#include <GL/glew.h>    // include GLEW and newest version of GL on windows
#include <GLFW/glfw3.h> // GL helper library

#include "RawModel.h"

class TextureModel
{

private:
	RawModel model;
	GLuint textureID;

public:
	TextureModel(RawModel model, GLuint textureID);

	//* GETTERS
	RawModel getModel();
	GLuint getTextureID();

};
