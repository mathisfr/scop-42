#ifndef FT_LOADER_HPP
#define FT_LOADER_HPP
#include <vector>
#include <iostream>
#include <exception>
#include "ft_math.hpp"
namespace ftloader{
        void OBJ(
            const char *path,
            std::vector<ftmath::vec3> &out_vertices,
            std::vector<ftmath::vec2> &out_uvs,
            std::vector<ftmath::vec3> &out_normals
        );
        float *OBJTOOPENGLVERTICES(
            unsigned int &out_size,
            const std::vector<ftmath::vec3> &in_vertices,
            const std::vector<ftmath::vec2> &in_uvs,
            const std::vector<ftmath::vec3> &in_normals,
            const ftmath::vec3 color[3]
        );
        unsigned char* BMP(
            const char *in_path,
            int &out_width,
            int &out_height
        );
}

class ftloader_OBJ_exception: public std::exception
{
  virtual const char* what() const throw()
  {
    return "Error to load OBJ file !";
  }
};
#endif