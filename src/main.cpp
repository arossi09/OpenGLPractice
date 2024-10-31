#include <iostream>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main(){
    
    int success;
	char infoLog[512];
    
    
/*--Initialize Window--*/

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//create the window with size of 800 and 600 title Learn Open GL
	GLFWwindow* window = glfwCreateWindow(800, 600, "ELIANA", NULL, NULL);

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



		
/*-----Vertex Shader----*/

	//instantiating the vertex shader source code to translate the points to
	//gl_Position.
	const char *vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aColor;\n"
        "out vec3 ourColor;\n"
		"void main() \n"
		"{\n"
		"  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "  ourColor = aColor;\n"
		"}\0";

	// create vertex shader object
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//attach the shader source code to the shader object and compile
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//check if the shader compiled with no errors
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if(!success){
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR:SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog
			<< std::endl;
	}

/*----Fragment Shader--*/

	//creatign the fragment shader to identify the color of each pixel
	const char *fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
        "in vec3 ourColor;\n"
		"void main()\n"
		"{\n"
		" FragColor = vec4(ourColor, 1.0f);\n"
		"}\0";

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if(!success){
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR:SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog
			<< std::endl;
	}




/*---Shader Program----*/

	// create program and returns ID to newly created object
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	//attach vertex shader & fragment shader to shader program
	//and link them
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);


	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success){
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR:SHADER::PROGRAM::LINK_FAILED\n" << infoLog
			<< std::endl;
	}


   

	// Now we can use the shader program os that every shader and rendering call
	// after ths will now use this program object

/*---Vertex Array Object----*/

	// A VAO stores a vertex attribute configuration and which VBO to use.

	// vertices input for a triangle x, y, & z
	float firstTriangle[] = {
        // positions         // colors
        -0.9f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // left 
        -0.0f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,// right
        -0.45f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f// top 
    };



	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	//initialize vertex buffer object to store memory on GPU
	glGenBuffers(1, &VBO);


	//first triangle
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle,
			GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
	//give the vertex attribute the location as its argument
	glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), 
            (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(VAO);



    
/*-----RENDER LOOP-----*/

	// checks at the beggining of each loop if the window should be closed
	while(!glfwWindowShouldClose(window)){

		//input
		processInput(window);

		//rendering commands here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);


        // change green value on every loop
        float timeValue = glfwGetTime();
        float greenValue = sin(timeValue) / 2.0f + 0.5f;
        //get the shader location
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		// drawi the first triangle with the first VAO
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);





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
}
