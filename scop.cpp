#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#define GL_SILENCE_DEPRECATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#include <exception>
#include "ft_shader.hpp"
#include "ft_math.hpp"
#include "ft_loader.hpp"
#include "ft_mesh.hpp"
#include "ft_light.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


// settings
unsigned int SCR_WIDTH = 800.0f;
unsigned int SCR_HEIGHT = 600.0f;

// visual mode for render obj
// --------------------------
enum VISUAL_MODE{
    VM_WIREFRAME,
    VM_POINT,
    VM_FILL
};

typedef struct DeltaTime{
    float currentFrame;
    float deltaTime;
    float lastFrame;
} DeltaTime;
DeltaTime dt;

//  command struct
//  --------------
typedef struct Command{
    float color_mix;
    int visual_mode;
    bool autorotationX;
    bool autorotationY;
    bool autorotationZ;
    float rotationSpeed;
    float translateX;
    float translateY;
    float translateZ;
    float rotationX;
    float rotationY;
    float rotationZ;
    float scaleX;
    float scaleY;
    float scaleZ;
    bool color_mix_transition;
    float visibility;
    float pointAndLineSize;
    float background_color[3];
} Command;
Command cmd;

int main(int argc, char *argv[])
{
    if (argc != 3 ){
        std::cout << "You need obj or texture file !\n";
        return 1;
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
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); 
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // hook user input
    glfwSetWindowSizeLimits(window, 400, 300, GLFW_DONT_CARE, GLFW_DONT_CARE);
    glfwSetWindowAspectRatio(window, 4, 3);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
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
    glDepthFunc(GL_LESS); 
    glEnable(GL_PROGRAM_POINT_SIZE);

    // Shader init
    // -----------
    Shader scop42shader("shader/scop.vs", "shader/scop.fs");

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
    const unsigned char* data = ftloader::BMP(argv[2], width, height);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
        delete[] data;
    }
    else{
        std::cerr << "Failed to load texture" << std::endl;
        glfwTerminate();
        return 1;
    }

    // Var Menu
    // --------
    cmd.color_mix = 0.0f;
    cmd.visual_mode = VM_FILL;
    cmd.autorotationX = false;
    cmd.autorotationY = true;
    cmd.autorotationZ = false;
    cmd.rotationX = 0.0f;
    cmd.rotationY = 0.0f;
    cmd.rotationZ = 0.0f;
    cmd.rotationSpeed = 60.0f;
    cmd.scaleX = 1.0f;
    cmd.scaleY = 1.0f;
    cmd.scaleZ = 1.0f;
    cmd.translateX = 0.0f;
    cmd.translateY = 0.0f;
    cmd.translateZ = 0.0f;
    cmd.pointAndLineSize = 1.0f;
    cmd.background_color[0] = 0.2f;
    cmd.background_color[1] = 0.3f;
    cmd.background_color[2] = 0.3f;

    ftmath::vec3 FacesColor[4];
    FacesColor[0]._x = 1.0f; FacesColor[0]._y = 1.0f; FacesColor[0]._z = 1.0f;
    FacesColor[1]._x = 0.65f; FacesColor[1]._y = 0.65f; FacesColor[1]._z = 0.65f;
    FacesColor[2]._x = 0.0f; FacesColor[2]._y = 0.0f; FacesColor[2]._z = 0.0f;
    FacesColor[3]._x = 0.45f; FacesColor[3]._y = 0.45f; FacesColor[3]._z = 0.45f;

    // mesh loading
    // ------------
    Mesh object;
    try{
        object = Mesh(argv[1], texture, scop42shader, FacesColor);
    }catch(std::exception& e){
        std::cerr << "Sorry we have an error...\n" << e.what();
        scop42shader.clean();
        glDeleteTextures(1, &texture);
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }
    const ftmath::vec3 objectCenter = object.getCenter();

    // view camera position
    // --------------------
    const ftmath::vec3 cameraPosition(0.0f, 0.0f, -10.0f);

    // prepare light
    // -------------
    ft_light light(1.0,1.0,1.0);
    light.setAmbientStrength(0.6);
    light.setLightStrength(1.0);
    scop42shader.setFloat3("viewPos", cameraPosition);

    // delta time
    // ----------
    dt.currentFrame = 0.0f;
    dt.deltaTime = 0.0f;
    dt.lastFrame = 0.0f;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // render
        // ------
        glClearColor(cmd.background_color[0], cmd.background_color[1], cmd.background_color[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // delta time
        // ----------
        dt.currentFrame = glfwGetTime();
        dt.deltaTime = dt.currentFrame - dt.lastFrame;
        dt.lastFrame = dt.currentFrame;

        // transition between color and texture
        // ------------------------------------
        if(cmd.color_mix_transition){
            cmd.color_mix = ftmath::lerp<float>(cmd.color_mix, 1.0f, dt.deltaTime);
        }else if (!cmd.color_mix_transition){
            cmd.color_mix = ftmath::lerp<float>(cmd.color_mix, 0.0f, dt.deltaTime);
        }
        scop42shader.setFloat("mixColor",cmd.color_mix);

        // light shader
        // ------------
        scop42shader.setFloat3("lightColor", light.getColor());
        scop42shader.setFloat("lightStrength", light.getLightStrength());
        scop42shader.setFloat("ambientStrength", light.getAmbientStrength());
        scop42shader.setFloat("specularStrength", light.getSpecularStrength());
        scop42shader.setFloat("specularShininess", light.getSpecularShininess());
        scop42shader.setFloat3("lightPos", light.getPos());

        // setup lite visual mode opengl vertex rendering
        //  ---------------------------------------------
        switch(cmd.visual_mode){
            case VM_WIREFRAME:
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
                glLineWidth(cmd.pointAndLineSize);
                break;
            case VM_POINT:
                glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); 
                scop42shader.setFloat("pointSize", cmd.pointAndLineSize);
                break;
            case VM_FILL:
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
                break;
        }

        // setup imgui menu
        // ----------------
        const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(SCR_WIDTH, 0));
        ImGui::Begin("Scop42 ToolBox", NULL, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_MenuBar);    
        if (ImGui::BeginMenuBar()){
            if (ImGui::BeginMenu("Rotation"))
            {
                ImGui::Checkbox("Auto X rotation", &cmd.autorotationX);
                ImGui::Checkbox("Auto Y rotation", &cmd.autorotationY); 
                ImGui::Checkbox("Auto Z rotation", &cmd.autorotationZ); 
                if (!cmd.autorotationX) ImGui::SliderFloat("X rotation", &cmd.rotationX, -180.0f, 180.0f);
                if (!cmd.autorotationY) ImGui::SliderFloat("Y rotation", &cmd.rotationY, -180.0f, 180.0f);
                if (!cmd.autorotationZ) ImGui::SliderFloat("Z rotation", &cmd.rotationZ, -180.0f, 180.0f); 
                ImGui::SliderFloat("Speed", &cmd.rotationSpeed, 0.0f, 120.0f);
                if (ImGui::Button("Reset")){
                    cmd.autorotationX = cmd.autorotationZ = false;
                    cmd.autorotationY = true;
                    cmd.rotationX = 0.0f;
                    cmd.rotationY = 0.0f;
                    cmd.rotationZ = 0.0f;
                    cmd.rotationSpeed = 60.0f;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        if (ImGui::BeginMenuBar()){
            if (ImGui::BeginMenu("Translate"))
            {
                ImGui::SliderFloat("X translate", &cmd.translateX, -20.0f, 20.0f);
                ImGui::SliderFloat("Y translate", &cmd.translateY, -20.0f, 20.0f);
                ImGui::SliderFloat("Z translate", &cmd.translateZ, -20.0f, 20.0f);
                if (ImGui::Button("Reset")){
                    cmd.translateX = 0.0f;
                    cmd.translateY = 0.0f;
                    cmd.translateZ = 0.0f;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        if (ImGui::BeginMenuBar()){
            if (ImGui::BeginMenu("Scale"))
            {
                ImGui::SliderFloat("X scale", &cmd.scaleX, 0.01f, 2.0f);
                ImGui::SliderFloat("Y scale", &cmd.scaleY, 0.01f, 2.0f);
                ImGui::SliderFloat("Z scale", &cmd.scaleZ, 0.01f, 2.0f);
                if (ImGui::Button("Reset")){
                    cmd.scaleX = 1.0f;
                    cmd.scaleY = 1.0f;
                    cmd.scaleZ = 1.0f;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        if (ImGui::BeginMenuBar()){
            if (ImGui::BeginMenu("Effect"))
            {
                ImGui::RadioButton("Wireframe", &cmd.visual_mode, VM_WIREFRAME);
                ImGui::RadioButton("Point", &cmd.visual_mode, VM_POINT);
                ImGui::RadioButton("Fill", &cmd.visual_mode, VM_FILL);
                switch(cmd.visual_mode){
                    case VM_POINT:
                        ImGui::SliderFloat("Point Size", &cmd.pointAndLineSize, 1.0f, 10.0f);
                        break;
                    case VM_WIREFRAME:
                        ImGui::SliderFloat("Line Size", &cmd.pointAndLineSize, 1.0f, 10.0f);
                        break;
                }
                ImGui::SliderFloat("Color Mix", &cmd.color_mix, 0.0f, 1.0f);
                if (ImGui::Button("Reset")){
                    cmd.visual_mode = VM_FILL;
                    cmd.color_mix = 0.0f;
                    cmd.pointAndLineSize = 0.0f;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        if (ImGui::BeginMenuBar()){
            if (ImGui::BeginMenu("Light"))
            {
                ImGui::SeparatorText("Color and intensity");
                ImGui::ColorEdit3("Color", &(light.getColor())[0]);
                ImGui::SliderFloat("Light Strength", &light.getLightStrength(), 0.0f, 1.0f);
                ImGui::SliderFloat("Ambient Strength", &light.getAmbientStrength(), 0.0f, 1.0f);
                ImGui::SeparatorText("Translate");
                ImGui::SliderFloat("X translate", &(light.getPos())[0], -10.0f, 10.0f);
                ImGui::SliderFloat("Y translate", &(light.getPos())[1], -10.0f, 10.0f);
                ImGui::SliderFloat("Z translate", &(light.getPos())[2], -10.0f, 10.0f);
                ImGui::SeparatorText("Background");
                ImGui::ColorEdit3("Background Color", cmd.background_color);
                if (ImGui::Button("Reset")){
                    light.reset();
                    cmd.background_color[0] = 0.2f;
                    cmd.background_color[1] = cmd.background_color[2] = 0.3f;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        ImGui::End();

        //  view matrix
        //  -----------
        ftmath::m4x4 view(1.0f);
        view = ftmath::translatem4(view, cameraPosition);
        scop42shader.setMatrix("view",view.toglsl());

        //  projection matrix
        //  -----------------
        // m4x4 persp(float fov, float ratio, float near, float far);
        ftmath::m4x4 projection = ftmath::persp(45.0f, SCR_WIDTH / SCR_HEIGHT, 0.01f, 1000.0f);
        scop42shader.setMatrix("projection",projection.toglsl());

        //  model matrix
        //  ------------
        object._modelMatrix = ftmath::translatem4(object._modelMatrix, ftmath::vec3(cmd.translateX - objectCenter._x, cmd.translateY - objectCenter._y, cmd.translateZ - objectCenter._z));
        if (cmd.autorotationX){
            object._modelMatrix = ftmath::rotatexm4(object._modelMatrix, (float)glfwGetTime() * cmd.rotationSpeed);
        }else{
            object._modelMatrix = ftmath::rotatexm4(object._modelMatrix, cmd.rotationX);
        }
        if (cmd.autorotationY){
            object._modelMatrix = ftmath::rotateym4(object._modelMatrix, (float)glfwGetTime() * cmd.rotationSpeed);
        }else{
            object._modelMatrix = ftmath::rotateym4(object._modelMatrix, cmd.rotationY);
        }
        if (cmd.autorotationZ){
            object._modelMatrix = ftmath::rotatezm4(object._modelMatrix, (float)glfwGetTime() * cmd.rotationSpeed);
        }else{
            object._modelMatrix = ftmath::rotatezm4(object._modelMatrix, cmd.rotationZ);
        }
        object._modelMatrix = ftmath::scalem4(object._modelMatrix, ftmath::vec3(cmd.scaleX, cmd.scaleY, cmd.scaleZ));
        object._modelMatrix = ftmath::translatem4(object._modelMatrix, ftmath::vec3(-objectCenter._x, -objectCenter._y, -objectCenter._z));
        object.Draw();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    object.clean();
    scop42shader.clean();
    glDeleteTextures(1, &texture);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

//  hook user input
//  ---------------
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //  texture to color
    //  ----------------
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS){
        cmd.color_mix_transition = !cmd.color_mix_transition;
    }
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }

    //  translation
    //  -----------
    const float max = 200.0f;
    const float min = -max;
    const float step = 5.0f;
    if (key == GLFW_KEY_W && action == GLFW_REPEAT){
        cmd.translateZ -= step * dt.deltaTime;
    }
    if (key == GLFW_KEY_S && action == GLFW_REPEAT){
        cmd.translateZ += step * dt.deltaTime;
    }
    if (key == GLFW_KEY_A && action == GLFW_REPEAT){
        cmd.translateX -= step * dt.deltaTime;
    }
    if (key == GLFW_KEY_D && action == GLFW_REPEAT){
        cmd.translateX += step * dt.deltaTime;
    }
    if (key == GLFW_KEY_Q && action == GLFW_REPEAT){
        cmd.translateY -= step * dt.deltaTime;
    }
    if (key == GLFW_KEY_E && action == GLFW_REPEAT){
        cmd.translateY += step * dt.deltaTime;
    }

    if (cmd.translateX > max) cmd.translateX = max;
    if (cmd.translateY > max) cmd.translateY = max;
    if (cmd.translateZ > max) cmd.translateZ = max;
    if (cmd.translateX < min) cmd.translateX = min;
    if (cmd.translateY < min) cmd.translateY = min;
    if (cmd.translateZ < min) cmd.translateZ = min;
}

//  scroll callback
//  ---------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    const float min = 0.010f;
    const float max = 20.0f;
    const float step = 5.0f;
    if (yoffset == 1){
        cmd.scaleX += step * dt.deltaTime;
        cmd.scaleY += step * dt.deltaTime;
        cmd.scaleZ += step * dt.deltaTime;
    }else if (yoffset == -1){
        cmd.scaleX -= step * dt.deltaTime;
        cmd.scaleY -= step * dt.deltaTime;
        cmd.scaleZ -= step * dt.deltaTime;
    }
    if (cmd.scaleX < min) cmd.scaleX = min;
    if (cmd.scaleY < min) cmd.scaleY = min;
    if (cmd.scaleZ < min) cmd.scaleZ = min;
    if (cmd.scaleX > max) cmd.scaleX = max;
    if (cmd.scaleY > max) cmd.scaleY = max;
    if (cmd.scaleZ > max) cmd.scaleZ = max;
}


//  hook when user rezie the window
//  -------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    SCR_HEIGHT = height;
    SCR_WIDTH = width;
    glViewport(0, 0, width, height);
}
