#include <glad/glad.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "glad.c"
#include <fstream>
#include <string>
#include <sstream>

struct ShaderProgramSource
{
    std::string vertexSource;
    std::string fragmentSource;
};

static ShaderProgramSource pareseShader(const std::string& filepath)
{
    std::fstream stream(filepath.c_str());
    //remove(filename.c_str());
    enum class shaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
    std::string line;
    std::stringstream ss[2];
    shaderType type = shaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = shaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = shaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }
    return { ss[0].str(), ss[1].str() };
}


int main(void)
{
   


    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Love", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initalize GLAD" << std::endl;
        return -1;
    }

    glad_glViewport(0, 0, 800, 600);

    ShaderProgramSource source = pareseShader("shader/basic.shader");
    /*std::cout << "FRAGMENT" << std::endl;
    
    std::cout << source.fragmentSource << std::endl;
    std::cout << "VERTEX" << std::endl;
    std::cout << source.vertexSource << std::endl;
    */

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* src = source.vertexSource.c_str();
    glShaderSource(vertexShader, 1,&src, NULL);
    glCompileShader(vertexShader);

     int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        std::cout << "Error in Vertex Shader: " << infoLog << std::endl;
    };

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* srcfrag = source.fragmentSource.c_str();
    glShaderSource(fragmentShader, 1,&srcfrag, NULL);
    glCompileShader(fragmentShader);
   
    int frgSuccess;
    char frgLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &frgSuccess);

    if (!frgSuccess)
    {
        std::cout << "Error in fragment: " << frgLog << std::endl;
    };
     

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    

    float vertices[] = 
    {
       -0.5f, 0.5f, 0.0f, // top right
       -0.5f,-0.5f, 0.0f, // bottom right
        0.0f, 0.0f, 0.0f, // bottom left
        0.5f,-0.5f, 0.0f, // top left
        0.5f, 0.5f, 0.0f
    };
    unsigned int indices[] = 
    { // note that we start from 0!
    0, 1, 2, // first triangle
    2, 3, 4
    };
    unsigned int VAO,VBO,EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),indices,GL_STATIC_DRAW);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glEnableVertexAttribArray(0);


    //std::cout << glGetString(GL_VERSION) << std::endl;
     //int max;
     //glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max);
     //std::cout << max;

    int a = 5;
    int& b = a;
    a++;
    std::cout << b++;

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
       
       // glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
       // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        glfwSwapBuffers(window);

        glfwPollEvents();
    };

   
    /*glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(shaderProgram);
    */
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);

    glfwTerminate();
    return 0;
}