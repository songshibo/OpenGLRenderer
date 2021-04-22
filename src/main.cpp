#include <iostream>
#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Eigen/Core>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <Core/Log.h>
#include <Core/shader.h>

using namespace std;

static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
static void glfw_error_callback(int error, const char *description);
static void mouse_callback(GLFWwindow *window, double xpos, double ypos);
static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
static void processInput(GLFWwindow *window);

const int width = 1280;
const int height = 720;
int swap_interval;
Eigen::Vector3f clear_color(1.0f, 0.0f, 0.0f);

int main()
{
    Log::Init();

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

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

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
}

static void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
}

static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
}