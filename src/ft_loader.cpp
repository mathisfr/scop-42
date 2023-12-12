#include "ft_loader.hpp"
#include "ft_math.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cstdint>
#define TIRANGLE_VERTICES 3

typedef struct OBJ_FORMAT{
    bool vertex;
    bool uv;
    bool normal;
    bool face;
}   OBJ_FORMAT;

//  Load OBJ 3d model
//  -----------------
void ftloader::OBJ(
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
    objFormat.vertex = objFormat.uv = objFormat.normal = objFormat.face = false;

    std::ifstream file;
    file.open(path, std::ios::in);
    if(!file.is_open()){
        throw ftloader_OBJ_exception();
    }
    std::string line;

    // convert text to data
    // --------------------
    while (getline(file, line)){
        ftmath::vec3 v;
        /*
        *   v   :   vertex
        *   vt  :   texture coordinate
        *   vn  :   normal
        *   f   :   face
        */
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
            /*
            *   face parsing
            *   multiple format possible
            *   f 1 2 3 ...
            *   f 1/1 2/2 3/3 ...
            *   f 1//1 2//2 3//3 ...
            *   f 1/1/1 2/2/2 3/3/3 ...
            *   Actually tested only with first and last possibility
            */
            objFormat.face = true;
            std::istringstream s(line.substr(2));
            std::vector<std::string> in_param;
            std::string tmp_in_param = "";
            std::string out_param;

            //  split word separate by space
            //  ----------------------------
            while(s >> tmp_in_param){
                in_param.push_back(tmp_in_param);
                #ifdef DEBUG
                std::cout << tmp_in_param << '\n';
                #endif
            }
            int params_size = in_param.size();
            if (params_size < 3 || params_size > 4){
                file.close();
                throw ftloader_OBJ_exception();
            }
            std::stringstream ss_param[TIRANGLE_VERTICES];
            for(unsigned int i = 0; i < TIRANGLE_VERTICES; i++){
                ss_param[i].str(in_param[i]);
            }
            //  for all params, split by '/' and store values
            //  ---------------------------------------------
            for (unsigned int i = 0; i < TIRANGLE_VERTICES; i++){
                unsigned int index = 0;
                while (getline(ss_param[i], out_param, '/')
                    || ((!objFormat.vertex && !objFormat.uv) && getline(ss_param[i], out_param, '\n'))){
                    switch(index)
                    {
                        case 0:
                            vertexIndices.push_back(std::stoul(out_param));
                            break;
                        case 1:
                            uvIndices.push_back(std::stoul(out_param));
                            break;
                        case 2:
                            normalIndices.push_back(std::stoul(out_param));
                            break;
                    }
                    index++;
                }
            }
            //  case where we have 4 param in the same line. (convert quad to triangle)
            //  ---------------------------------------------------------------------
            if (params_size == 4){
                std::stringstream ss_param_convert[TIRANGLE_VERTICES];
                for(unsigned int i = 0; i < TIRANGLE_VERTICES; i++){
                    ss_param_convert[i].str(in_param[(i+2)%4]);
                }
                for (unsigned int i = 0; i < TIRANGLE_VERTICES; i++){
                unsigned int index = 0;
                while (getline(ss_param_convert[i], out_param, '/')
                    || ((!objFormat.vertex && !objFormat.uv) && getline(ss_param_convert[i], out_param, '\n'))){
                    switch(index)
                    {
                        case 0:
                            vertexIndices.push_back(std::stoul(out_param));
                            break;
                        case 1:
                            uvIndices.push_back(std::stoul(out_param));
                            break;
                        case 2:
                            normalIndices.push_back(std::stoul(out_param));
                            break;
                    }
                    index++;
                }
            }
            }
        }
    }
    // check if vertex and face is present
    // -----------------------------
    if (!objFormat.vertex){
        file.close();
        throw ftloader_OBJ_exception();
    }
    if (!objFormat.face){
        file.close();
        throw ftloader_OBJ_exception();
    }
    //  fill out vertices array
    //  -----------------------
    for (unsigned int i = 0; i < vertexIndices.size(); i++){
        unsigned int vertexIndex = vertexIndices[i];
        ftmath::vec3 vertex = temp_vertices[vertexIndex - 1];
        out_vertices.push_back(vertex);
    }
    //  fill out uv array
    //  -----------------------
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
    //  fill out uv normal
    //  -----------------------
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
    std::cout << "\n\nLoad end. Total Triangle: " << out_vertices.size() << " Total Vertices\n";
    for (int i = 0; i < out_vertices.size(); i++){
        std::cout << "v " << out_vertices[i] << "\tvt " << out_uvs[i] << "\tvn "<< out_normals[i] << std::endl;
    }
