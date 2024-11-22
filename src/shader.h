#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



// these may be usaeless
class Shader
{
    public:
        //the program id
        unsigned int ID;

        //constructor reads and builds the shader
        Shader(const char* vertexPath, const char* fragmentPath);

        //use or activate the shader
        void use();

        //utility uniform funcitons
        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;
        void setMat4(const std::string &name, glm::mat4 mat) const; 
        void setVec3(const std::string &name, glm::vec3 &vector) const; 
        void setVec3(const std::string &name, float x, float y, float z) const;

    private:
        //utility function for checking shader compile/linking errors
        void checkCompilerErrors(unsigned int shader, std::string type);
 
};
#endif

