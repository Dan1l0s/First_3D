#include <iostream>
#include "../Wrappings/Shader/shader.h"
#include "../Wrappings/ShaderProgram/shader_program.h"
#include "../Wrappings/Buffer/buffer.h"
#include "../Wrappings/Texture/texture.h"
#include "../Wrappings/Camera/camera.h"
#include "../Wrappings/InputManager/input_manager.h"
#include "../Wrappings/Config/config.h"
#include <windows.h>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace fs = std::filesystem;

float WINDOW_WIDTH = 1920.0;
float WINDOW_HEIGHT = 1080.0;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void init_window();
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void GetScreenSize();

InputManager manager;
Texture texture1;
Texture texture2;
Buffer cube;
Buffer light_cube;
Camera camera;
GLFWwindow *window;
ShaderProgram program;
ShaderProgram light_program;

float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
float lastX = WINDOW_WIDTH / 2, lastY = WINDOW_HEIGHT / 2;
bool first_mouse = true;

std::vector<float> vertices = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f //
};

std::vector<unsigned int> indices = {
    0, 1, 2, // first triangle
    0, 2, 3  // second triangle
};

glm::vec3 light_color(0.9f, 0.9f, 0.9f);
// glm::vec3 object_color(1.0f, 1.0f, 1.2f);
glm::vec3 object_color(0.6f, 0.2f, 0.6f);
glm::vec3 light_pos(2.0f, 1.5f, 1.5f);

