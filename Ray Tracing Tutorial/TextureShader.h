#pragma once

#include "ShaderProgram.h"

class TextureShader : public ShaderProgram
{

private:

	static const char* VERTEX_SHADER_PATH;
	static const char* FRAGMENT_SHADER_PATH;

	void bindAttribs();

public:

	TextureShader() : ShaderProgram(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH) { bindAttribs(); };

};

