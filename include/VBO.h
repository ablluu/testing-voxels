#pragma once
#include <glad/glad.h>
#include <vector>

class VBO {
public:
    unsigned int id;

    VBO() {
        glGenBuffers(1, &id);
    }

    ~VBO() {
        glDeleteBuffers(1, &id);
    }

    void bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, id);
    }

    void unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    // upload data to GPU
    template<typename T>
    void upload(const std::vector<T>& data, GLenum usage = GL_STATIC_DRAW) {
        bind();
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T), data.data(), usage);
    }

    // non copyable
    VBO(const VBO&) = delete;
    VBO& operator=(const VBO&) = delete;
};
