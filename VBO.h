#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glad/glad.h>

#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

class VBO
{
public: 
	GLuint ID;

	VBO(GLfloat* vertices, GLsizeiptr size);

	void Bind();
	void Unbind();
	void Delete();
};

#endif