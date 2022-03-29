#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float moveX;
uniform float moveY;
uniform float myColor;

void main()
{
	gl_Position = projection * view * model * vec4(aPos.x * moveX, aPos.y + moveY, aPos.z * moveX, 1.0f);

   color = aColor * myColor;
}
