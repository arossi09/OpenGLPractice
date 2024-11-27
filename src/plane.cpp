#include "plane.h"


#include "thirdparty/stb/stb_perlin.h"

//plane takes the div amount width/size of the plane 
//and the position which is defualted to the origin
Plane::Plane(int div, int width, glm::vec3 position){

    this->div = div;
    this->width = width;
    this->Position = position;

    initPlane();

}

//sets the Div amount of the plane
void Plane::setDiv(int div){
    if(this->div != div){
        this->div = div;
        needsUpdate = true;
    }
}

//sets teh width/sie of the plane
void Plane::setWidth(int width){
    if(this->width != width){
        this->width = width;
        needsUpdate = true;
    }
}



//initializes the plane to flat surface
void Plane::initPlane(){

    vertices.clear();
    indices.clear();

    float triangleSide = width/div;
    for (int row = 0; row < div + 1; row++){
        for(int col = 0; col < div+1; col++){
            glm::vec3 crntVec = glm::vec3(col * triangleSide, 0.0f,
                    row* -triangleSide);
            vertices.push_back(crntVec.x);
            vertices.push_back(crntVec.y);
            vertices.push_back(-crntVec.z);
            vertices.push_back(0.0f);
            vertices.push_back(1.0f);
            vertices.push_back(0.0f);
        }
    }

    for(int row=0; row<div; row++){
        for(int col=0; col<div; col++){
            int index = row * (div+1) + col;
            //top triangle
            indices.push_back(index);
            indices.push_back(index + (div+1) + 1);
            indices.push_back(index + (div+1));
            //Bot triangle
            indices.push_back(index);
            indices.push_back(index + 1);
            indices.push_back(index + (div+1) + 1);
        }
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
   
    //bind the vertex object array for the vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), 
            vertices.data(), GL_STATIC_DRAW);

    //bind the element array buffer for the order of elements
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLint),
            indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 
            6*sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
            6*sizeof(GLfloat), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    needsUpdate = false;

}

//draws the plane 
void Plane::draw(Shader shader){
    if(needsUpdate){
        this->initPlane();
    }
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, Position);
    shader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

}

//given a shader, speed scale, and time value recaluclates the vectors
//of the plane based on a sin function and updated the VBO
void Plane::drawSinWave(Shader shader, float speedScale, float time){
    float scaledTime = speedScale * time;
    if(needsUpdate){
        this->initPlane();
    }

    //alter the heights of the vertices based on sin wave
    for(int row = 0; row < div+1; row++){
        for(int col = 0; col < div+1; col++){
            int vertexIndex = (row * (div + 1) + col) * 6 + 1;
            vertices[vertexIndex] =(float)sin(scaledTime+(row*0.5f)+(col*0.5f));
        }
    }


    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size()*sizeof(GLfloat),
            vertices.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    draw(shader);


}

//Given a shader, speed scale, height scale, and time the function
//recalculates the vectores based on the attributes passed and 
//perlin noise 
void Plane::drawPerlinWave(Shader shader, float speedScale,
       float heightScale,  float time){
    float scaledTime = speedScale * time;
    if(needsUpdate){
        this->initPlane();

    }

    //alter the heights of the vertices based on perlin noise
    for(int row = 0; row < div+1; row++){
        for(int col = 0; col < div+1; col++){
            float noise = stb_perlin_noise3(row * 0.1f, col * 0.1f, 
                    scaledTime * 0.1f, 0, 0, 0);
            float height = noise * heightScale;
            int vertexIndex = (row * (div + 1) + col) * 6 + 1;
            vertices[vertexIndex] = height;
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size()*sizeof(GLfloat),
            vertices.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);

   draw(shader);


}

Plane::~Plane(){
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
