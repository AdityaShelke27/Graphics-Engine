#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

class Camera
{
	public:
		float cameraSpeed = 10.0f;
		float cameraSensitivity = 10.0f;
		float fov = 45;

        void mouseMovement(glm::vec2 delta)
        {
            yaw += delta.x;
            pitch -= delta.y;

            if (pitch > 89.0f)
            {
                pitch = 89.0f;
            }
            else if (pitch < -89.0f)
            {
                pitch = -89.0f;
            }
        }
        void scroll(double yOffset)
        {
            fov -= (float)yOffset;

            if (fov > 45.0f)
            {
                fov = 45.0f;
            }
            else if (fov < 1.0f)
            {
                fov = 1.0f;
            }
        }
        void moveFront()
        {
            cameraPosition += cameraSpeed * deltaTime * cameraFront;
        }
        void moveBack()
        {
            cameraPosition -= cameraSpeed * deltaTime * cameraFront;
        }
        void moveRight()
        {
            cameraPosition += cameraSpeed * deltaTime * glm::cross(cameraFront, cameraUp);
        }
        void moveLeft()
        {
            cameraPosition -= cameraSpeed * deltaTime * glm::cross(cameraFront, cameraUp);
        }
        void calculate()
        {
            direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            direction.y = sin(glm::radians(pitch));
            cameraFront = glm::normalize(direction);

            view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, up);
            projection = glm::perspective(glm::radians(fov), (float)WIDTH / HEIGHT, 0.1f, 100.0f);
        }
        glm::f32* getView()
        {
            return glm::value_ptr(view);
        }
        glm::f32* getProjection()
        {
            return glm::value_ptr(projection);
        }

	private:
		glm::vec3 cameraPosition = glm::vec3(0, 0, 1);
		glm::vec3 cameraFront = glm::vec3(0, 0, -1);
		glm::vec3 cameraUp = glm::vec3(0, 1, 0);
		float yaw = -90, pitch;
		glm::vec3 direction;
        glm::mat4 view;
        glm::mat4 projection;
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
};