#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#define GL_SILENCE_DEPRECATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#include "ft_shader.hpp"
#include "ft_math.hpp"
#include "ft_loader.hpp"
#include "ft_mesh.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800.0f;
const unsigned int SCR_HEIGHT = 600.0f;

float visibility = 0.0f;

// visual mode for render obj
// --------------------------
enum VISUAL_MODE{
    VM_WIREFRAME,
    VM_POINT,
    VM_FILL
};

int main(int argc, char *argv[])
{
    if (argc != 3 ){
        std::cout << "You need obj or texture file !\n";
        return 0;
    }


    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Scop - 42", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); 
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // hook user input

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char* glsl_version = "#version 130";
    ImGui_ImplOpenGL3_Init(glsl_version);
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


    //  global configuration
    //  --------------------
    glEnable(GL_DEPTH_TEST); 

    // Shader init
    // -----------
    Shader ourShader("shader/scop.vs", "shader/scop.fs");

    // texture
    // -------
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height;
    unsigned char *data = ftloader::BMP(argv[2], width, height);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    delete(data);
    // uncomment this call to draw in wireframe polygons.
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    Mesh object = Mesh(argv[1], texture, ourShader);


    // mesh var
    // --------
    int visual_mode = VM_FILL;
    bool autorotationX = false;
    bool autorotationY = false;
    bool autorotationZ = false;
    float rotationX = 0.0f;
    float rotationY = 0.0f;
    float rotationZ = 0.0f;
    float translateX = 0.0f;
    float translateY = 0.0f;
    float translateZ = 0.0f;
    float scaleX = 1.0f;
    float scaleY = 1.0f;
    float scaleZ = 1.0f;
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        switch(visual_mode){
            case VM_WIREFRAME:
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
                break;
            case VM_POINT:
                glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); 
                break;
            case VM_FILL:
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
                break;
        }
        static float f = 0.0f;
        static int counter = 0;
        const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(SCR_WIDTH, 0));
        ImGui::Begin("Scop42 ToolBox", NULL, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_MenuBar);    
        if (ImGui::BeginMenuBar()){
            if (ImGui::BeginMenu("Rotation"))
            {
                ImGui::Checkbox("Auto X rotation", &autorotationX);      // Edit bools storing our window open/close state
                ImGui::Checkbox("Auto Y rotation", &autorotationY); 
                ImGui::Checkbox("Auto Z rotation", &autorotationZ); 
                if (!autorotationX) ImGui::SliderFloat("X rotation", &rotationX, -180.0f, 180.0f);
                if (!autorotationY) ImGui::SliderFloat("Y rotation", &rotationY, -180.0f, 180.0f);
                if (!autorotationZ) ImGui::SliderFloat("Z rotation", &rotationZ, -180.0f, 180.0f); 
                if (ImGui::Button("Reset")){
                    rotationX = 0.0f;
                    rotationY = 0.0f;
                    rotationZ = 0.0f;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        if (ImGui::BeginMenuBar()){
            if (ImGui::BeginMenu("Translate"))
            {
                ImGui::SliderFloat("X translate", &translateX, -10.0f, 10.0f);
                ImGui::SliderFloat("Y translate", &translateY, -10.0f, 10.0f);
                ImGui::SliderFloat("Z translate", &translateZ, -10.0f, 10.0f);
                if (ImGui::Button("Reset")){
                    translateX = 0.0f;
                    translateY = 0.0f;
                    translateZ = 0.0f;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        if (ImGui::BeginMenuBar()){
            if (ImGui::BeginMenu("Scale"))
            {
                ImGui::SliderFloat("X scale", &scaleX, -5.0f, 5.0f);
                ImGui::SliderFloat("Y scale", &scaleY, -5.0f, 5.0f);
                ImGui::SliderFloat("Z scale", &scaleZ, -5.0f, 5.0f);
                if (ImGui::Button("Reset")){
                    scaleX = 1.0f;
                    scaleY = 1.0f;
                    scaleZ = 1.0f;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        if (ImGui::BeginMenuBar()){
            if (ImGui::BeginMenu("Other"))
            {
                ImGui::RadioButton("Wireframe", &visual_mode, VM_WIREFRAME);
                ImGui::RadioButton("Point", &visual_mode, VM_POINT);
                ImGui::RadioButton("Fill", &visual_mode, VM_FILL);
                if (ImGui::Button("Reset")){
                    visual_mode = VM_FILL;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        // ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        // ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
        // if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            // counter++;
        // ImGui::SameLine();
        // ImGui::Text("counter = %d", counter);
        // ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();

        //  model matrix
        //  ------------
        ftmath::m4x4 model(1.0f);
        model = ftmath::translatem4(model, ftmath::vec3(translateX, translateY, translateZ));
        model = ftmath::scalem4(model, ftmath::vec3(scaleX, scaleY, scaleZ));
        if (autorotationX){
            model = ftmath::rotatexm4(model, (float)glfwGetTime() * 60.0f);
        }else{
            model = ftmath::rotatexm4(model, rotationX);
        }
        if (autorotationY){
            model = ftmath::rotateym4(model, (float)glfwGetTime() * 60.0f);
        }else{
            model = ftmath::rotateym4(model, rotationY);
        }
        if (autorotationZ){
            model = ftmath::rotatezm4(model, (float)glfwGetTime() * 60.0f);
        }else{
            model = ftmath::rotatezm4(model, rotationZ);
        }
        unsigned int modelShader = glGetUniformLocation(ourShader.ID, "model");
        glUniformMatrix4fv(modelShader, 1, GL_FALSE, model.toglsl());

        //  view matrix
        //  -----------
        ftmath::m4x4 view(1.0f);
        view = ftmath::translatem4(view, ftmath::vec3(0.0f, 0.0f, 4.0f));
        unsigned int viewShader = glGetUniformLocation(ourShader.ID, "view");
        glUniformMatrix4fv(viewShader, 1, GL_FALSE, view.toglsl());

        //  projection matrix
        //  -----------------
        // m4x4 persp(float fov, float ratio, float near, float far);
        ftmath::m4x4 projection = ftmath::persp(35.0f, SCR_WIDTH / SCR_HEIGHT, 0.01f, 1000.0f);
        unsigned int projectionShader = glGetUniformLocation(ourShader.ID, "projection");
        glUniformMatrix4fv(projectionShader, 1, GL_FALSE, projection.toglsl());

        object.Draw();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

//  hook user input
//  ---------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        visibility-=0.005f;
        if (visibility >= 2.0f)
            visibility = 2.0f;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        visibility+=0.005f;
        if (visibility <= -1.0f)
            visibility = -1.0f;
}

//  hook when user rezie the window
//  -------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}