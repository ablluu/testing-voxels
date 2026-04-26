#pragma once
#include <glad/glad.h>
#include <vector>
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

class Mesh {
public:
    unsigned int indexCount;

    // Takes flat float vertex data (x, y, z per vertex) and triangle indices.
    // Immediately uploads to GPU — after construction the CPU vectors are not needed.
    Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices)
        : indexCount(indices.size())
    {
        vao.bind();
        vbo.upload(vertices);
        ebo.upload(indices);
        // layout: location 0, 3 floats, tightly packed
        vao.defineLayout({{0, 3, GL_FLOAT, false, 0}}, sizeof(float) * 3);
        vao.unbind();
    }

    void draw() const {
        vao.bind();
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
        vao.unbind();
    }

    // non copyable — VAO/VBO/EBO own GPU resources
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

private:
    VAO vao;
    VBO vbo;
    EBO ebo;
};
