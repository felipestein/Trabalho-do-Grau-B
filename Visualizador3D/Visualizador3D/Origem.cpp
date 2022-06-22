#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "model.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 2.5f, 10.0f)); 
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float translatex = 0.0f, translatey = 0.0f, translatez = 0.0f;
float scalex = 1.0f, scaley = 1.0f, scalez = 1.0f;
float rotatex = 1.0f, rotatey = 1.0f, rotatez = 1.0f;
bool x = false, y = false, z = false;
int p, o;

int main()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Visualizador 3D", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // ativar cursor GLFW_CURSOR_NORMAL

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(false);

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    // build e compilar shaders
    Shader ourShader("shaders/hello.vs", "shaders/hello.fs");

    // ativando o shader
    ourShader.use();

    // carregar o modelo
    Model ourModel("3D_Models/Pokemon/untitled.obj"); 
    ourModel.setPosition(glm::vec3(-2.0f, 0.0f, 0.0f));

    Model ourModel2("3D_Models/Pokemon/Pikachu.obj"); 
    ourModel2.setPosition(glm::vec3(2.0f, 0.0f, 0.0f));

    Model ourModel3("3D_Models/Pokemon/casa.obj"); 
    ourModel3.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

    Model ourModel4("3D_Models/Pokemon/ground.obj"); 
    ourModel4.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

    Model ourModel5("3D_Models/Pokemon/poco.obj");
    ourModel5.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

    // leitura dos modelos // vector
    vector < Model > models;
    models.push_back(ourModel);
    models.push_back(ourModel2);
    models.push_back(ourModel3);
    models.push_back(ourModel4);
    models.push_back(ourModel5);

    // ativar wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        //tempo e angulo
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        float angle = (GLfloat)glfwGetTime();

        // input
        processInput(window);

        // render
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /// iluminação /// luz pontual
        // câmera
        ourShader.setVec3("light.position", camera.Position);
        ourShader.setVec3("light.direction", camera.Front);
        ourShader.setFloat("light.cutOff", glm::cos(glm::radians(15.0f)));
        ourShader.setFloat("light.outerCutOff", glm::cos(glm::radians(20.0f)));
        ourShader.setVec3("viewPos", camera.Position);

        // atenuação
        ourShader.setFloat("light.quadratic", 0.032f);
        ourShader.setFloat("material.shininess", 50.0f);

        ourShader.setVec3("light.ambient", 1.5f, 1.5f, 1.5f);
        ourShader.setVec3("light.diffuse", 0.8f, 0.8f, 0.8f);
        ourShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        ourShader.setFloat("light.constant", 1.0f);
        ourShader.setFloat("light.linear", 0.09f);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view); 

        // translate
        models[0].setPosition(glm::vec3(-2.0f + translatex, 0.0f + translatey, 0.0f + translatez));
        models[1].setPosition(glm::vec3(2.0f + translatex, 0.0f + translatey, 0.0f + translatez));
        models[2].setPosition(glm::vec3(0.0f + translatex, 0.0f + translatey, 0.0f + translatez));
        models[3].setPosition(glm::vec3(0.0f + translatex, 0.0f + translatey, 0.0f + translatez));
        models[4].setPosition(glm::vec3(0.0f + translatex, 0.0f + translatey, 0.0f + translatez));

        //if (x)
        //{
        //    //model = glm::rotate(model, angle, glm::vec3(rotatex, 0.0f, 0.0f));
        //    ourModel.setRotation(glm::vec3(rotatex, 0.0f, 0.0f));
        //}
        //else if (y)
        //{
        //    //model = glm::rotate(model, angle, glm::vec3(0.0f, rotatey, 0.0f));
        //    ourModel.setRotation(glm::vec3(0.0f, rotatey, 0.0f));
        //}
        //else if (z)
        //{
        //   // model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, rotatez));
        //    ourModel.setRotation(glm::vec3(0.0f, 0.0f, rotatez));
        //}

        // scale
        models[0].setScale(glm::vec3(scalex, scaley, scalez));
        models[1].setScale(glm::vec3(scalex, scaley, scalez));
        models[2].setScale(glm::vec3(scalex, scaley, scalez));
        models[3].setScale(glm::vec3(scalex, scaley, scalez));
        models[4].setScale(glm::vec3(scalex, scaley, scalez));

        //rotation
        if (x)
        {
            models[0].setRotation(glm::vec3(rotatex, 0.0f, 0.0f));
            models[1].setRotation(glm::vec3(rotatex, 0.0f, 0.0f));
            models[2].setRotation(glm::vec3(rotatex, 0.0f, 0.0f));
            models[3].setRotation(glm::vec3(rotatex, 0.0f, 0.0f));
            models[4].setRotation(glm::vec3(rotatex, 0.0f, 0.0f));
        }
        else if (y)
        {
            models[0].setRotation(glm::vec3(0.0f, rotatey, 0.0f));
            models[1].setRotation(glm::vec3(0.0f, rotatey, 0.0f));
            models[2].setRotation(glm::vec3(0.0f, rotatey, 0.0f));
            models[3].setRotation(glm::vec3(0.0f, rotatey, 0.0f));
            models[4].setRotation(glm::vec3(0.0f, rotatey, 0.0f));
        }
        else if (z)
        {
            models[0].setRotation(glm::vec3(0.0f, 0.0f, rotatez));
            models[1].setRotation(glm::vec3(0.0f, 0.0f, rotatez));
            models[2].setRotation(glm::vec3(0.0f, 0.0f, rotatez));
            models[3].setRotation(glm::vec3(0.0f, 0.0f, rotatez));
            models[4].setRotation(glm::vec3(0.0f, 0.0f, rotatez));
        }

        // 
        for (int i = 0; i < models.size(); i++)
        {
            models[i].update(ourShader);
            models[i].Draw(ourShader);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    /// rotatação
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        x = true;
        y = false;
        z = false;
    }   
    //else
    //{
    //    x = false;
    //    y = false;
    //    z = false;
    //}

    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
    {
        y = true;
        x = false;
        z = false;
    }

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        z = true;
        x = false;
        y = false;
    }

    /// translação ///
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        translatey += 0.01f;
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        translatey -= 0.01f;
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        translatex += 0.01f;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        translatex -= 0.01f;
    }

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        translatez += 0.01f;
    }

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        translatez -= 0.01f;
    }

    /// escala ///
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    {
        scalex += 0.01f;
        scaley += 0.01f;
        scalez += 0.01f;
    }
 
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        if (scalex <= 0.0f || scaley <= 0.0f || scalez <= 0.0f)
        {
            scalex += 0.01f;
            scaley += 0.01f;
            scalez += 0.01f;
        }
        else
        {
            scalex -= 0.01f;
            scaley -= 0.01f;
            scalez -= 0.01f;
        }

        scalex = glm::clamp(scalex, 0.0f, 20.0f);
        scaley = glm::clamp(scaley, 0.0f, 20.0f);
        scalez = glm::clamp(scalez, 0.0f, 20.0f);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}