#include <iostream>
#include <math.h>
#include "shader.h"
#include "camera.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



//1. box class that takes x,y,z 
//saves its position and can be called with its verticies
//and position on the model to draw it



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float last_x = SCR_WIDTH/2.0f;
float last_y = SCR_HEIGHT/2.0f;
bool first_mouse = true;

float delta_time = 0.0f; // time between current fram and last frame
float last_frame = 0.0f; // Time of last frame

int main(){ 

	/*--Initialize Window--*/

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	//create the window with size of 800 and 600 title Learn Open GL
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "o:", NULL, NULL);

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







	/*---Build and compile shader program----*/

	glEnable(GL_DEPTH_TEST);

	Shader ourShader("src/shaders/shader.vs", "src/shaders/shader.fs");



	// vertices input for two triangles to form a rectangle x, y, & z
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	//different cube positions
	glm::vec3 cubePositions[] = {
		glm::vec3( 0.0f,  0.0f,  0.0f), 
		glm::vec3( 2.0f,  5.0f, -15.0f), 
		glm::vec3(-1.5f, -2.2f, -2.5f),  
		glm::vec3(-3.8f, -2.0f, -12.3f),  
		glm::vec3( 2.4f, -0.4f, -3.5f),  
		glm::vec3(-1.7f,  3.0f, -7.5f),  
		glm::vec3( 1.3f, -2.0f, -2.5f),  
		glm::vec3( 1.5f,  2.0f, -2.5f), 
		glm::vec3( 1.5f,  0.2f, -1.5f), 
		glm::vec3(-1.3f,  1.0f, -1.5f)  
	};

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	//initialize vertex buffer object to store memory on GPU
	glGenBuffers(1, &VBO);


	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
			GL_STATIC_DRAW);

	//vertex attrib pointer for position chords
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
	//give the vertex attribute the location as its argument
	glEnableVertexAttribArray(0);

	//vertex attrib pointer for tex cords
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float),
			(void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	/* load and create texture */

	//texture 1
	//use stb_image.h to load the data of the image
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);

	// creating a texture object
	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	// set the texture wrapping/filtering options on current bounded texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	unsigned char *data = stbi_load("resources/textures/container.jpg", &width, &height,
			&nrChannels, 0);

	if(data){
		//Generate a texture for the GL_TEXTURE_2D we bounded
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
				GL_UNSIGNED_BYTE, data);
		//Generate all the needed mipmaps for the bounded texture
		glGenerateMipmap(GL_TEXTURE_2D); 
	}else{
		std::cout << "Failed to load texture" << std::endl;
	}

	// free the texture data
	stbi_image_free(data);

	/*-----RENDER LOOP-----*/

	// checks at the beggining of each loop if the window should be closed
	while(!glfwWindowShouldClose(window)){

		float current_frame = static_cast<float>(glfwGetTime());
		delta_time = current_frame - last_frame;
		last_frame = current_frame;

		//input
		processInput(window);

		//rendering commands here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		ourShader.use();

		// setting the camera look at position 
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("view", view);

		//projection matrix for perspective
		glm::mat4 projection =
			glm::perspective(glm::radians(75.0f), (float)SCR_WIDTH/SCR_HEIGHT, 0.1f, 100.0f);
		ourShader.setMat4("projection", projection);


		//creating viewport, centered in the bottom right of screen with 
		//dimensions
		//from before
		glViewport(0, 0, frameBufferWidth, frameBufferHeight);


		glBindVertexArray(VAO);
		for(unsigned int i = 0; i < 10; i++){
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = sin(glfwGetTime()) * 75.0f;

			if(i%3 == 0)
				angle = glfwGetTime() * 25.0f;

			model = glm::rotate(model, 
					glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));

			ourShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

        


		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);



	//clean all of GLFW's resources that were allocated
	glfwTerminate();
	return 0;



}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn){


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
}

//Set up this function so that every time the windows size is changed
//the following commands are executed
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	glViewport(0, 0, width, height);
}

/* Gathers and processes key input fro mthe window*/
void processInput(GLFWwindow *window){

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
