#include <iostream>
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
	GLFWwindow* window = glfwCreateWindow(800, 600, "Kai Futami", NULL, NULL);

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
		"void main() \n"
		"{\n"
		"  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
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

/*----Fragment Shader(Orange)--*/

	//creatign the fragment shader to identify the color of each pixel
	const char *fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		" FragColor = vec4(1.0f, .5f, .2f, 1.0f);\n"
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

/*----Fragment Shader(Yellow)--*/
	const char *fragmentShaderSource2 = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		" FragColor = vec4(1.0f, 1.0f, .2f, 1.0f);\n"
		"}\0";
	
	unsigned int fragmentYellow;
	fragmentYellow = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentYellow, 1, &fragmentShaderSource2, NULL);
	glCompileShader(fragmentYellow);




/*---Shader Program----*/

	// create program and returns ID to newly created object
	unsigned int shaderProgramOrange, shaderProgramYellow;
	shaderProgramOrange = glCreateProgram();
	shaderProgramYellow = glCreateProgram();

	//attach vertex shader & fragment shader to shader program
	//and link them
	glAttachShader(shaderProgramOrange, vertexShader);
	glAttachShader(shaderProgramOrange, fragmentShader);
	glLinkProgram(shaderProgramOrange);

	glAttachShader(shaderProgramYellow, vertexShader);
	glAttachShader(shaderProgramYellow, fragmentYellow);
	glLinkProgram(shaderProgramYellow);


	glGetProgramiv(shaderProgramOrange, GL_LINK_STATUS, &success);
	if(!success){
		glGetProgramInfoLog(shaderProgramOrange, 512, NULL, infoLog);
		std::cout << "ERROR:SHADER::PROGRAM::LINK_FAILED\n" << infoLog
			<< std::endl;
	}


	glGetProgramiv(shaderProgramYellow, GL_LINK_STATUS, &success);
	if(!success){
		glGetProgramInfoLog(shaderProgramYellow, 512, NULL, infoLog);
		std::cout << "ERROR:SHADER::PROGRAM::LINK_FAILED\n" << infoLog
			<< std::endl;
	}


	// Now we can use the shader program os that every shader and rendering call
	// after ths will now use this program object

/*---Vertex Array Object----*/

	// A VAO stores a vertex attribute configuration and which VBO to use.

	// vertices input for a triangle x, y, & z
	float firstTriangle[] = {
        -0.9f, -0.5f, 0.0f,  // left 
        -0.0f, -0.5f, 0.0f,  // right
        -0.45f, 0.5f, 0.0f,  // top 
  };
  float secondTriangle[] = {
        0.0f, -0.5f, 0.0f,  // left
        0.9f, -0.5f, 0.0f,  // right
        0.45f, 0.5f, 0.0f   // top 
  };


	unsigned int VAOs[2], VBOs[2];
	glGenVertexArrays(2, VAOs);
	//initialize vertex buffer object to store memory on GPU
	glGenBuffers(2, VBOs);


	//first triangle
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle,
			GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	//give the vertex attribute the location as its argument
	glEnableVertexAttribArray(0);

	//second triangle
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle,
			GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//give the vertex attribute the location as its argument
	glEnableVertexAttribArray(0);



    
/*-----RENDER LOOP-----*/

	// checks at the beggining of each loop if the window should be closed
	while(!glfwWindowShouldClose(window)){

		//input
		processInput(window);

		//rendering commands here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgramOrange);
		// drawi the first triangle with the first VAO
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//draw the second triangle with the second VAO
		glUseProgram(shaderProgramYellow);
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);


		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteProgram(shaderProgramOrange);
		glDeleteProgram(shaderProgramYellow);
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
