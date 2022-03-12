#include "Shader.h"

std::string getSourceCode(const char* filename)
{
	// create file object 
	std::ifstream file;
	std::string sourceCode;

	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		//open the file
		file.open(filename);
		// create stream
		std::stringstream fileStream;
		// read buffer contents into stream
		fileStream << file.rdbuf();
		file.close();

		sourceCode = fileStream.str();
		std::cout << sourceCode << std::endl;
		return sourceCode;
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "Oh no! Shader file not read successfully!" << std::endl;
	}

	//std::ifstream in(filename, std::ios::binary);
	//if (in)
	//{
	//	std::string sourceCode;
	//	in.seekg(0, std::ios::end);
	//	sourceCode.resize(in.tellg());
	//	in.seekg(0, std::ios::beg);
	//	in.read(&sourceCode[0], sourceCode.size());
	//	in.close();
	//	return(sourceCode);
	//}
	//throw(errno);
	
}

Shader::Shader(const char* vertexFile, const char* fragmentFile) {
	// get string from file for vertex
	std::string vertexCode = getSourceCode(vertexFile);
	std::string fragmentCode = getSourceCode(fragmentFile);

	// convert into char the vertex code 
	const char* vertexShaderCode = vertexCode.c_str();
	const char* fragmentShaderCode = fragmentCode.c_str();

	
	// create the vertext shader object
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// attach the vertex shader source code
	glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
	// compile the vertex shader
	glCompileShader(vertexShader);

	// check for errors

	// do the same for the fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragmentShader);

	// create the shader program
	ID = glCreateProgram();
	// attach the shaders
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	// link the shaders to the program
	glLinkProgram(ID);
	// check for compile errors

	// delete the vertex and fragment shader
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Start() {
	glUseProgram(ID);
}

void Shader::Delete() {
	glDeleteProgram(ID);
}


// Checks if the different Shaders have compiled properly
void Shader::compileErrors(unsigned int shader, const char* type)
{
	// Stores status of compilation
	GLint hasCompiled;
	// Character array to store error message in
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}
}
