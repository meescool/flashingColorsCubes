#include "VAO.h"
/// <summary>
/// Class for the Vertex Array Object
/// </summary>
VAO::VAO() 
{
	glGenVertexArrays(1, &ID);
}
/// <summary>
/// 
/// </summary>
/// <param name="VBO"></param>
/// <param name="layout"></param>
/// <param name="numComponents"></param>
/// <param name="type"></param>
/// <param name="stride"></param>
/// <param name="offset"></param>
void VAO::LinkAttrib(VBO VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	VBO.Bind();

	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	
	glEnableVertexAttribArray(layout);
	
	VBO.Unbind();
}

void VAO::Bind()
{
	// binding, make a certain object the current object
	// make the vao the current object by binding it
	glBindVertexArray(ID);

}

void VAO::Unbind()
{
	glBindVertexArray(0);
}
/// <summary>
/// Delete the VAO
/// </summary>
void VAO::Delete()
{
	glDeleteVertexArrays(1, &ID);
}