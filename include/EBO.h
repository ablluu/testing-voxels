#pragma once
#include <glad/glad.h>
#include <vector>

class EBO {
public:
    unsigned int id;

    EBO() {
        glGenBuffers(1, &id);
    }

    ~EBO() {
        glDeleteBuffers(1, &id);
    }

    void bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    }

    void unbind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void upload(const std::vector<unsigned int>& data, GLenum usage = GL_DYNAMIC_DRAW) {
        bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(unsigned int), data.data(), usage);
    }

    // non copyable
    EBO(const EBO&) = delete;
    EBO& operator=(const EBO&) = delete;
};
