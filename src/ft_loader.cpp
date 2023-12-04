#include "ft_loader.hpp"
#include "ft_math.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>

typedef struct OBJ_FORMAT{
    bool vertex;
    bool uv;
    bool normal;
}   OBJ_FORMAT;

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
    OBJ_FORMAT objFormat;
    objFormat.vertex = objFormat.uv = objFormat.normal = false;
    std::ifstream file;
    const int triangleFace = 3;
    file.open(path, std::ios::in);
    if(!file.is_open()){
        std::cerr << "Error in OBJ loader: is_open()\n";
        return false;
    }
    std::string line;
    while (getline(file, line)){
        ftmath::vec3 v;
        if (line.substr(0, 2) == "v "){
            objFormat.vertex = true;
            std::istringstream s(line.substr(2));
            s >> v._x;
            s >> v._y;
            s >> v._z;
            temp_vertices.push_back(v);
        }
        else if (line.substr(0, 3) == "vt "){
            objFormat.uv = true;
            std::istringstream s(line.substr(3));
            s >> v._x;
            s >> v._y;
            temp_uvs.push_back(ftmath::vec2(v._x, v._y));  
        }else if (line.substr(0, 3) == "vn "){
            objFormat.normal = true;
            std::istringstream s(line.substr(3));
            s >> v._x;
            s >> v._y;
            s >> v._z;
            temp_normals.push_back(v);
        }else if (line.substr(0, 2) == "f "){
            std::istringstream s(line.substr(2));
            std::vector<std::string> in_param;
            std::string tmp_in_param = "";
            std::string out_param;
            #ifdef DEBUG
            std::cout << "\n---------[\n";
            #endif
            while(s >> tmp_in_param){
                in_param.push_back(tmp_in_param);
                #ifdef DEBUG
                std::cout << tmp_in_param << '\n';
                #endif
            }
            int params_size = in_param.size();
            #ifdef DEBUG
            std::cout << "\n\n\nparams_size "<< in_param.size() <<"\n\n\n";
            #endif
            if (params_size < 3 || params_size > 4){
                std::cerr << "Error load OBJ\n";
                file.close();
                return false;
            }
            #ifdef DEBUG
            std::cout << "]---------\n\n";
            #endif
            std::stringstream ss_param[triangleFace];
            #ifdef DEBUG
            std::cout << "f ";
            #endif
            for(unsigned int i = 0; i < triangleFace; i++){
                ss_param[i].str(in_param[i]);
                #ifdef DEBUG
                std::cout << ss_param[i].str() << '\t';
                #endif
            }
            #ifdef DEBUG
            std::cout << '\n';
            #endif
            for (unsigned int i = 0; i < triangleFace; i++){
                unsigned int index = 0;
                while (getline(ss_param[i], out_param, '/')
                    || ((!objFormat.vertex && !objFormat.uv) && getline(ss_param[i], out_param, '\n'))){
                    switch(index)
                    {
                        case 0:
                            vertexIndices.push_back(std::stoul(out_param));
                            #ifdef DEBUG
                            std::cout << "v " << std::stoul(out_param) << '\t';
                            #endif
                            break;
                        case 1:
                            uvIndices.push_back(std::stoul(out_param));
                            #ifdef DEBUG
                            std::cout << "vt " << std::stoul(out_param) << '\t';
                            #endif
                            break;
                        case 2:
                            normalIndices.push_back(std::stoul(out_param));
                            #ifdef DEBUG
                            std::cout << "vn " << std::stoul(out_param);
                            #endif
                            break;
                    }
                    #ifdef DEBUG
                    std::cout << '\n';
                    #endif
                    index++;
                }
            }
            if (params_size == 4){
                std::stringstream ss_param_convert[triangleFace];
                #ifdef DEBUG
                std::cout << "\nf ";
                #endif
                for(unsigned int i = 0; i < triangleFace; i++){
                    ss_param_convert[i].str(in_param[(i+2)%4]);
                    #ifdef DEBUG
                    std::cout << ss_param_convert[i].str() << '\t';
                    #endif
                }
                #ifdef DEBUG
                std::cout << '\n';
                #endif
                for (unsigned int i = 0; i < triangleFace; i++){
                unsigned int index = 0;
                while (getline(ss_param_convert[i], out_param, '/')
                    || ((!objFormat.vertex && !objFormat.uv) && getline(ss_param_convert[i], out_param, '\n'))){
                    switch(index)
                    {
                        case 0:
                            vertexIndices.push_back(std::stoul(out_param));
                            #ifdef DEBUG
                            std::cout << "v " << std::stoul(out_param) << '\t';
                            #endif
                            break;
                        case 1:
                            uvIndices.push_back(std::stoul(out_param));
                            #ifdef DEBUG
                            std::cout << "vt " << std::stoul(out_param) << '\t';
                            #endif
                            break;
                        case 2:
                            normalIndices.push_back(std::stoul(out_param));
                            #ifdef DEBUG
                            std::cout << "vn " << std::stoul(out_param);
                            #endif
                            break;
                    }
                    #ifdef DEBUG
                    std::cout << '\n';
                    #endif
                    index++;
                }
            }
            }
        }
    }
    #ifdef DEBUG
    std::cout << "ok 3"<< std::endl;
    #endif
    if (objFormat.vertex == false){
        std::cerr << "Error load OBJ\n";
        file.close();
        return false;
    }
    #ifdef DEBUG
    std::cout << "ok 4"<< std::endl;
    #endif
    for (unsigned int i = 0; i < vertexIndices.size(); i++){
        unsigned int vertexIndex = vertexIndices[i];
        ftmath::vec3 vertex = temp_vertices[vertexIndex - 1];
        out_vertices.push_back(vertex);
    }
    #ifdef DEBUG
    std::cout << "ok 5"<< std::endl;
    #endif
    if (objFormat.uv){
        for (unsigned int i = 0; i < uvIndices.size(); i++){
            unsigned int uvIndex = uvIndices[i];
            ftmath::vec2 uv = temp_uvs[uvIndex - 1];
            out_uvs.push_back(uv);
        }
    }else{
        for (unsigned int i = 0; i < vertexIndices.size(); i++){
            ftmath::vec2 uv;
            uv._x = out_vertices[i]._z;
            uv._y = out_vertices[i]._y;
            out_uvs.push_back(uv);
        }
    }
    #ifdef DEBUG
    std::cout << "ok 6"<< std::endl;
    #endif
    if (objFormat.normal){
        for (unsigned int i = 0; i < normalIndices.size(); i++){
            unsigned int normalIndex = normalIndices[i];
            ftmath::vec3 normal = temp_normals[normalIndex - 1];
            out_normals.push_back(normal);
        }
    }else{
        for (unsigned int i = 0; i < vertexIndices.size(); i++){
            out_normals.push_back(ftmath::vec3(0.0f, 0.0f, 0.0f));
        }
    }
