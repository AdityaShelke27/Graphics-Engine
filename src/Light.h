#include <glad\glad.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "Shader.h"
class Light
{
	public:
		glm::vec3 lightPos;
		void setLightPosition(glm::vec3 pos)
		{
			lightPos = pos;
			modal = glm::mat4(1);
			modal = glm::translate(modal, lightPos);

		}
		Light(glm::vec3 pos, const std::vector<float>& vertices, const int strideSize)
		{
			lightPos = pos;

			glGenVertexArrays(1, &lightVAO);
			glBindVertexArray(lightVAO);
			glGenBuffers(1, &lightVBO);
			glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, strideSize * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			modal = glm::translate(modal, lightPos);
			
			lightModalLoc = glGetUniformLocation(lightShader.ID, "modal");
			lightViewLoc = glGetUniformLocation(lightShader.ID, "view");
			lightProjectionLoc = glGetUniformLocation(lightShader.ID, "projection");
			glBindVertexArray(0);
		}
		void calculate(glm::f32* view, glm::f32* projection)
		{
			lightShader.use();
			glUniformMatrix4fv(lightViewLoc, 1, GL_FALSE, view);
			glUniformMatrix4fv(lightProjectionLoc, 1, GL_FALSE, projection);

			glBindVertexArray(lightVAO);

			glUniformMatrix4fv(lightModalLoc, 1, GL_FALSE, glm::value_ptr(modal));
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
		}
		Shader* getShader()
		{
			return &lightShader;
		}
	private:
		unsigned int lightVAO = 0, lightVBO = 0;
		glm::mat4 modal = glm::mat4(1);
		Shader lightShader = Shader("./shaders/lightShader.vs", "./shaders/lightShader.fs");
		unsigned int lightModalLoc;
		unsigned int lightViewLoc;
		unsigned int lightProjectionLoc;
};