#endif
    file.close();
}

//  Convert Obj result loader to readable array buffer for opengl
//  -------------------------------------------------------------
float* ftloader::OBJTOOPENGLVERTICES(
    unsigned int &out_size,
    const std::vector<ftmath::vec3> &vertices,
    const std::vector<ftmath::vec2> &uvs,
    const std::vector<ftmath::vec3> &normals,
    const ftmath::vec3 color[3]
){
    if (vertices.size() < 1 || uvs.size() < 1 || normals.size() < 1){
        throw ftloader_OBJ_exception();
        return nullptr;
    }
    int colorIndex = 0;
    const int stride = 8;
    out_size = vertices.size() * (sizeof(float) * stride);
    float *final_vertices = new float[out_size];
    int pos = 0;
    int loop_max = out_size / 4;
    for (int line = 0; line < loop_max; line+=stride)
    {
        //  each face we color face with another color
        //  ------------------------------------------
        if(pos % 3 == 0){
            colorIndex = (colorIndex + 1) % 3;
        }
        
        //  store value to correct position in vertex buffer
        //  ------------------------------------------------
        for(int offset = 0; offset < stride; offset++){
                #ifdef DEBUG
                std::cout << "line + offset = " << line + offset << "\nout_size = " << out_size << std::endl;
                std::cout << "pos = " << pos << std::endl;
                std::cout << "vertices.size() = " << vertices.size() << std::endl;
                std::cout << "uvs.size() = " << vertices.size() << std::endl;
                std::cout << "color.size() = " << vertices.size() << std::endl;
                #endif
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
                        final_vertices[line + offset] = color[colorIndex]._x;
                        break;
                    case 6:
                        final_vertices[line + offset] = color[colorIndex]._y;
                        break;
                    case 7:
                        final_vertices[line + offset] = color[colorIndex]._z;
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

    std::ifstream file(path, std::ios::in | std::ios::binary);
    if (!file){
        std::cerr << "Failed to open BMP texture\n";
        return nullptr;
    }
    file.read(&header[0], 54);
    if(file.gcount() != 54){
        std::cerr << "Not a correct BMP file\n";
        file.close();
        return nullptr;
    }
    if ( header[0]!='B' || header[1]!='M' ){
        std::cerr << "Not a correct BMP file\n";
        file.close();
        return nullptr;
    }
    dataPos = *(int*)&(header[0x0A]);
    imageSize = *(int*)&(header[0x22]);
    width = *(int*)&(header[0x12]);
    height = *(int*)&(header[0x16]);
    #ifdef DEBUG
    std::cout << "dataPos: " << dataPos << std::endl;
    std::cout << "imageSize: " << imageSize << std::endl;
    std::cout << "datawidthPos: " << width << std::endl;
    std::cout << "height: " << height << std::endl;
    #endif
    if (dataPos != 138){
        std::cerr << "Not a correct BMP file\n";
        file.close();
        return nullptr;
    }
    if (imageSize == 0) imageSize = width * height * 3;
    unsigned char* data = new unsigned char[imageSize];
    int i = 0;
    file.read(reinterpret_cast<char*>(data), imageSize);
    file.close();
    return (data);
}