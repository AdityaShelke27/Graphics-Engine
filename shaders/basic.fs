#version 330 core

out vec4 fragColor;

in vec3 ourColor;
in vec2 TexCoords;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float alphaValue;

void main()
{
	fragColor = mix(texture(texture1, TexCoords), texture(texture2, TexCoords), alphaValue);
}