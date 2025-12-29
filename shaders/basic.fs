#version 330 core

out vec4 fragColor;

in vec2 TexCoords;
in vec3 Normals;
in vec3 fragPos;

uniform mat4 view;
uniform mat4 modal;

uniform vec3 viewPos;

struct Light
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
struct Material
{
	sampler2D diffuse;
	vec3 specular;
	float shininess;
};
uniform Material material;
uniform Light light;

void main()
{
	vec3 ambience = light.ambient * material.ambient;
	mat3 viewModal = mat3(view * modal);

	vec3 norm = normalize(Normals);
	vec3 lightDir = viewModal * normalize(light.position - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	vec3 viewDir = vec3(0, 0, 1);
	vec3 reflectDir = reflect(viewModal * -light.position, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = material.specular * spec * light.specular;

	vec3 result = ambience + diffuse + specular;
	fragColor = vec4(result, 1.0f) * texture(texture1, TexCoords);
}