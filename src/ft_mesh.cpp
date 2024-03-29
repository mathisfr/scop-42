#include "ft_mesh.hpp"
#include "ft_loader.hpp"
#include "ft_math.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

Mesh::Mesh(){
    _modelMatrix = ftmath::m4x4(1.0f);
    _verticesbuffer == nullptr;
};

Mesh::~Mesh(){
    if (_verticesbuffer){
        delete[] _verticesbuffer;
    }
    _verticesbuffer = nullptr;
}

Mesh::Mesh(const char *path_obj, unsigned int textureId, Shader ourShader, const ftmath::vec3 color[4]){
    _modelMatrix = ftmath::m4x4(1.0f);
    _texture = textureId;
    for (int i = 0; i < 4; i++){
        _color[i] = color[i];
    }
    _shader = ourShader;
    ftloader::OBJ(path_obj, _vertices.Position, _vertices.TexCoords, _vertices.Normal, _boundBox);
    _verticesbuffer = ftloader::OBJTOOPENGLVERTICES(_verticesbuffersize, _vertices.Position, _vertices.TexCoords, _vertices.Normal, _color);
    setupMesh();
}

Mesh& Mesh::operator=(const Mesh &other){
    _texture = other._texture;
    _shader = other._shader;
    _boundBox = other._boundBox;
    for (int i = 0; i < 4; i++){
        _color[i] = other._color[i];
    }
    _vertices = other._vertices;
    _verticesbuffer = ftloader::OBJTOOPENGLVERTICES(_verticesbuffersize, _vertices.Position, _vertices.TexCoords, _vertices.Normal, _color);
    setupMesh();
    return *this;
}

ftmath::vec3 Mesh::getCenter(){
    ftmath::vec3 centeredOffset;
    centeredOffset._x = (_boundBox.maxBoundBox._x + _boundBox.minBoundBox._x) / 2.0f;
    centeredOffset._y = (_boundBox.maxBoundBox._y + _boundBox.minBoundBox._y) / 2.0f;
    centeredOffset._z = (_boundBox.maxBoundBox._z + _boundBox.minBoundBox._z) / 2.0f;
    return centeredOffset;
}

const BoundBox Mesh::getBoundBox() const{
    return _boundBox;
}

void Mesh::clean(){
    if(_verticesbuffer)
        delete[] _verticesbuffer;
    _verticesbuffer = nullptr;
    glDeleteVertexArrays(1, &_VAO);
    glDeleteBuffers(1, &_VBO);
}

void Mesh::setupMesh(){
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);

    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);

    glBufferData(GL_ARRAY_BUFFER, _verticesbuffersize, _verticesbuffer, GL_STATIC_DRAW);

    const int strideSize = 11 * sizeof(float);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, strideSize, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, strideSize, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, strideSize, (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, strideSize, (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Mesh::Draw(){
    unsigned int modelShader = glGetUniformLocation(_shader.ID, "model");
    glUniformMatrix4fv(modelShader, 1, GL_FALSE, _modelMatrix.toglsl());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texture);

    glUniform1i(glGetUniformLocation(_shader.ID, "texture0"), 0);
    _shader.use();

    glBindVertexArray(_VAO);
    glDrawArrays(GL_TRIANGLES, 0, _vertices.Position.size());
    glBindVertexArray(0);
    _modelMatrix = ftmath::m4x4(1.0f);
}