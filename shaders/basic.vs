#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec2 aNormals;

out vec2 TexCoords;
out vec2 Normals;

uniform mat4 modal;
uniform mat4 view;
uniform mat4 projection;

void main()
{
   gl_Position = projection * view * modal * vec4(aPos, 1.0f);
   TexCoords = aTexCoords;
   Normals = aNormals;
}