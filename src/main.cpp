#include <iostream>
#include <math.h>
#include "shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

float delta_time = 0.0f; // time between current fram and last frame
float last_frame = 0.0f; // Time of last frame

float mix_amount = 0.2f;
// the position vector of the camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
// a vector pointing to where the front of the camera
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
// vector relative to what we want the up direction to be
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

int main(){

    
    
    
/*--Initialize Window--*/

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//create the window with size of 800 and 600 title Learn Open GL
	GLFWwindow* window = glfwCreateWindow(800, 600, "o:", NULL, NULL);

	//error checking
	if(window == NULL){

		std::cout << "failed to create window" << std::endl;

		glfwTerminate();
		return -1;
	
	}



	glfwMakeContextCurrent(window);

	//Initializing GLAD to our os
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){

		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//creating viewport, centered in the bottom right of screen with dimensions
	//from before
	glViewport(0, 0, 800, 600);


	// we have to tell GLFW to call this function on every rezizing by
	// registering it
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    
		
/*---Build and compile shader program----*/

    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Shader ourShader("src/shaders/shader.vs", "src/shaders/shader.fs");

    //projection matrix for perspective
    glm::mat4 projection;
    projection =
        glm::perspective(glm::radians(75.0f), 800.0f/600.0f, 0.1f, 100.0f);




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
	unsigned int texture1, texture2;
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

	//texture 2
	
	//bind the texture to the GL_TEXTURE_2D
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	//load the texture data
	data = stbi_load("resources/textures/awesomeface.png", &width, 
					&height, &nrChannels, 0);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	if(data){
			//Generate a texture for the GL_TEXTURE_2D we bounded
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
							GL_UNSIGNED_BYTE, data);
			//Generate all the needed mipmaps for the bounded texture
			glGenerateMipmap(GL_TEXTURE_2D); 
	}else{
			std::cout << "Failed to load texture" << std::endl;
	}
	
	stbi_image_free(data);

	// telling which texture unit each sampler belongs to
	ourShader.use();
	ourShader.setInt("texture2", 1);

    
/*-----RENDER LOOP-----*/

	// checks at the beggining of each loop if the window should be closed
	while(!glfwWindowShouldClose(window)){

        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

		//input
		processInput(window);

		//rendering commands here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

  
        glm::mat4 view;
        // setting the camera look at position the second paramter is the
        // position we are at plus the direction we defined so the camera
        // keeps looking at the target location
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		ourShader.setFloat("mAmount", mix_amount); 
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);

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



		ourShader.use();
       



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


	//Set up this function so that every time the windows size is changed
	//the following commands are executed
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
		glViewport(0, 0, width, height);
}

/* Gathers and processes key input fro mthe window*/
void processInput(GLFWwindow *window){

	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
		mix_amount += 0.1f;
		if(mix_amount >=  1){
			mix_amount = 1;
		}
		printf("new mix ammount %.2f\n", mix_amount);
	}

	if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
		mix_amount -= 0.1f;
		if(mix_amount <= 0){
			mix_amount = 0;
		}
		printf("new mix ammount %.2f\n", mix_amount);
	}

    const float camera_speed = 2.5f * delta_time;
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += camera_speed * cameraFront;
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= camera_speed * cameraFront;
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * camera_speed;
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * camera_speed;








}
