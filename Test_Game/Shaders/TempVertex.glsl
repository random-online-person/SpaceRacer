#version 440

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 texcoord;

out vec3 vs_color;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main()
{
	vs_color = texcoord;

	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(pos, 1.0f);
}