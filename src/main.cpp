#include <iostream>
#include "thirdparty/imgui/imgui.h"
#include "thirdparty/imgui/backends/imgui_impl_glfw.h"
#include "thirdparty/imgui/backends/imgui_impl_opengl3.h"
#include <math.h>
#include "shader.h"
#include "box.h"
#include "camera.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#define STB_PERLIN_IMPLEMENTATION
#include "thirdparty/stb/stb_perlin.h"



//1. box class that takes x,y,z 
//saves its position and can be called with its verticies
//and position on the model to draw it



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1200;

bool wireframe = false;
bool cursorVisible = false;
bool lastState = false;

//camera
Camera camera(glm::vec3(5.0f, 5.0f, 20.0f));

float last_x = SCR_WIDTH/2.0f;
float last_y = SCR_HEIGHT/2.0f;
bool first_mouse = true;

float delta_time = 0.0f; // time between current fram and last frame
float last_frame = 0.0f; // Time of last frame
                        

int main(){ 

    float heightScale = 4.0f;
    float speedScale = 4.0f;
    int waveX = 5;
    int waveZ = 5;

    bool perlin_noise = true;
    bool sin_wave = false;

	/*--Initialize Window--*/

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




	int frameBufferWidth, frameBufferHeight;
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






	glEnable(GL_DEPTH_TEST);

	/*---Build and compile shader program----*/



	Shader ourShader("src/shaders/shader.vs", "src/shaders/shader.fs");




    Box box("resources/textures/water.jpg");


/*----Im Gui----*/

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");




/*-----RENDER LOOP-----*/

	// checks at the beggining of each loop if the window should be closed
	while(!glfwWindowShouldClose(window)){

		float current_frame = static_cast<float>(glfwGetTime());
		delta_time = current_frame - last_frame;
		last_frame = current_frame;

		//input
		processInput(window);



		//rendering commands here
		glClearColor(0.55f, 0.67f, 0.94f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader.use();

        //creating viewport, centered in the bottom right of screen with 
		//dimensions
		//from before
		glViewport(0, 0, frameBufferWidth, frameBufferHeight);


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


       
        //flag for swapping between wireframe and fill
        if(wireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        if(!wireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	



        if(perlin_noise){

            //draw the boxes with perlin noise
            for(int i = 0; i < waveX; i++){
                for(int j = 0; j < waveZ; j++){
                    float time = glfwGetTime()*speedScale;
                    float noise = stb_perlin_noise3(i * 0.1f, j * 0.1f, 
                            time * 0.1f, 0, 0, 0);

                    float height = noise * heightScale;

                    box.setPosition(glm::vec3(i, height, j));
                    box.draw(ourShader);            
                }
            }


        }
        if(sin_wave){

            //draw the boxes with sin wave function
            for(int i = 0; i < waveX; i++){
                for(int j = 0; j < waveZ; j++){
                    float time = glfwGetTime()*speedScale;
                    box.setPosition(glm::vec3(i,
                            (float)sin(time+(i*0.5f)+(j*0.5f)),j));
                    box.draw(ourShader);
                }
            }

        }




     

        glDisable(GL_DEPTH_TEST);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Wave Settings");


        //handle the swap beetween box checked for sin and perlin noise, so
        //both boxes can't be ticked at the same time
        if(ImGui::Checkbox("Perlin Noise", &perlin_noise)){
            if(perlin_noise){
                sin_wave = false;

            }

        }
        if(ImGui::Checkbox("Sin Wave", &sin_wave)){
            if(sin_wave){
                perlin_noise = false;
            }

        }
        ImGui::SliderFloat("Height Scale", &heightScale,1.0f, 10.0f);
        ImGui::SliderInt("Wave Width", &waveX, 5, 100); 
        ImGui::SliderInt("Wave Length", &waveZ, 5, 100);
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
	glViewport(0, 0, width, height);
}

/* Gathers and processes key input fro mthe window*/
void processInput(GLFWwindow *window){


    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        wireframe = !wireframe;
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

    if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){

        lastState = cursorVisible;


        cursorVisible = !cursorVisible;



        if(cursorVisible){
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }else{
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }

    }


	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProccessKeyboard(FORWARD, delta_time);
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProccessKeyboard(BACKWARD, delta_time);
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProccessKeyboard(LEFT, delta_time);
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProccessKeyboard(RIGHT, delta_time);

}
