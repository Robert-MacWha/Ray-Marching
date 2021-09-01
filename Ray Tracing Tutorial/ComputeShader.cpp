#include "ComputeShader.h"

ComputeShader::ComputeShader(const char* shaderPath) {

	ComputeShader::shaderPath = shaderPath;

	ComputeShader::programID = createShaderProgram();

}

GLuint ComputeShader::createShaderProgram() {

	// load in the shader
	string shaderStr = readFromFile(shaderPath);
	const char* shader = shaderStr.c_str();

	// compile the shader
	shaderID = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(shaderID, 1, &shader, NULL);
	glCompileShader(shaderID);
	shaderHasCompiled(shaderID);

	// create the shader program
	GLuint pID = glCreateProgram();
	glAttachShader(pID, shaderID);
	glLinkProgram(pID);
	programHasLinked(pID);

	// delete shader handelers
	glDeleteShader(shaderID);

	return pID;

}

void ComputeShader::use() {

	glUseProgram(programID);

}

void ComputeShader::dispatch(GLuint x, GLuint y, GLuint z) {

	// launch the compute shader
	glDispatchCompute(x, y, z);

	// delay the program until the compute shader is done
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}
// hot reload the shaders for this shader program
void ComputeShader::reload() {

	// create thge new program
	GLuint newProgramID = createShaderProgram();

	// delete the old program to free memory and point the programID pointer to the new shader
	if (newProgramID) {

		glDeleteProgram(programID);
		programID = newProgramID;

	}
	else { fprintf(stderr, "ERROR: Could not create new program"); }

}

GLint ComputeShader::getUniformLocation(const char* uniform) {

	return glGetUniformLocation(programID, uniform);

}

string ComputeShader::readFromFile(const char* pathToFile)
{

	std::ifstream file(pathToFile);
	std::string str((istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>());

	return str;

}

//* DEBUG INFO
// shader debug
void ComputeShader::shaderHasCompiled(GLuint shaderID) {

	int params = -1;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &params);
	if (GL_TRUE != params) {

		fprintf(stderr, "ERROR: GL shader index %i did not compile\n", shaderID);
		printShaderInfoLog(shaderID);

		std::exit(0);

	}

}

void ComputeShader::printShaderInfoLog(GLuint shaderID) {

	const int maxLen = 2048;
	int actualLen = 0;
	char shaderLog[maxLen];
	glGetShaderInfoLog(shaderID, maxLen, &actualLen, shaderLog);
	printf("Shader info log for GL index %u:\n%s\n", shaderID, shaderLog);

}

// program debug
void ComputeShader::programHasLinked(GLuint programID) {

	int params = -1;
	glGetProgramiv(programID, GL_LINK_STATUS, &params);
	if (GL_TRUE != params) {

		fprintf(stderr, "ERROR: Could not link shader programme GL index %u\n", programID);
		printProgramInfoLog(programID);

		std::exit(0);

	}

}

void ComputeShader::printProgramInfoLog(GLuint programID) {

	const int maxLen = 2048;
	int actualLen = 0;
	char programLog[maxLen];
	glGetProgramInfoLog(programID, maxLen, &actualLen, programLog);
	printf("program info log for GL index %u:\n%s", programID, programLog);

}