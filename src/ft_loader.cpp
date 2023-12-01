#include "ft_loader.hpp"
#include "ft_math.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

//  Load OBJ 3d model
//  -----------------
bool ftloader::OBJ(
            const char *path,
            std::vector<ftmath::vec3> &out_vertices,
            std::vector<ftmath::vec2> &out_uvs,
            std::vector<ftmath::vec3> &out_normals)
{
    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<ftmath::vec3> temp_vertices;
    std::vector<ftmath::vec2> temp_uvs;
    std::vector<ftmath::vec3> temp_normals;
    std::ifstream file;
    file.open(path, std::ios::in);
    if(!file.is_open()){
        std::cerr << "Error in OBJ loader: is_open()\n";
        return false;
    }
    std::string line;
    while (getline(file, line)){
        ftmath::vec3 v;
        if (line.substr(0, 2) == "v "){
            std::istringstream s(line.substr(2));
            s >> v._x;
            s >> v._y;
            s >> v._z;
            temp_vertices.push_back(v);
        }
        else if (line.substr(0, 3) == "vt "){
            std::istringstream s(line.substr(3));
            s >> v._x;
            s >> v._y;
            temp_uvs.push_back(ftmath::vec2(v._x, v._y));  
        }else if (line.substr(0, 3) == "vn "){
            std::istringstream s(line.substr(3));
            s >> v._x;
            s >> v._y;
            s >> v._z;
            temp_normals.push_back(v);
        }else if (line.substr(0, 2) == "f "){
            std::istringstream s(line.substr(2));
            std::string in_param[3];
            std::string out_param;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            for(unsigned int i = 0; i < 3; i++){
                s >> in_param[i];
            }
            std::stringstream ss_param[3];
            for(unsigned int i = 0; i < 3; i++){
                ss_param[i].str(in_param[i]);
            }
            for (unsigned int i = 0; i < 3; i++){
                unsigned int index = 0;
                while (getline(ss_param[i], out_param, '/')){
                    switch(index)
                    {
                        case 0:
                            vertexIndex[i] = std::stoul(out_param);
                            break;
                        case 1:
                            uvIndex[i] = std::stoul(out_param);
                            break;
                        case 2:
                            normalIndex[i] = std::stoul(out_param);
                            break;
                    }
                    index++;
                }
            }
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices.push_back(uvIndex[0]);
            uvIndices.push_back(uvIndex[1]);
            uvIndices.push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        }
    }
    for (unsigned int i = 0; i < vertexIndices.size(); i++){
        unsigned int vertexIndex = vertexIndices[i];
        ftmath::vec3 vertex = temp_vertices[vertexIndex - 1];
        out_vertices.push_back(vertex);
    }
    for (unsigned int i = 0; i < uvIndices.size(); i++){
        unsigned int uvIndex = uvIndices[i];
        ftmath::vec2 uv = temp_uvs[uvIndex - 1];
        out_uvs.push_back(uv);
    }
    for (unsigned int i = 0; i < normalIndices.size(); i++){
        unsigned int normalIndex = normalIndices[i];
        ftmath::vec3 normal = temp_normals[normalIndex - 1];
        out_normals.push_back(normal);
    }
    file.close();
    return true;
}

float* ftloader::OBJTOOPENGLVERTICES(
    unsigned int &out_size,
    const std::vector<ftmath::vec3> &vertices,
    const std::vector<ftmath::vec2> &uvs,
    const std::vector<ftmath::vec3> &normals
){
    if (vertices.size() < 1 || uvs.size() < 1 || normals.size() < 1){
        return (float*)0;
    }
    const int stride = 5;
    out_size = (vertices.size() * sizeof(ftmath::vec3) + uvs.size() * sizeof(ftmath::vec2));
    float *final_vertices = new float[out_size];
    int vpos = 0;
    int uvpos = 0;
    for (int line = 0; line < out_size; line+=stride)
    {
        for(int offset = 0; offset < stride; offset++){
                switch (offset){
                    case 0:
                        final_vertices[line + offset] = vertices[vpos]._x;
                        break;
                    case 1:
                        final_vertices[line + offset] = vertices[vpos]._y;
                        break;
                    case 2:
                        final_vertices[line + offset] = vertices[vpos]._z;
                        break;
                    case 3:
                        final_vertices[line + offset] = uvs[uvpos]._x;
                        break;
                    case 4:
                        final_vertices[line + offset] = uvs[uvpos]._y;
                        break;
                }
        }
        vpos++;
        uvpos++;
    }
    return final_vertices;
}

//  Load BMP image
//  -----------------
unsigned char* ftloader::BMP(
    const char *path,
    int &width,
    int &height
){
    std::string header(54, '\0');
    unsigned int dataPos;
    unsigned int imageSize;

    std::ifstream file;
    file.open(path, std::ios::in | std::ios::binary);
    if (!file.is_open()){
        std::cerr << "Failed to open BMP texture\n";
        return nullptr;
    }
    file.read(&header[0], 54);
    if(file.gcount() != 54){
        std::cerr << "Not a correct BMP file\n";
        return nullptr;
    }
    if ( header[0]!='B' || header[1]!='M' ){
        std::cerr << "ot a correct BMP file\n";
        return nullptr;
    }
    dataPos = *(int*)&(header[0x0A]);
    imageSize = *(int*)&(header[0x22]);
    width = *(int*)&(header[0x12]);
    height = *(int*)&(header[0x16]);

    unsigned char* data = new unsigned char[imageSize];
    if (imageSize == 0) imageSize = width * height * 3;
    if (dataPos == 0) dataPos = 54;
    file.read(reinterpret_cast<char*>(data), imageSize);
    file.close();
    return (data);
}