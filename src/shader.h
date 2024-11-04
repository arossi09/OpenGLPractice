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
};
#endif

