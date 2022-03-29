/*
* @author	Marcela Estrada
* @desc		A computer animation of rectangular cubes stacked in the form of a pyramid
* @source	I followed tutorials from https://learnopengl.com/ 
*/

#include <iostream>
#include <glad/glad.h>  // this needs to go before GLFW
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp> 
#include <glm/glm/gtc/matrix_transform.hpp> 
#include <glm/glm/gtc/type_ptr.hpp> 

#include"Shader.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"

// resize frame buffer to fit the window
void resizeFrameBuffer(GLFWwindow* window, int width, int height);
// process key inputs
void processInput(GLFWwindow* window, GLuint* size, int* myKeys);

// safer to use OPENGL version of floats
// give the coordinates of the vertices of a triangle
GLfloat vertices[] =
{ //			coordinates	   /				colors				//
	-0.5f,	 0.5f,		-0.5f,	0.3f,	0.0f,	0.2f,	// lower left corner
	 0.5f,	 0.5f,		-0.5f,	0.0f,	0.1f,	0.2f,	// lower right corner
	-0.5f,	-0.5f,		-0.5f,	0.2f,	0.1f,	0.2f,	// upper corner
	 0.5f,  -0.5f,		-0.5f,	0.2f,	0.1f,	0.2f,	// inner left
	
    -0.5f,	 0.5f,		0.5f,	0.9f,	0.9f,	1.0f,	// lower left corner
	 0.5f,	 0.5f,		0.5f,	0.9f,	0.1f,	0.9f,	// lower right corner
	-0.5f,	-0.5f,		0.5f,	0.9f,	0.1f,	0.9f,	// upper corner
	 0.5f,  -0.5f,		0.5f,	0.9f,	0.1f,	0.9f,	// inner left

	 0.5f,	-0.5f,		-0.5f,	0.9f,	0.9f,	1.0f,	// lower left corner
	 0.5f,	-0.5f,		0.5f,	0.9f,	0.1f,	0.9f,	// lower right corner
	 0.5f,	 0.5f,		-0.5f,	0.9f,	0.1f,	0.9f,	// upper corner
	 0.5f,   0.5f,		0.5f,	0.9f,	0.1f,	0.9f,	// inner left

};

