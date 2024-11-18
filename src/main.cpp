#include <iostream>
#include "thirdparty/imgui/imgui.h"
#include "thirdparty/imgui/backends/imgui_impl_glfw.h"
#include "thirdparty/imgui/backends/imgui_impl_opengl3.h"
#include <math.h>
#include "shader.h"
#include "box.h"
#include "camera.h"
#include "plane.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <glm/gtc/type_ptr.hpp>


// ctrl ^, %, ctrl b and d, $




//1. box class that takes x,y,z 
//saves its position and can be called with its verticies
//and position on the model to draw it
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int frameBufferWidth, frameBufferHeight;
    

//global flags 
bool wireframe = false;
bool cursorVisible = false;
bool lastState = false;

//camera
Camera camera(glm::vec3(5.0f, 5.0f, 20.0f));

//for mouse offset
float last_x = SCR_WIDTH/2.0f;
float last_y = SCR_HEIGHT/2.0f;
bool first_mouse = true;

// time between current fram and last frame
float delta_time = 0.0f; 
// Time of last frame
float last_frame = 0.0f; 


int main(){ 


    float heightScale = 4.0f;
    float speedScale = 4.0f;
    int waveX = 5;
    int waveZ = 5;
    int div = 5;
    float width =5.0f;

    bool perlin_noise = true;
    bool sin_wave = false;


    bool drawBox = false;
    bool drawMesh = true;
    

    //Initialize Window

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



    //create the window with size of 800 and 600 title Learn Open GL
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "o:", NULL, 
            NULL);

    //error checking
    if(window == NULL){

        std::cout << "failed to create window" << std::endl;

        glfwTerminate();
        return -1;

    }
    glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    // we have to tell GLFW to call this function on every rezizing by
    // registering it
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //Initializing GLAD to our os
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){

        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //enable depth test so objects are drawn behind eachother correctyl
    glEnable(GL_DEPTH_TEST);

    //build and compile the shader
    Shader ourShader("src/shaders/shader.vs", "src/shaders/shader.fs");

    //creating the box object with the water texture
    Box box("resources/textures/water.jpg");

    Plane plane(div, width, glm::vec3(0.0f, 0.0f, 0.0f));

    //ImGUI Configuration
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");


    /*-----RENDER LOOP-----*/

    // checks at the beggining of each loop if the window should be closed
    while(!glfwWindowShouldClose(window)){

        //calculating the delta time
        float current_frame = static_cast<float>(glfwGetTime());
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        //input
        processInput(window);

        //rendering commands here
        //glClearColor(0.55f, 0.67f, 0.94f, 1.0f);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();

        //creating viewport, centered in the bottom right of screen
        glViewport(0, 0, frameBufferWidth, frameBufferHeight);

        //flag for swapping between wireframe and fill
        if(wireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        if(!wireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // setting the camera look at position 
        glm::mat4 view = camera.GetViewMatrix(); 
        ourShader.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        ourShader.setMat4("model", model);

        //projection matrix for perspective
        glm::mat4 projection =
            glm::perspective(glm::radians(75.0f), (float)SCR_WIDTH/SCR_HEIGHT,
                    0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        if(drawBox){
            //swap between perlin noise and sin wave for box
            if(perlin_noise)
                box.drawPerlinWave(ourShader, waveX, waveZ, speedScale, heightScale,
                        glfwGetTime());
            else if(sin_wave)
                box.drawSinWave(ourShader, waveX, waveZ, speedScale, glfwGetTime());
            else
                box.draw(ourShader);

        }else if(drawMesh){
            plane.setDiv(div);
            plane.setWidth(width);
            if(perlin_noise)
                plane.drawPerlinWave(ourShader, speedScale, heightScale,
                        glfwGetTime());
            else if(sin_wave)
                plane.drawSinWave(ourShader, speedScale, heightScale, 
                        glfwGetTime());
            else
                plane.draw(ourShader);
        }


        glDisable(GL_DEPTH_TEST);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Wave Simulation");

        if(ImGui::Checkbox("Plane", &drawMesh)){
            plane.initPlane();
            if(drawBox)
                drawBox = false;
        }
        if(ImGui::Checkbox("Boxes", &drawBox)){
            box.initBox();
            if(drawMesh)
                drawMesh = false;
        }
        //handle the swap beetween box checked for sin and perlin noise, so
        //both boxes can't be ticked at the same time
        if(ImGui::Checkbox("Perlin Noise", &perlin_noise)){
            if(drawMesh)
                plane.initPlane();
            if(drawBox)
                box.initBox();
            if(perlin_noise){
                sin_wave = false;
            }
        }

        if(ImGui::Checkbox("Sin Wave", &sin_wave)){
            if(drawMesh)
                plane.initPlane();
            if(drawBox)
                box.initBox();
            if(sin_wave){
                perlin_noise = false;
            }

        }
        ImGui::Text("Box settings:");
        ImGui::SliderInt("Box Wave Width", &waveX, 5, 100); 
        ImGui::SliderInt("Box Wave Length", &waveZ, 5, 100);
        ImGui::Text("Plane settings:");
        ImGui::SliderInt("Plane Div Amount", &div, 1.0f, 100.0f);
        ImGui::SliderFloat("Plane Size", &width, 1.0f, 100.0f);
        ImGui::Text("Wave Settings:"); 
        ImGui::SliderFloat("Height Scale", &heightScale,1.0f, 10.0f);
        ImGui::SliderFloat("Wave Speed", &speedScale, 1.0f, 10.0f);
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glEnable(GL_DEPTH_TEST);



        // check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();



    //clean all of GLFW's resources that were allocated
    glfwTerminate();
    return 0;



}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn){

    if(!cursorVisible){

        //cast the x and y pos to float so camera class accepts them
        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        // to compensate from major change in offset based off creation of window
        if(first_mouse){
            last_x = xpos;
            last_y = ypos;
            first_mouse = false;
        }

        //calculate the offsets of the x and y position of mouse
        float xoffset = xpos - last_x;
        float yoffset = last_y - ypos;

        last_x = xpos;;
        last_y = ypos;

        camera.ProcessMouseMovement(xoffset, yoffset);
    }else{
        first_mouse = true;
    }
}

//Set up this function so that every time the windows size is changed
//the following commands are executed
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    frameBufferWidth = width;
    frameBufferHeight = height;
    glViewport(0, 0, width, height);
}

/* Gathers and processes key input fro mthe window*/
void processInput(GLFWwindow *window){

    static bool eKeyWasPressed = false;
    static bool spaceKeyWasPressed = false;

    //toggle draw options between wireframe and fill
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !spaceKeyWasPressed){
        wireframe = !wireframe;
        spaceKeyWasPressed = true;
    }

    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE){
        //reset the e key
        spaceKeyWasPressed = false;
    }

    // toggle between cursor and camera 
    if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && !eKeyWasPressed){

        cursorVisible = !cursorVisible;

        if(cursorVisible){
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }else{
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }

        //mark the e key as pressed 
        eKeyWasPressed = true;
    }

    if(glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE){
        //reset the e key
        eKeyWasPressed = false;
    }


    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProccessKeyboard(FORWARD, delta_time);
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProccessKeyboard(BACKWARD, delta_time);
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProccessKeyboard(LEFT, delta_time);
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProccessKeyboard(RIGHT, delta_time);

}
