#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(const char* vsPath, const char* fsPath) {

	vertexShaderPath = vsPath;
	fragmentShaderPath = fsPath;

	programID = createShaderProgram();

}

void ShaderProgram::start() {

	glUseProgram(programID);

}

// hot reload the shaders for this shader program
void ShaderProgram::reload() {

	// create thge new program
	GLuint newProgramID = createShaderProgram();

	// delete the old program to free memory and point the programID pointer to the new shader
	if (newProgramID) {

		glDeleteProgram(programID);
		programID = newProgramID;

	}
	else { fprintf(stderr, "ERROR: Could not create new program"); }

}

GLuint ShaderProgram::createShaderProgram() {

	// load in the shaders
	std::string vStr = readFromFile(vertexShaderPath);
	const char* vertShader = vStr.c_str();

	std::string fStr = readFromFile(fragmentShaderPath);
	const char* fragShader = fStr.c_str();

	// compile the shaders
	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertShader, NULL);
	glCompileShader(vs);
	shaderHasCompiled(vs);

	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragShader, NULL);
	glCompileShader(fs);
	shaderHasCompiled(fs);

	// create the shader program
	GLuint pID = glCreateProgram();
	glAttachShader(pID, fs);
	glAttachShader(pID, vs);
	glLinkProgram(pID);
	programHasLinked(pID);

	// delete the shader handelers to free memory
	glDeleteShader(vs);
	glDeleteShader(fs);

	// return the program;
	return pID;

}

std::string ShaderProgram::readFromFile(const char* pathToFile)
{

	std::ifstream file(pathToFile);
	std::string str((std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>());

	return str;

}

//* GETTERS
GLuint ShaderProgram::getProgramID() {
	
	return programID;

}

GLuint ShaderProgram::getVertexShaderID() {

	return vs;

}

GLuint ShaderProgram::getFragmentShaderID() {

	return fs;

}

//* DEBUG INFO
// shader debug
void ShaderProgram::shaderHasCompiled(GLuint shaderID) {

	int params = -1;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &params);
	if (GL_TRUE != params) {

		fprintf(stderr, "ERROR: GL shader index %i did not compile\n", shaderID);
		printShaderInfoLog(shaderID);

		std::exit(0);

	}

}

void ShaderProgram::printShaderInfoLog(GLuint shaderID) {

	const int maxLen = 2048;
	int actualLen = 0;
	char shaderLog[maxLen];
	glGetShaderInfoLog(shaderID, maxLen, &actualLen, shaderLog);
	printf("Shader info log for GL index %u:\n%s\n", shaderID, shaderLog);

}

// program debug
void ShaderProgram::programHasLinked(GLuint programID) {

	int params = -1;
	glGetProgramiv(programID, GL_LINK_STATUS, &params);
	if (GL_TRUE != params) {

		fprintf(stderr, "ERROR: Could not link shader programme GL index %u\n", programID);
		printProgramInfoLog(programID);

		std::exit(0);

	}

}

void ShaderProgram::printProgramInfoLog(GLuint programID) {

	const int maxLen = 2048;
	int actualLen = 0;
	char programLog[maxLen];
	glGetProgramInfoLog(programID, maxLen, &actualLen, programLog);
	printf("program info log for GL index %u:\n%s", programID, programLog);

}  