#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const int WIDTH = 800;
const int HEIGHT = 600;

const char* vertexShaderCode = "#version 330 core\n"
                                "layout (location = 0) in vec3 aPos;\n"
                                "void main() \n"
                                "{\n"
                                "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
                                "}\0";

const char* fragmentShaderCode = "#version 330 core\n"
                                "out vec4 fragColor;\n"
                                "void main() \n"
                                "{\n"
                                "fragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                "}\0";

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

static void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}
static void createShaderProgram(unsigned int* shaderProgram)
{
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
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
    glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
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

    glGetProgramiv(*shaderProgram, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(*shaderProgram, 512, NULL, info_Log);
        std::cout << "Error in Shader Program: " << info_Log << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}
static void createVAO(unsigned int* VAO)
{
    glGenVertexArrays(1, VAO);
    glBindVertexArray(*VAO);
}
static void createBufferData()
{
    float vertices[] =
    {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
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

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    unsigned int shaderProgram;
    createShaderProgram(&shaderProgram);
    unsigned int VAO;
    createVAO(&VAO);
    createBufferData();

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        processInput(window);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}