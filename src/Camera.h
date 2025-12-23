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
        void moveForward(float deltaTime, float direction)
        {
            cameraPosition += cameraSpeed * deltaTime * direction * cameraFront;
        }
        void moveRight(float deltaTime, float direction)
        {
            cameraPosition += cameraSpeed * deltaTime * direction * glm::cross(cameraFront, cameraUp);
        }
        void moveUp(float deltaTime, float direction)
        {
            cameraPosition += cameraSpeed * deltaTime * direction * cameraUp;
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
        void setWidthHeight(int width, int height)
        {
            WIDTH = width;
            HEIGHT = height;
        }
        glm::vec3* getPosition()
        {
            return &cameraPosition;
        }

	private:
		glm::vec3 cameraPosition = glm::vec3(0, 0, 1);
		glm::vec3 cameraFront = glm::vec3(0, 0, -1);
		glm::vec3 cameraUp = glm::vec3(0, 1, 0);
		float yaw = -90, pitch = 0;
		glm::vec3 direction = glm::vec3(0);
        glm::mat4 view = glm::mat4(1);
        glm::mat4 projection = glm::mat4(1);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        int WIDTH = 800, HEIGHT = 600;
};