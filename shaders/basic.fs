#version 330 core

out vec4 fragColor;

in vec2 TexCoords;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float alphaValue;

uniform vec3 lightColor;

void main()
{
	float ambientStrength = 0.1f;
	vec3 ambience = lightColor * ambientStrength;
	fragColor = texture(texture1, TexCoords) * vec4(ambience, 1.0f);//mix(texture(texture1, TexCoords), texture(texture2, TexCoords), alphaValue);
}