#include <iostream>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <Core/Log.h>
#include <Core/shader.h>
#include <Core/camera.h>
#include <Core/mesh.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
static void glfw_error_callback(int error, const char *description);
static void mouse_callback(GLFWwindow *window, double xpos, double ypos);
static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
static void processInput(GLFWwindow *window);

const int width = 1280;
const int height = 720;
int swap_interval;
Eigen::Vector4f clear_color(1.0f, 1.0f, 1.0f, 1.0f);
FpsCamera *camera;
// Camera control
float lastX = width * 0.5f;
float lastY = height * 0.5f;
bool firstMouse = true;
// timing
float deltaT = 0.0f;
float lastFrame = 0.0f;

int main()
{
    Log::Init();
    camera = new FpsCamera(Eigen::Vector3f(0.0f, 0.0f, 3.0f));

    {
        Eigen::Vector3f position(0.0f, 0.0f, 0.0f);
        Eigen::Vector3f scale(1.0f, 1.0f, 1.0f);
        Eigen::Quaternionf rotation;
        Eigen::Matrix4f model = Eigen::Matrix4f::Zero();
        float4x4 model = float4x4::Identity();
        model.block<3, 3>(0, 0) = rotation.toRotationMatrix() * Scaling(scale);
        model.col(3) << position, 1.0f;
    }

    glfwSetErrorCallback(glfw_error_callback);
    glfwInit();
    // since my mac only support OpenGL4.1 at most
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(width, height, "No Application Debug", NULL, NULL);
    if (window == NULL)
    {
        DEBUG_FATAL("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    DEBUG_INFO("GLFWWindows intialized");
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        DEBUG_FATAL("Failed to initialize GLAD");
        return -1;
    }
    DEBUG_INFO("GLAD intialized");

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSwapInterval(swap_interval);

    glEnable(GL_DEPTH_TEST);

    // ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    // ImGui style
    ImGui::StyleColorsDark();
    // glsl version
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410 core");
    // fonts
    io.Fonts->AddFontDefault();

    Shader shader("Assets/shaders/basic.vs", "Assets/shaders/basic.fs");

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,

        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,

        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,

        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f};

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(clear_color(0), clear_color(1), clear_color(2), clear_color(3));
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float currentFrame = (float)glfwGetTime();
        deltaT = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        shader.use();
        shader.SetFloat4x4("projection", camera->perspective((float)width / (float)height));
        shader.SetFloat4x4("view", camera->view());
        shader.SetFloat4x4("model", Eigen::Matrix4f::Identity());
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        ImGui::Begin("Basic");

        ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::SameLine();
        if (ImGui::Button("Vsync"))
        {
            swap_interval = swap_interval == 1 ? 0 : 1;
            DEBUG_INFO("Vsync: {}", swap_interval == 1 ? "True" : "False");
            glfwSwapInterval(swap_interval);
        }
        ImGui::ColorEdit3("clear color", clear_color.data()); // Edit 3 floats representing a color

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

static void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

static void glfw_error_callback(int error, const char *description)
{
    DEBUG_ERROR("Glfw Error {}: {}", error, description);
}

static void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->ProcessKeyboard(FORWARD, deltaT);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->ProcessKeyboard(BACKWARD, deltaT);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->ProcessKeyboard(RIGHT, deltaT);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->ProcessKeyboard(LEFT, deltaT);
}

static void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = static_cast<float>(xpos);
        lastY = static_cast<float>(ypos);
        firstMouse = false;
    }

    float xoffset = static_cast<float>(xpos) - lastX;
    float yoffset = lastY - static_cast<float>(ypos); // reversed since y-coordinates go from bottom to top

    lastX = static_cast<float>(xpos);
    lastY = static_cast<float>(ypos);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        camera->ProcessMouseInput(xoffset, yoffset);
    }
}

static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
}