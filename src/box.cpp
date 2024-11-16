#include "box.h"

#define STB_IMAGE_IMPLEMENTATION
#include "thirdparty/stb/stb_image.h"


//constructor for box
Box::Box(const char *text_path, glm::vec3 position){

    Position = position;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);



    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES,
            GL_STATIC_DRAW);

    //vertex attrib pointer for position chords
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    //give the vertex attribute the location as its argument
    glEnableVertexAttribArray(0);

    //vertex attrib pointer for tex cords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float),
            (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

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

	unsigned char *data = stbi_load(text_path, &width, &height,
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

}


Box::~Box(){
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

//return the position
glm::vec3 Box::getPosition(){
    return Position;
}

void Box::setPosition(glm::vec3 new_pos){
    Position = new_pos;
}


void Box::draw(Shader shader){
		
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, Position);
    shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, NUM_OF_VERTICES);

}


