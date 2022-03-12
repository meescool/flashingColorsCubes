#include "VBO.h"

VBO::VBO(GLfloat* vertices, GLsizeiptr size) {
	// generate buffer id
	glGenBuffers(1, &ID);
	// bind the buffer
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	// copy vertex data into buffer memory
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

void VBO::Bind() {
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::Unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete() {
	glDeleteBuffers(1, &ID);
}