int main()
{

    init_window();
    cube.Init(vertices, indices);
    light_cube.Init(vertices, indices);

    light_program.InitProgram();
    light_program.AddShader(WORKING_DIR / "src" / "Shaders" / "light_fragment_shader.fs", GL_FRAGMENT_SHADER);
    light_program.AddShader(WORKING_DIR / "src" / "Shaders" / "vertex_shader.vs", GL_VERTEX_SHADER);

    program.InitProgram();
    program.AddShader(WORKING_DIR / "src" / "Shaders" / "vertex_shader.vs", GL_VERTEX_SHADER);
    program.AddShader(WORKING_DIR / "src" / "Shaders" / "fragment_shader.fs", GL_FRAGMENT_SHADER);

    texture1.Init(WORKING_DIR / "res" / "container2_diffuse.png");
    texture2.Init(WORKING_DIR / "res" / "container2_specular.png");

    texture1.BindToUnit(1);
    texture2.BindToUnit(2);

    program.LinkProgram();
    light_program.LinkProgram();

    glEnable(GL_DEPTH_TEST);

    int width, height;
    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    //  render loop
    while (!glfwWindowShouldClose(window))
    {
        // clear screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        processInput(window);
        glfwGetWindowSize(window, &width, &height);
        camera.setAspect(double(width) / double(height));

        glm::mat4 model_light = glm::mat4(1.0f);
        model_light = glm::translate(model_light, light_pos);
        model_light = glm::scale(model_light, glm::vec3(0.2f));

        light_program.Use();

        light_program.SetVec3f("lightColor", light_color);
        light_program.SetMat4fv("model", model_light);
        light_program.SetMat4fv("projection", camera.getProjectionMatrix());
        light_program.SetMat4fv("view", camera.getViewMatrix());
        light_cube.Use();
        glDrawArrays(GL_TRIANGLES, 0, 36);

        program.Use();
        glm::mat4 model = glm::mat4(1.0f);
        // model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

        // light_pos = glm::vec3(sin((float)glfwGetTime()), 0.0f, cos((float)glfwGetTime())) * 2.0f;

        program.SetInt("material.specular", 2);
        program.SetInt("material.diffuse", 1);

        program.SetFloat("material.shininess", 64.0f);

        // light_color = glm::vec3((sin((float)glfwGetTime()) + 1) / 2, (cos((float)glfwGetTime()) + 1) / 2, 0.5f);

        glm::vec3 ambient = light_color * 0.3f;
        glm::vec3 diffuse = light_color * 0.5f;
        glm::vec3 specular = light_color * 0.7f;

        program.SetVec3f("spotLight.position", camera.getPosition());
        program.SetVec3f("spotLight.direction", camera.getDirection());
        program.SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        program.SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
        program.SetVec3f("spotLight.clq", glm::vec3(1.0f, 0.09f, 0.032f));
        program.SetVec3f("spotLight.ambient", ambient);
        program.SetVec3f("spotLight.diffuse", diffuse);
        program.SetVec3f("spotLight.specular", specular);

        ambient = light_color * 0.3f;
        diffuse = light_color * 0.8f;
        specular = light_color * 0.4f;

        program.SetVec3f("pointLight.clq", glm::vec3(1.0f, 0.09f, 0.032f));
        program.SetVec3f("pointLight.position", light_pos);
        program.SetVec3f("pointLight.ambient", ambient);
        program.SetVec3f("pointLight.diffuse", diffuse);
        program.SetVec3f("pointLight.specular", specular);

        program.SetVec3f("viewPos", camera.getPosition());
        program.SetMat4fv("model", model);
        program.SetMat4fv("projection", camera.getProjectionMatrix());
        program.SetMat4fv("view", camera.getViewMatrix());

        cube.Use();
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // draw only lines (borders)
        // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // draw and fill

        // swap buffers, check if there are any actions
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// custom event on resize
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// user input check
void processInput(GLFWwindow *window)
{
    const float sensitivity = 0.12f;
    static bool cursormode, flashlight_mode, point_mode;
    manager.ReadFrame();
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    float speed = 5 * deltaTime;

    if (manager.KeyPressed(GLFW_KEY_TAB))
    {
        if (cursormode)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            cursormode = false;
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            cursormode = true;
            // glfwSetCursorPos(window, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
        }
    }
    if (cursormode)
        return;
    camera.RotateY(manager.CursorXOffset() * sensitivity);
    camera.RotateX(manager.CursorYOffset() * sensitivity / 16 * 9);
    if (manager.KeyDown(GLFW_KEY_UP))
    {
        light_pos += glm::vec3(0.0f, 1.0f, 0.0f) * speed;
    }
    if (manager.KeyDown(GLFW_KEY_DOWN))
    {
        light_pos += glm::vec3(0.0f, -1.0f, 0.0f) * speed;
    }
    if (manager.KeyDown(GLFW_KEY_LEFT))
    {
        light_pos += glm::vec3(-1.0f, 0.0f, 0.0f) * speed;
    }
    if (manager.KeyDown(GLFW_KEY_RIGHT))
    {
        light_pos += glm::vec3(1.0f, 0.0f, 0.0f) * speed;
    }
    if (manager.KeyDown(GLFW_KEY_LEFT_CONTROL))
    {
        light_pos += glm::vec3(0.0f, 0.0f, 1.0f) * speed;
    }
    if (manager.KeyDown(GLFW_KEY_LEFT_SHIFT))
    {
        light_pos += glm::vec3(0.0f, 0.0f, -1.0f) * speed;
    }
    if (manager.KeyPressed(GLFW_KEY_F))
    {
        if (flashlight_mode)
        {
            flashlight_mode = false;
            program.SetInt("flash_mode", 0);
        }
        else
        {
            flashlight_mode = true;
            program.SetInt("flash_mode", 1);
        }
    }
    if (manager.KeyPressed(GLFW_KEY_L))
    {
        if (point_mode)
        {
            point_mode = false;
            program.SetInt("point_mode", 0);
        }
        else
        {
            point_mode = true;
            program.SetInt("point_mode", 1);
        }
    }
    if (manager.KeyDown(GLFW_KEY_W))
    {
        camera.MoveInLocal(glm::vec3(0.0f, 0.0f, 1.0f) * speed);
    }
    if (manager.KeyDown(GLFW_KEY_S))
    {
        camera.MoveInLocal(glm::vec3(0.0f, 0.0f, -1.0f) * speed);
    }
    if (manager.KeyDown(GLFW_KEY_D))
    {
        camera.MoveInLocal(glm::vec3(1.0f, 0.0f, 0.0f) * speed);
    }
    if (manager.KeyDown(GLFW_KEY_A))
    {
        camera.MoveInLocal(glm::vec3(-1.0f, 0.0f, 0.0f) * speed);
    }
    if (manager.KeyDown(GLFW_KEY_C))
    {
        camera.MoveInWorld(glm::vec3(0.0f, -1.0f, 0.0f) * speed);
    }
    if (manager.KeyDown(GLFW_KEY_SPACE))
    {
        camera.MoveInWorld(glm::vec3(0.0f, 1.0f, 0.0f) * speed);
    }
    if (manager.KeyPressed(GLFW_KEY_ESCAPE))
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void init_window()
{ // opengl version choice
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GetScreenSize();
    // create window (empty)
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
    glfwSetWindowPos(window, 0, 25);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // GLAD initialization
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }

    // resize draw area
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    // custom event on resize
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    manager.SetWindow(window);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.setFov(camera.getFov() - yoffset);
}

void GetScreenSize()
{
    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    WINDOW_WIDTH = mode->width;
    WINDOW_HEIGHT = mode->height;
}