
#pragma once

#include <GL/glew.h>    // include GLEW and newest version of GL on windows
#include <GLFW/glfw3.h> // GL helper library

#include <vector>

#include "RawModel.h"
#include "TextureModel.h"

class Renderer
{

private:
	void enableAttribs(vector<int> attribIDs);
	void disableAttribs(vector<int> attribIDs);

public:

	void Render(RawModel model);
	void Render(TextureModel textureModel);

};

