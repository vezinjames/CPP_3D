// C++_3D.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "glm/gtc/matrix_transform.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <fstream>
#include <sstream>
#include <vector>

#include "stb_image.h"

constexpr unsigned int Width_screen = 800;
constexpr unsigned int Height_screen = 600;

const float CAMERA_FOV = 100.0f;

glm::vec3 worldFront = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 worldRight = glm::vec3(1.0f, 0.0f, 0.0f);

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -3.0f);
glm::vec3 cameraFront = worldFront;
glm::vec3 cameraUp = worldUp;
glm::vec3 cameraRight = worldRight;

unsigned int loadTexture(const char *path);


void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

std::vector<glm::vec3> _vertices;
std::vector<unsigned int> vertexIndices;

std::vector<glm::vec3> teapotVertices;
std::vector<glm::vec3> teapotNormals;

std::vector<glm::vec3> teapotAll;

void loadOBJ(const char* path)
{
    std::ifstream obj;

    obj.open(path);
    for(std::string line; std::getline(obj, line);)
    {
        //std::cout << line << '\n';
        std::stringstream stream(line);
        std::string element;
        stream >> element;
        //std::cout << element << '\n';

        if(element == "v")
        {
            glm::vec3 vertex;
            stream >> element;
            vertex.x = std::stof(element);
            stream >> element;
            vertex.y = std::stof(element);
            stream >> element;
            vertex.z = std::stof(element);
            _vertices.push_back(vertex);
        }
        else if(element == "f")
        {
            stream >> element;
            vertexIndices.push_back(std::stoi(element) - 1);
            teapotVertices.push_back(_vertices[std::stoi(element) - 1]);
            stream >> element;
            vertexIndices.push_back(std::stoi(element) - 1);
            teapotVertices.push_back(_vertices[std::stoi(element) - 1]);
            stream >> element;
            vertexIndices.push_back(std::stoi(element) - 1);
            teapotVertices.push_back(_vertices[std::stoi(element) - 1]);
        }
    }
    obj.close();
}

void GenerateNormal()
{
    for(int i = 0; i < teapotVertices.size(); i += 3)
    {
        glm::vec3 p1 = teapotVertices[i];
        glm::vec3 p2 = teapotVertices[i+1];
        glm::vec3 p3 = teapotVertices[i+2];

        glm::vec3 u = p2 - p1;
        glm::vec3 v = p3 - p1;

        glm::vec3 normal = glm::cross(u,v);

        teapotNormals.push_back(normal);
        teapotNormals.push_back(normal);
        teapotNormals.push_back(normal);
    }
}

void GenerateAll()
{
    for(int i = 0; i < teapotVertices.size(); i++)
    {
        teapotAll.push_back(teapotVertices[i]);
        teapotAll.push_back(teapotNormals[i]);
    }  
}


int main(int argc, char* argv[])
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    loadOBJ("teapot.obj");
    GenerateNormal();
    GenerateAll();

    GLFWwindow* window = glfwCreateWindow(Width_screen, Height_screen, "CoursOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << '\n';
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << '\n';
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader shader("vertex.shader", "fragment.shader");

    Shader shaderLight("vertexLight.shader", "fragmentLight.shader");
    glm::vec3 lightPosition(2, 3, -2);

    // Point of triangle
    float vertices[] = {
        //Front face
        0.5f, -0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 1.0f,
        // Left Face
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        // Top Face
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        // Bottom Face
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        // Right Face
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        // Back Face
        -0.5, 0.5f, -0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f,
        
    };

    // Create new variable with uid in buffer
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // Bind vertexBuffe(s), and then configure vertex attributes
    glBindVertexArray(VAO);

    // specify variable type
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, teapotAll.size()*sizeof(glm::vec3), teapotAll.data(), GL_STATIC_DRAW); // GL_DYNAMIC_DRAW, GL_STREAM_DRAW

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,  sizeof(glm::vec3) * 2, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,  sizeof(glm::vec3) * 2, (void*)sizeof(glm::vec3));
    glEnableVertexAttribArray(2);

    /*glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);*/

    /*unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexIndices.size() * sizeof(unsigned int), vertexIndices.data(), GL_STATIC_DRAW);*/
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    unsigned int VBOLight, VAOLight;
    glGenVertexArrays(1, &VAOLight);
    glGenBuffers(1, &VBOLight);

    // Bind vertex buffer(s), and then configure vertex attributes
    glBindVertexArray(VAOLight);

    // specify variable type
    glBindBuffer(GL_ARRAY_BUFFER, VBOLight);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // GL_DYNAMIC_DRAW, GL_STREAM_DRAW

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // WRAPPING
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // FILTERING
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load texture
    int width, height, nbChannels;
    unsigned char* data = stbi_load("wall.jpg", &width, &height, &nbChannels, 0);

    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load ressources !" << '\n';
    }
    stbi_image_free(data);

    shader.Use();
    shader.SetInt("texture", 0);

    unsigned int diffuseMap = loadTexture("white_square.png");
    
    shaderLight.Use();
    
    
    float lastTime = (float)glfwGetTime();
    
    while (!glfwWindowShouldClose(window))
    {
        // Window
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        }
        // Camera
        
        float cameraSpeed = 1;
        float currentTime = (float)glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        float cameraSpeedByDeltaTime = cameraSpeed * deltaTime;
        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            cameraPos += cameraFront * cameraSpeedByDeltaTime;
        }
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            cameraPos -= cameraFront * cameraSpeedByDeltaTime;
        }
        
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeedByDeltaTime;
        }
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeedByDeltaTime;
        }

        if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            cameraPos += cameraUp * cameraSpeedByDeltaTime;
        }
        if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        {
            cameraPos -= cameraUp * cameraSpeedByDeltaTime;
        }

        // Render
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();
        
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), (float)Width_screen / (float)Height_screen, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        shader.SetMatrix4("projection", projection);
        shader.SetMatrix4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        float angle = 30.0f;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

        shader.SetMatrix4("model", model);
        
        
        shader.SetVector3("lightColor", 1.0f, 1.0f, 1.0f);
        shader.SetVector3("objectColor", 1.0f, 1.0f, 1.0f);
        shader.SetFloat("ambiantStrength", 0.5f);
        shader.SetVector3("lightPos", lightPosition);
        shader.SetVector3("viewPos", cameraPos);
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glDrawArrays(GL_TRIANGLES, 0, teapotAll.size()/2);
        //glDrawElements(GL_TRIANGLES, vertexIndices.size(), GL_UNSIGNED_INT, NULL);
        glBindVertexArray(0);

        shaderLight.Use();
        shaderLight.SetMatrix4("projection", projection);
        shaderLight.SetMatrix4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPosition);
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
        shaderLight.SetMatrix4("model", model);
        shaderLight.SetInt("material.diffuse", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);

        glBindVertexArray(VAOLight);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shader.ID);

    glDeleteVertexArrays(1, &VAOLight);
    glDeleteBuffers(1, &VBOLight);
    glDeleteProgram(shaderLight.ID);


    glfwTerminate();
    return 0;
}

// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Astuces pour bien démarrer  : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.
