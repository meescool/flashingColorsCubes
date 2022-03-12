#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

std::string getSourceCode(const char* filename);

class Shader 
{
public:
	GLuint ID;

	Shader(const char* vertexFile, const char* fragmentFile);

	void Start();

	void Delete();

private:
	// Checks if the different Shaders have compiled properly
	void compileErrors(unsigned int shader, const char* type);

};



#endif