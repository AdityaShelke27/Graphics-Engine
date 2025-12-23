#version 330 core

out vec4 fragColor;

in vec2 TexCoords;
in vec3 Normals;
in vec3 fragPos;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float alphaValue;
uniform mat4 view;
uniform mat4 modal;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	float ambientStrength = 0.1f;
	float specularStrength = 0.1f;
	vec3 ambience = lightColor * ambientStrength;
	mat3 viewModal = mat3(view * modal);

	vec3 norm = normalize(Normals);
	vec3 lightDir = viewModal * normalize(lightPos - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = lightColor * diff;

	vec3 viewDir = vec3(0, 0, 1);//normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(viewModal * -lightPos, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 2);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = ambience + diffuse + specular;
	fragColor = vec4(result, 1.0f) * texture(texture1, TexCoords);
}