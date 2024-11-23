#ifndef BOX_H
#define BOX_H

#include "shader.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Box{
	public:
        unsigned int VBO;
        unsigned int VAO;

        Box(const char *text_path, 
                glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f));
		
        void initBox();

        ~Box();

		//returns the position of the box
		glm::vec3 getPosition();

        //sets the position of the box
        void setPosition(glm::vec3 new_pos);

        //draw the box
        void draw(Shader shader);

        //draw a sin wave with width of waveX length of waveZ
        void drawSinWave(Shader shader, int waveX, int waveZ, float speedScale,
        float time);

        //draw the perlin noise wave 
        void drawPerlinWave(Shader shader, int waveX, int waveZ, float speedScale,
                float heightScale, float time);

        //draw the grid of boxes given the size and shader
        void drawGrid(Shader shader, int waveX, int waveZ);

	private:

    	//vector for world position
		glm::vec3 Position;

        const char *text_path;


	


};



#endif
