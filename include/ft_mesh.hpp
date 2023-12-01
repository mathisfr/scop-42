#ifndef FT_MESH_HPP
#define FT_MESH_HPP
#include <vector>
#include "ft_shader.hpp"
#include "ft_math.hpp"
typedef struct Vertices {
    std::vector<ftmath::vec3> Position;
    std::vector<ftmath::vec2> TexCoords;
    std::vector<ftmath::vec3> Normal;
} Vertices;
class Mesh{
    public:
        Mesh(const char *path_obj, unsigned int textureId, Shader ourShader);
        void Draw();
        ~Mesh();
    private:
        Vertices _vertices;
        unsigned int _texture;
        Shader _shader;
        float *_verticesbuffer;
        unsigned int _verticesbuffersize;
        unsigned int _VAO, _VBO;
        void setupMesh();
        void loadTexture();
};
#endif