// indicated with vertices to use to create the square
GLuint indices[] =
{
	0, 2, 3, 
	0, 3, 1,
	2, 6, 7, 
	2, 7, 3,
	6, 4, 5, 
	6, 5, 7,
	4, 0, 1, 
	4, 1, 5,
	0, 4, 6, 
	0, 6, 2,
	1, 5, 7, 
	1, 7, 3,

};

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(600, 600, "Flashing Colors", NULL, NULL);
    
	if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // initializing glad, setting checks in case it doesn't load
	if (gladLoadGL() != 1) {
		std::cout << "Failed to initialize Glad!!! </3"<< std::endl;
		return -1;
	}


    glViewport(0, 0, 600, 600);

    // for resizing the window
    glfwSetFramebufferSizeCallback(window, resizeFrameBuffer);

	// setting up 3D 
	// setting up model matrix
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	// setting up view matrix
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -15.0f));
	// setting up projection matrix
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	
	// create the shader program
	Shader shaderProgram("default.vert", "default.frag");

	// generates vertex array object and binds it
	VAO VAO1;
	VAO1.Bind();

	// generates vertex buffer object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// generates element buffer object and links it to indices
	EBO EBO1(indices, sizeof(indices));

	// links vbo to vao
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	// unbind all objects
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// set positions for cubes 
	glm::vec3 cubePos[] = {
			glm::vec3(-1.0f, -2.0f, -3.0f),
			glm::vec3(-1.0f, -1.0f, -3.0f),
			glm::vec3(-1.0f,  0.0f, -3.0f),
			glm::vec3(-1.0f,  1.0f, -3.0f),
			glm::vec3(-1.0f,  2.0f, -3.0f),
			glm::vec3(-1.0f,  3.0f, -3.0f),
	};



	// uniforms for creating new squares
	GLuint mxID = glGetUniformLocation(shaderProgram.ID, "moveX");
	GLuint myID = glGetUniformLocation(shaderProgram.ID, "moveY");

	// uniform for changing color
	GLuint cID = glGetUniformLocation(shaderProgram.ID, "myColor");

	// uniforms for 3D
	GLuint modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	
	GLuint viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	GLuint projLoc = glGetUniformLocation(shaderProgram.ID, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// temp variables for uniforms
	float x = 0; // width
	float y = 0; // height
	float c = 0; // color
	float time = 0;
	GLuint counter = 0;
	float angle = 0.0f;
	GLboolean rewind = false;
	// sizes of the squares
	GLuint size[2] = { 1,1 };
	// up,down,left,right
	int myKeys[4] = {
		false,
		false,
		false,
		false
	};

	glEnable(GL_DEPTH_TEST);

	// setting initial color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    while (!glfwWindowShouldClose(window))
    {
		if (time >= 200.0f) 
		{
			time = 0.1f;
			if (rewind == true) {
				counter--;
				if (counter == 0) {
					rewind = false;
				}
			}
			else {
				counter++;

			}
			std::cout << "counter " << counter << std::endl;
		}

		if (counter > 4){
			rewind = true;
		}

		time += 0.1f;

		processInput(window, size, myKeys);

		// rows and columns to new sizes incase the user presses UP/DOWN keys
		int row = size[0];
		int col = size[1];

		// clear the color buffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// start the shader program
		shaderProgram.Start();
		// bind the vao so opengl knows to use it
		VAO1.Bind();

		// reset x and y to 0
		x = 0.0f;
		y = 0.0f;

		// update the color
		//glUniform1f(cID, sin(c));
		c += 0.001f;

		// set the width and height for the squares
		// in case the sizes are new
		float w = 3/static_cast<float>(col);
		float h = 3/static_cast<float>(row);

		//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		//glUniform1f(mxID, x);
		//glUniform1f(myID, y);
		
		for (GLuint i = 0; i < 6; i++)
		{
			x = i;
			glm::mat4 newmodel = glm::mat4(1.0f);
			glm::vec3 tempCube = cubePos[i] + glm::vec3(cos((c/2 )* 1.4f));
			newmodel = glm::translate(newmodel, tempCube);
			if (i == counter) {
				angle = 0.04f * time;
				newmodel = glm::translate(newmodel, glm::vec3(sin(time*0.1f) + 2.0f, 0.0f, 0.0f));
			}
			if (((i == counter-1) && i >=0) || ((i == counter + 1) && i <= 5)) {
				angle = 0.035f * time;
				newmodel = glm::translate(newmodel, glm::vec3(sin(time * 0.1f) + 1.0f, 0.0f, 0.0f));

			}
			if (((i == counter - 2) && i >= 0) || ((i == counter + 2) && i <= 5)) {
				angle = 0.03f * time;
				newmodel = glm::translate(newmodel, glm::vec3(sin(time * 0.1f), 0.0f, 0.0f));
			}

			if (((i == counter - 3) && i >= 0) || ((i == counter + 3) && i <= 5)) {
				angle = 0.025f * time;
			}

			if (((i == counter - 4) && i >= 0) || ((i == counter + 4) && i <= 5)) {
				angle = 0.02f * time;
			}

			if (((i == counter - 5) && i >= 0) || ((i == counter + 5) && i <= 5)) {
				angle = 0.015f * time;
			}

			newmodel = glm::rotate(newmodel, (float)glfwGetTime() * glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(newmodel));
			glUniform1f(mxID, x);
			glUniform1f(myID, y);
			glUniform1f(cID, cos((c+i)));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			

		}




		//for (int i = 0; i <= row*col ; i++) {

		//	if (i % col == 0 && i != 0) {
		//		x = 0.0f;
		//		y += h;
		//	}
		//	glUniform1f(cID, tan(c+i));
		//	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		//	glUniform1f(mxID, x);
		//	glUniform1f(myID, y);
		//	x += w;			
		//}

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // delete all the object we created
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    shaderProgram.Delete();

    glfwDestroyWindow(window);
    // need to always terminate
    glfwTerminate();
    return 0;
}

void resizeFrameBuffer(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
/// <summary>
/// function for processing user input. 
/// If the user presses the escape key, the program will exit.
/// If the user presses the up key, the number of squares will go down
/// If the user presses the down key, the number of squares will go up
/// </summary>
/// <param name="window">the window object</param>
/// <param name="size">the number of squares</param>
/// <param name="myKeys">the keys</param>
void processInput(GLFWwindow* window, GLuint* size, int* myKeys)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

	// if the up key was pressed, it will set that key value to true
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS ) {
		myKeys[0] = true;
		std::cout << size[0] << "up is being pressed" << std::endl;
	}
	// if the up key was released and it had the value from myKeys as true
	// it will then indicate to increase the number of squares
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE && myKeys[0] == true) {
		myKeys[0] = false;
		if (size[0] <= 20) {
			size[0] += 1;
			size[1] += 1;
		}

	}

	// if the down key was pressed, it will set that key value to true
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		myKeys[1] = true;

	}
	// if the down key was released and it had the value from myKeys as true
	// it will then indicate to decrease the number of squares	
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE && myKeys[1] == true) {
		myKeys[1] = false;
		if (size[0] > 0) {
			size[0] -= 1;
			size[1] -= 1;
		}
	}
		
}
