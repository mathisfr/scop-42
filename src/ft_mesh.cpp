#include "ft_mesh.hpp"
#include "ft_loader.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

Mesh::Mesh(const char *path_obj, unsigned int textureId, Shader ourShader){
    _texture = textureId;
    _shader = ourShader;
    bool res = ftloader::OBJ(path_obj, _vertices.Position, _vertices.TexCoords, _vertices.Normal);
    _verticesbuffer = ftloader::OBJTOOPENGLVERTICES(_verticesbuffersize, _vertices.Position, _vertices.TexCoords, _vertices.Normal);
    if (_verticesbuffer){
        setupMesh();
    }
    delete(_verticesbuffer);
}

Mesh::~Mesh(){
    glDeleteVertexArrays(1, &_VAO);
    glDeleteBuffers(1, &_VBO);
}

void Mesh::setupMesh(){
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);

    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);

    glBufferData(GL_ARRAY_BUFFER, _verticesbuffersize, _verticesbuffer, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Mesh::Draw(){
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texture);

    glUniform1i(glGetUniformLocation(_shader.ID, "texture0"), 0);
    _shader.use();

    glBindVertexArray(_VAO);
    glDrawArrays(GL_TRIANGLES, 0, _vertices.Position.size());
    glBindVertexArray(0);
}