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

/*----Fragment Shader--*/

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
	glUseProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


/*---Vertex Array Object----*/

    // A VAO stores a vertex attribute configuration and which VBO to use.

    // vertices input for a triangle x, y, & z
	float vertices[] = {
		 0.5f,  0.5f, 0.0f, //top right
		 0.5f, -0.5f, 0.0f, //bottom right
	    -0.5f, -0.5f, 0.0f, //bottem left
        -0.5f,  0.5f, 0.0f  //top left
	};

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    //initialize vertex buffer object to store memory on GPU
	glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);


    // bind the vertex array
    glBindVertexArray(VAO);

    //bind the array buffer to our buffer VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//now we can write to the GL_ARRAY_BUFFER and it will write to our VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
            GL_STATIC_DRAW);


    //tell OpenGL to interpret vertex data per vertex attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 *sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);





    
/*-----RENDER LOOP-----*/

	// checks at the beggining of each loop if the window should be closed
	while(!glfwWindowShouldClose(window)){

		//input
		processInput(window);

		//rendering commands here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

        //draw triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);
		
		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);



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
