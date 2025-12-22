#version 330 core

out vec4 fragColor;

in vec2 TexCoords;
in vec3 Normals;
in vec3 fragPos;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float alphaValue;

uniform vec3 lightColor;
uniform vec3 lightPos;

void main()
{
	float ambientStrength = 0.1f;
	vec3 ambience = lightColor * ambientStrength;
	vec3 norm = normalize(Normals);
	vec3 lightDir = normalize(-fragPos + lightPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = lightColor * diff;
	vec3 result = ambience + diffuse;
	fragColor = vec4(result, 1.0f) * texture(texture1, TexCoords);//mix(texture(texture1, TexCoords), texture(texture2, TexCoords), alphaValue);
}