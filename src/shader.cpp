#include "shader.h"


Shader::Shader(const char* vertexPath, const char* fragmentPath){
    //1. retrieve the vert and fragment from filePath
    int vShaderFile = open(vertexPath, O_RDONLY);
    int fShaderFile = open(fragmentPath, O_RDONLY);

    if( vShaderFile == -1 || fShaderFile == -1){
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_OPENED" << std::endl;
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
    read(vShaderFile, &vertexCode, vStat.st_size);
    read(fShaderFile, &fragmentCode, fStat.st_size);

    close(vShaderFile);
    close(fShaderFile);

    //convert from string to const char because OPenGl expects it
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    //2. Compile and link Shaders






    
}
