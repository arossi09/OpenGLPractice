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


//fix the fragment shader so that it takes in the light source
//vector and multiplies it by the color of the object
//
//add a setVec4 to the shader class 



//1. box class that takes x,y,z 
//saves its position and can be called with its verticies
//and position on the model to draw it
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
bool rayIntersectsLight(glm::vec3 rayWorld);
glm::vec3 calculateRayWorld(float xpos, float ypos, glm::mat4 projection);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int frameBufferWidth, frameBufferHeight;
    

//global flags 
bool wireframe = false;
bool cursorVisible = false;
bool lastState = false;
bool light_the_scene = false;
bool lightSelected = false;

glm::vec3 light_pos = glm::vec3(5.0f, 5.0f, 5.0f);

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


    bool drawBox = true;
    bool drawMesh = false;
    

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

    Shader lights_on_shader("src/shaders/lightingShader.vs", "src/shaders/lightingShader.fs");

    //shader for the light source
    Shader light_shader("src/shaders/shader.vs", "src/shaders/light_shader.fs");
    

    //creating the box object with the water texture
    Box box("resources/textures/water.jpg");

    //creating the light source box representation
    Box light_source("resources/textures/water.jpg");


    //the plane with default div and width amount
    Plane plane(div, width, glm::vec3(0.0f, 0.0f, 0.0f));

    //ImGUI Configuration
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");


    //setting the color of the object and the light
    glm::vec3 color = glm::vec3(1.0f, 0.5f, 0.5f);
    glm::vec3 light_color = glm::vec3(1.0f, 1.0f, 1.0f);


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


        //creating viewport, centered in the bottom right of screen
        glViewport(0, 0, frameBufferWidth, frameBufferHeight);

        //flag for swapping between wireframe and fill
        if(wireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        if(!wireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);



        // setting the camera look at position 
        glm::mat4 view = camera.GetViewMatrix(); 
        glm::mat4 model = glm::mat4(1.0f);
        //projection matrix for perspective
        glm::mat4 projection =
            glm::perspective(glm::radians(75.0f), (float)SCR_WIDTH/SCR_HEIGHT,
                    0.1f, 100.0f);

        //swaps between shaders based on if lights are on
        Shader& active_shader = light_the_scene ? lights_on_shader : ourShader;


        //sets the uniforms in each shader based on which one is active
        active_shader.use();
        active_shader.setMat4("projection", projection);
        active_shader.setMat4("model", model);
        active_shader.setMat4("view", view);
        active_shader.setVec3("objectColor", color);
        if(light_the_scene){
            active_shader.setVec3("viewPos", camera.Position);

            active_shader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
            active_shader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
            active_shader.setVec3("material.specular", 0.2f, 0.2f, 0.2f);
            active_shader.setFloat("material.shininess", 32.0f);


            glm::vec3 diffuseColor = light_color * glm::vec3(0.5f, 0.5f, 0.5f);
            glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f, 0.2f, 0.2f);
            active_shader.setVec3("light.ambient", ambientColor);
            active_shader.setVec3("light.diffuse", diffuseColor);
            active_shader.setVec3("light.specular", light_color);
            active_shader.setVec3("light.position", light_pos);
        }
        //swap between drawing plane and boxes 
        if(drawBox){
            if(perlin_noise)
                box.drawPerlinWave(active_shader, waveX, waveZ, speedScale, heightScale,
                        glfwGetTime());
            else if(sin_wave)
                box.drawSinWave(active_shader, waveX, waveZ, speedScale, glfwGetTime());
            else
                box.drawGrid(active_shader, waveX, waveZ);

        }else if(drawMesh){
            plane.setDiv(div);
            plane.setWidth(width);
            if(perlin_noise)
                plane.drawPerlinWave(active_shader, speedScale, heightScale,
                        glfwGetTime());
            else if(sin_wave)
                plane.drawSinWave(active_shader, speedScale,  glfwGetTime());
            else
                plane.draw(active_shader);
        }

        //drawing the light_source block
        light_shader.use();
        light_shader.setMat4("view", view);
        light_shader.setMat4("projection", projection);
        light_shader.setMat4("model", model);
        light_shader.setVec3("lightColor",light_color);
        light_source.setPosition(light_pos);
        light_source.draw(light_shader);

        glDisable(GL_DEPTH_TEST);


        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Wave Simulation");
        ImGui::SliderFloat3("Color of Light", &light_color[0], 0.0f, 1.0f);
        ImGui::Checkbox("Lights", &light_the_scene);
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
glm::vec3 calculateRayWorld(float xpos, float ypos, glm::mat4 projection){

    //raycastin: we need to convert the 2d cordiante to 3d cordinates
    //so that the mouse position is relative to the cameras projection
    //and blocks position in the 3d world
    float ndcX = (2.0f * xpos) / frameBufferWidth*2-1.0f;
    float ndcY = 1.0f - (2.0f * ypos) / frameBufferHeight*2; 
    glm::vec4 rayClip = glm::vec4(ndcX, ndcY,-1.0f,1.0f);

    //we need to tranform the ray to world space
    glm::vec4 rayEye = glm::inverse(projection) * rayClip;
    rayEye.z = -1.0f;
    rayEye.w = 0.0f;

    glm::vec3 rayWorld = glm::vec3(glm::inverse(camera.GetViewMatrix()) * rayEye);
    return glm::normalize(rayWorld);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn){
    //cast the x and y pos to float so camera class accepts them
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);



    if(!cursorVisible){

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
        static bool isDragging = false;
        static glm::vec3 rayWorld;

        glm::mat4 projection =
            glm::perspective(glm::radians(75.0f), (float)SCR_WIDTH/SCR_HEIGHT,
                    0.1f, 100.0f);

        if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
            if(!isDragging && lightSelected){
                isDragging = true;
            }else{
                if(lightSelected){
                    rayWorld = calculateRayWorld(xpos, ypos, projection);
                    //we need to restrict movement to the XZ plane by making the
                    //normal the y plane and computing the intersection with this
                    //plane 
                    glm::vec3 planeNormal = glm::vec3(camera.Front);
                    glm::vec3 planePoint = light_pos;

                    float t = glm::dot(planePoint - camera.Position, planeNormal) /
                        glm::dot(rayWorld, planeNormal);
                    glm::vec3 intersection = camera.Position + t * rayWorld;

                    light_pos = intersection;

                }

            }

        }
        if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE){
            rayWorld = calculateRayWorld(xpos, ypos, projection);
            lightSelected = rayIntersectsLight(rayWorld); 
            isDragging = false;
        }
        first_mouse = true;
    }
}

bool rayIntersectsLight(glm::vec3 rayWorld){
    glm::vec3 cubeMin =light_pos - glm::vec3(0.5f);
    glm::vec3 cubeMax =light_pos + glm::vec3(0.5f);

    glm::vec3 invDir = 1.0f / rayWorld; 
    glm::vec3 tMin = (cubeMin - camera.Position) * invDir;
    glm::vec3 tMax = (cubeMax - camera.Position) * invDir;

    glm::vec3 t1 = min(tMin, tMax);
    glm::vec3 t2 = max(tMin, tMax);

    float tNear = glm::max(glm::max(t1.x, t1.y), t1.z);
    float tFar = glm::min(glm::min(t2.x, t2.y), t2.z);

    if (tNear <= tFar && tFar >= 0.0f) {
        return true;
    } else {
        return false;
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

