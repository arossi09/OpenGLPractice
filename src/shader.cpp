#include "shader.h"

#include <string.h>


Shader::Shader(const char* vertexPath, const char* fragmentPath){
    //1. retrieve the vert and fragment from filePath
    int vShaderFile = open(vertexPath, O_RDONLY);
    int fShaderFile = open(fragmentPath, O_RDONLY);

    if( vShaderFile == -1 || fShaderFile == -1){
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_OPENED " <<
            strerror(errno) << std::endl;
        close(vShaderFile);
        close(fShaderFile);
        return;
    }

    //get the file sizes for buffers
    struct stat vStat, fStat;
    fstat(vShaderFile, &vStat);
    fstat(fShaderFile, &fStat);


    // create buffers for the code
    std::string vertexCode(vStat.st_size, ' ');
    std::string fragmentCode(fStat.st_size, ' ');

    // read the code from the files into buffers
    read(vShaderFile, &vertexCode[0], vStat.st_size);
    read(fShaderFile, &fragmentCode[0], fStat.st_size);

    close(vShaderFile);
    close(fShaderFile);

    //convert from string to const char because OPenGl expects it
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    //2. Compile and link Shaders
    
    unsigned int vertex, fragment;

    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompilerErrors(vertex, "VERTEX");

       // fragment shadr
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompilerErrors(fragment, "FRAGMENT");

    

    //shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    checkCompilerErrors(ID, "PROGRAM");


    glDeleteShader(vertex);
    glDeleteShader(fragment);
    
}

// Funciton to use shader object created
void Shader::use(){
    glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const
{ 
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::checkCompilerErrors(unsigned int shader, std::string type){
    int success;
    char infoLog[1024];
    if(type != "program"){
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: "
                    << type << "\n" 
                    << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
    }else{
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }

    }
}
