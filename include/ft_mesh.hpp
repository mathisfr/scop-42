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
        Mesh();
        Mesh(const char *path_obj, unsigned int textureId, Shader ourShader, const ftmath::vec3 color[4]);
        ~Mesh();

        void Draw();
        ftmath::vec3 getCenter();
        const BoundBox getBoundBox() const;
        void clean();
        Mesh& operator=(const Mesh &other);
        ftmath::m4x4 _modelMatrix;
    private:
        Vertices _vertices;
        BoundBox _boundBox;

        unsigned int _texture;
        Shader _shader;

        ftmath::vec3 _color[4];

        float *_verticesbuffer;
        unsigned int _verticesbuffersize;

        unsigned int _VAO, _VBO;
    
        void setupMesh();
        void loadTexture();
};
#endif