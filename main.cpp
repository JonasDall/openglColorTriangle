#include <iostream>
#include <fstream>
#include <string>
#include "glad.h"
#include "glfw3.h"

float triangle1[] = {
    -0.5, -0.5, 0.0, 1.0, 0.0, 0.0,
    0.0, 0.5, 0.0, 0.0, 1.0, 0.0,
    0.5, -0.5, 0.0, 0.0, 0.0, 1.0
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

const char* loadShader(std::string path)
{
    std::string returnValue;
    std::ifstream file;
    file.open(path, std::ios::in);

    if (!file.is_open())
    {
        std::cout << "Could not open\n";
        return "";
    }

    std::string line;
    while (!file.eof())
    {
        std::getline(file, line);
        returnValue.append(line + "\n");
    }

    file.close();
    return returnValue.c_str();
}

bool getShaderState(unsigned int shader, GLenum type)
{
    bool returnState{true};
    char log[512];
    int success;

    glGetShaderiv(shader, type, &success);

    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, log);

        std::cout << "SHADER COMPILATION ERROR\n" << log << '\n';

        returnState = false;
    }

    return returnState;
}

int main()
{
    if (!glfwInit())
    {
        std::cout << "Not working!\n";
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(500, 500, "Test", NULL, NULL);
    if (!window)
    {
        std::cout << "No window\n";
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
{
    std::cout << "Failed to load GLAD\n";
    glfwTerminate();
    return -1;
}

//Load shaders
const char *vertexShaderSource = loadShader("shaders/vertex.glsl");
const char *fragmentShaderSource = loadShader("shaders/fragment1.glsl");

//Compile shaders
unsigned int vertexShader1 = glCreateShader(GL_VERTEX_SHADER);
glShaderSource(vertexShader1, 1, &vertexShaderSource, NULL);
glCompileShader(vertexShader1);

getShaderState(vertexShader1, GL_COMPILE_STATUS);

unsigned int fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
glShaderSource(fragmentShader1, 1, &fragmentShaderSource, NULL);
glCompileShader(fragmentShader1);

getShaderState(fragmentShader1, GL_COMPILE_STATUS);

unsigned int shaderProgram1 = glCreateProgram();
glAttachShader(shaderProgram1, vertexShader1);
glAttachShader(shaderProgram1, fragmentShader1);
glLinkProgram(shaderProgram1);

getShaderState(shaderProgram1, GL_LINK_STATUS);

//Define objects
unsigned int VAO1;
glGenVertexArrays(1, &VAO1);
glBindVertexArray(VAO1);

unsigned int VBO1;
glGenBuffers(1, &VBO1);
glBindBuffer(GL_ARRAY_BUFFER, VBO1);
glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1), triangle1, GL_DYNAMIC_DRAW);

glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
glEnableVertexAttribArray(0);

glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
glEnableVertexAttribArray(1);

unsigned int vertexOffsetUniform = glGetUniformLocation(shaderProgram1, "offset");
glUseProgram(shaderProgram1);

glClearColor(0.7, 0.7, 1.0, 1.0);

    while (!glfwWindowShouldClose(window))
    {
        //Input
        processInput(window);

        //Logic
        float time = glfwGetTime();
        glUniform1f(vertexOffsetUniform, time);

        //Graphics
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        //Buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}