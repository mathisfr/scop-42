#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "ft_math.hpp"
  
class Shader
{
public:
    // the program ID
    unsigned int ID;

    Shader();
    Shader(const char* vertexPath, const char* fragmentPath);

    void use();

    void setBool(const std::string &name, bool value) const;  
    void setInt(const std::string &name, int value) const;   
    void setFloat(const std::string &name, float value) const;
    void setMatrix(const std::string &name, float* value) const;
    void setFloat3(const std::string &name,  float value[3]) const;
    void setFloat3(const std::string &name,  ftmath::vec3 value) const;
    void clean();

    Shader& operator=(const Shader &other);
};
  
#endif