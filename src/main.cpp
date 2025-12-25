#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include "Shader.h"
#include "Camera.h"
#include "stb_image.h"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "Light.h"

const int WIDTH = 800;
const int HEIGHT = 600;

float deltaTime = 0;
float currentTime = 0;

glm::vec2 mousePos;

Camera cam;

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    cam.setWidthHeight(width, height);
    glViewport(0, 0, width, height);
}
static void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
    cam.scroll(yOffset);
}
void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
    glm::vec2 delta = glm::vec2(xPos - mousePos.x, yPos - mousePos.y);
    cam.mouseMovement(delta);

    mousePos.x = (float)xPos;
    mousePos.y = (float)yPos;
}
static void processInput(GLFWwindow* window, float &alphaValue)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        alphaValue += 0.01f;
        if (alphaValue > 1) alphaValue = 1;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        alphaValue -= 0.01f;
        if (alphaValue < 0) alphaValue = 0;
    }
    if (glfwGetKey(window, GLFW_KEY_W))
    {
        cam.moveForward(deltaTime, 1.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_S))
    {
        cam.moveForward(deltaTime, -1.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_A))
    {
        cam.moveRight(deltaTime, -1.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_D))
    {
        cam.moveRight(deltaTime, 1.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_Q))
    {
        cam.moveUp(deltaTime, -1.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_E))
    {
        cam.moveUp(deltaTime, 1.0f);
    }
}
static void createShaderProgram(unsigned int* shaderProgram, const char** vertexShaderCode, const char** fragmentShaderCode)
{
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, vertexShaderCode, NULL);
    glCompileShader(vertexShader);

    int success;
    char info_Log[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, info_Log);
        std::cout << "Error in Vertex Shader: " << info_Log << std::endl;
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, fragmentShaderCode, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, info_Log);
        std::cout << "Error in Fragment Shader: " << info_Log << std::endl;
    }

    *shaderProgram = glCreateProgram();
    glAttachShader(*shaderProgram, vertexShader);
    glAttachShader(*shaderProgram, fragmentShader);
    glLinkProgram(*shaderProgram);

    glGetProgramiv(*shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(*shaderProgram, 512, NULL, info_Log);
        std::cout << "Error in Shader Program: " << info_Log << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}
static void createTexture(unsigned int &texture, const char* path, int format, int sWRAP = GL_REPEAT, int tWRAP = GL_REPEAT)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sWRAP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tWRAP);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    int height, width, nrChannels;
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load Texture" << std::endl;
    }
    stbi_image_free(data);
}
static void createVAO(unsigned int* VAO)
{
    glGenVertexArrays(1, VAO);
    glBindVertexArray(*VAO);
}
template<size_t N>
static void createBufferData(unsigned int* VBO, float (&vertices)[N])
{
    glGenBuffers(1, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}
static void disableMouse(GLFWwindow* window)
{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

int main()
{
    if (!glfwInit())
    {
        std::cout << "GLFW failed\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Graphics-Engine", NULL, NULL);

    if (!window)
    {
        std::cout << "Window creation failed\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    Shader ourShader("./shaders/basic.vs", "./shaders/basic.fs");
    
    float vertices[] = {
        // Vertices                     Texture Coords          Normals
        -0.5f, -0.5f, -0.5f,            0.0f, 0.0f,         0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,            1.0f, 0.0f,         0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,            1.0f, 1.0f,         0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,            1.0f, 1.0f,         0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,            0.0f, 1.0f,         0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,            0.0f, 0.0f,         0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,            0.0f, 0.0f,         0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,            1.0f, 0.0f,         0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,            1.0f, 1.0f,         0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,            1.0f, 1.0f,         0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,            0.0f, 1.0f,         0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,            0.0f, 0.0f,         0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f,            1.0f, 0.0f,         -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,            1.0f, 1.0f,         -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,            0.0f, 1.0f,         -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,            0.0f, 1.0f,         -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,            0.0f, 0.0f,         -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,            1.0f, 0.0f,         -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,            1.0f, 0.0f,         1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,            1.0f, 1.0f,         1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,            0.0f, 1.0f,         1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,            0.0f, 1.0f,         1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,            0.0f, 0.0f,         1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,            1.0f, 0.0f,         1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,            0.0f, 1.0f,         0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,            1.0f, 1.0f,         0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,            1.0f, 0.0f,         0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,            1.0f, 0.0f,         0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,            0.0f, 0.0f,         0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,            0.0f, 1.0f,         0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,            0.0f, 1.0f,         0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,            1.0f, 1.0f,         0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,            1.0f, 0.0f,         0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,            1.0f, 0.0f,         0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,            0.0f, 0.0f,         0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,            0.0f, 1.0f,         0.0f,  1.0f,  0.0f,
    };
    unsigned int indices[] ={
        1, 0, 2,
        3, 1, 2
    };
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        /*glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)*/
    };

    stbi_set_flip_vertically_on_load(true);
    unsigned int texture, texture2;
    createTexture(texture, "./textures/container.jpg", GL_RGB);
    createTexture(texture2, "./textures/awesomeface.png", GL_RGBA);

    unsigned int VAO, VBO;
    createVAO(&VAO);
    createBufferData(&VBO, vertices);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    Light light(glm::vec3(1.2f, 0, 1), std::vector<float>(std::begin(vertices), std::end(vertices)), 8);

    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);
    ourShader.setVec3("light.ambient", glm::vec3(0.1f));
    ourShader.setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
    ourShader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

    ourShader.setVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
    ourShader.setVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
    ourShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    ourShader.setFloat("material.shininess", 1);

    float alphaValue = 0.2f;

    disableMouse(window);

    unsigned int modalLoc = glGetUniformLocation(ourShader.ID, "modal");
    unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");
    unsigned int projectionLoc = glGetUniformLocation(ourShader.ID, "projection");

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window, alphaValue);

        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float time = (float)glfwGetTime();
        deltaTime = time - currentTime;
        currentTime = time;
        float radius = 4;

        cam.calculate();

        light.setLightPosition(glm::vec3(radius * sin(time), radius * cos(time), radius * sin(time)));

        light.calculate(cam.getView(), cam.getProjection());
        light.setLightColor(glm::vec3(sin(time * 2), sin(time * 0.7f), sin(time * 1.3f)));

        ourShader.use();
        ourShader.setVec3("light.ambient", light.getlightColor() * 0.1f);
        ourShader.setVec3("light.diffuse", light.getlightColor() * 0.5f);

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, cam.getView());
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, cam.getProjection());
        ourShader.setVec3("light.position", light.lightPos);
        ourShader.setVec3("viewPos", *cam.getPosition());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        ourShader.setFloat("alphaValue", alphaValue);
        glBindVertexArray(VAO);

        for (int i = 0; i < 1; i++)
        {
            glm::mat4 modal(1);

            modal = glm::translate(modal, glm::vec3(0)/*cubePositions[i]*/);
            //modal = glm::rotate(modal, (float)glfwGetTime() * glm::radians(45.0f), glm::vec3(0.5f, 1.0f, 0.0f));
            glUniformMatrix4fv(modalLoc, 1, GL_FALSE, glm::value_ptr(modal));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}