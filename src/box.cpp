#include "box.h"

#define STB_IMAGE_IMPLEMENTATION
#include "thirdparty/stb/stb_image.h"

#define STB_PERLIN_IMPLEMENTATION
#include "thirdparty/stb/stb_perlin.h"


const float NUM_OF_VERTICES = 36;

//vertices for 6 sided box with tex cordinates
float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};



//constructor for box
Box::Box(const char *text_path, glm::vec3 position){

    Position = position;
    this->text_path = text_path;
    
    initBox();
}

//Initalize the box object
void Box::initBox(){

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
            GL_STATIC_DRAW);

    glBindVertexArray(VAO);

    //vertex attrib pointer for position chords
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    //give the vertex attribute the location as its argument
    glEnableVertexAttribArray(0);

    //normal vectors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);


/*    if(text_path){



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
    */
}

//deconstructor for the box object
Box::~Box(){
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

//return the position
glm::vec3 Box::getPosition(){
    return Position;
}

//set the position of the box 
void Box::setPosition(glm::vec3 new_pos){
    Position = new_pos;
}

//Given a shader draws a box
void Box::draw(Shader shader){
    glBindVertexArray(VAO);
		
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, Position);
    shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, NUM_OF_VERTICES);

}

//draws the the perlin noise wave by repositioning a box based on the 
//the noise and drawing each one
void Box::drawPerlinWave(Shader shader, int waveX, int waveZ, float speedScale,
        float heightScale, float time){


    float scaledTime = time * speedScale;
    //draw the boxes with perlin noise
    for(int i = 0; i < waveX; i++){
        for(int j = 0; j < waveZ; j++){
            float noise = stb_perlin_noise3(i * 0.1f, j * 0.1f, 
                    scaledTime * 0.1f, 0, 0, 0);

            float height = noise * heightScale;

            this->setPosition(glm::vec3(i, height, j));
            this->draw(shader);
        }
    }


}

//Repositions the box based on a sin wave and draws each one 
void Box::drawSinWave(Shader shader, int waveX, int waveZ, float speedScale,
        float time){
    float scaledTime = time * speedScale;
    //draw the boxes with sin wave function
    for(int i = 0; i < waveX; i++){
        for(int j = 0; j < waveZ; j++){
            this->setPosition(glm::vec3(i,
                        (float)sin(scaledTime+(i*0.5f)+(j*0.5f)),j));
            this->draw(shader);
        }
    }

}