#ifdef DEBUG
    std::cout << "ok 7"<< std::endl;
    std::cout << "\n\nLoad end. Total Triangle: " << out_vertices.size() << " Total Vertices\n";
    for (int i = 0; i < out_vertices.size(); i++){
        std::cout << "v " << out_vertices[i] << "\tvt " << out_uvs[i] << "\tvn "<< out_normals[i] << std::endl;
    }
    std::cout << "ok 8"<< std::endl;
#endif
    file.close();
    return true;
}

float* ftloader::OBJTOOPENGLVERTICES(
    unsigned int &out_size,
    const std::vector<ftmath::vec3> &vertices,
    const std::vector<ftmath::vec2> &uvs,
    const std::vector<ftmath::vec3> &normals,
    const ftmath::vec3 &color
){
    if (vertices.size() < 1 || uvs.size() < 1 || normals.size() < 1){
        return (float*)0;
    }
    const int stride = 8;
    out_size = vertices.size() * (sizeof(float) * stride);
    float *final_vertices = new float[out_size];
    int pos = 0;
    float colorEffect = 1.0f;
    for (int line = 0; line < out_size; line+=stride)
    {
        if (line % stride == 0) colorEffect += 0.1f;
        for(int offset = 0; offset < stride; offset++){
                switch (offset){
                    // vertices
                    // --------
                    case 0:
                        final_vertices[line + offset] = vertices[pos]._x;
                        break;
                    case 1:
                        final_vertices[line + offset] = vertices[pos]._y;
                        break;
                    case 2:
                        final_vertices[line + offset] = vertices[pos]._z;
                        break;

                    // texture
                    // -------
                    case 3:
                        final_vertices[line + offset] = uvs[pos]._x;
                        break;
                    case 4:
                        final_vertices[line + offset] = uvs[pos]._y;
                        break;

                    // color
                    // -----
                    case 5:
                        final_vertices[line + offset] = color._x + 1 / colorEffect;
                        break;
                    case 6:
                        final_vertices[line + offset] = color._y + 1 / colorEffect;
                        break;
                    case 7:
                        final_vertices[line + offset] = color._z + 1 / colorEffect;
                        break;
                }
        }
        pos++;
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