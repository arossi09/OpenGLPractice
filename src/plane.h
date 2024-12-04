#ifndef PLANE_H
#define PLANE_H

#include "shader.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Plane{

    public:

        Plane(int div, int width, glm::vec3 position);

        ~Plane();

        void draw(Shader shader);

        void drawPerlinWave(Shader shader, float speedScale,
                float heightScale, float time);

        void drawSinWave(Shader shader, float speedScale, float time);

        void setDiv(int div);

        void setWidth(int width);
        
        void initPlane();

    private:


        bool needsUpdate;
        unsigned int VAO, VBO, EBO;
        glm::vec3 Position;
        int div;
        float width;
        std::vector<GLfloat> vertices;
        std::vector<GLint> indices;



};
#endif
