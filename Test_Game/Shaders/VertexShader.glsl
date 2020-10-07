#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 objectpos;
out vec3 Normal;
out vec2 TexCoord;

void main()
{
	objectpos = vec3(model * vec4(aPos, 1.0));
	Normal = mat3(transpose(inverse(model))) * aNormal;

	TexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);

	gl_Position = projection * view * vec4(objectpos, 1.0f);
}