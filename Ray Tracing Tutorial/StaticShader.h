#pragma once

#include "ShaderProgram.h"

class StaticShader : public ShaderProgram
{

private:

	static const char* VERTEX_SHADER_PATH;
	static const char* FRAGMENT_SHADER_PATH;

	void bindAttribs();

public:

	StaticShader() : ShaderProgram(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH) { bindAttribs(); };

};

