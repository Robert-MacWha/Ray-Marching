#include "TextureShader.h"

const char* TextureShader::VERTEX_SHADER_PATH = "textureVertex.vert";
const char* TextureShader::FRAGMENT_SHADER_PATH = "textureFragment.frag";

void TextureShader::bindAttribs() {

	glBindAttribLocation(getProgramID(), 0, "position");
	glBindAttribLocation(getProgramID(), 1, "textureCoords");

}  