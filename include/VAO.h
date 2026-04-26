#pragma once
#include <glad/glad.h>
#include <vector>

struct VertexAttrib {
    unsigned int index;      // location in shader
    int size;                // number of components (2, 3, 4)
    GLenum type;             // GL_FLOAT, GL_UNSIGNED_INT etc.
    bool normalized;         // rarely true
    size_t offset;           // byte offset within vertex
};

class VAO {
public:
    unsigned int id;

    VAO() {
        glGenVertexArrays(1, &id);
    }

    ~VAO() {
        glDeleteVertexArrays(1, &id);
    }

    void bind() const {
        glBindVertexArray(id);
    }

    void unbind() const {
        glBindVertexArray(0);
    }

    // Call this after binding VAO and VBO
    // stride = total byte size of one vertex
    void defineLayout(const std::vector<VertexAttrib>& attribs, size_t stride) {
        for (const auto& a : attribs) {
            if (a.type == GL_FLOAT || a.type == GL_DOUBLE) {
                // floating point types
                glVertexAttribPointer(
                    a.index,
                    a.size,
                    a.type,
                    a.normalized ? GL_TRUE : GL_FALSE,
                    stride,
                    (void*)a.offset
                );
            } else {
                // integer types (GL_INT, GL_UNSIGNED_INT, GL_BYTE etc.)
                glVertexAttribIPointer(
                    a.index,
                    a.size,
                    a.type,
                    stride,
                    (void*)a.offset
                );
            }
            glEnableVertexAttribArray(a.index);
        }
    }

    // non copyable
    VAO(const VAO&) = delete;
    VAO& operator=(const VAO&) = delete;
};
