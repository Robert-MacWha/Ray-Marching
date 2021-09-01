#include "StaticShader.h"

const char* StaticShader::VERTEX_SHADER_PATH = "rawVertex.vert";
const char* StaticShader::FRAGMENT_SHADER_PATH = "rawFragment.frag";

void StaticShader::bindAttribs() {

	glBindAttribLocation(getProgramID(), 0, "vertexPosition");

}