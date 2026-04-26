#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

// minimal shaders inline for now
const char* vertSrc = R"(
    #version 330 core
    layout (location = 0) in ivec3 position;
    void main() {
        gl_Position = vec4(vec3(position), 1.0);
    }
)";

const char* fragSrc = R"(
    #version 330 core
    out vec4 fragColor;
    void main() {
        fragColor = vec4(1.0, 0.5, 0.2, 1.0);
    }
)";

int compileShader(const char* src, GLenum type) {
    int shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    return shader;
}

int main() {
    // init glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Voxel", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // init glad — must happen after context creation
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    // compile and link shaders
    int vert = compileShader(vertSrc, GL_VERTEX_SHADER);
    int frag = compileShader(fragSrc, GL_FRAGMENT_SHADER);
    int shader = glCreateProgram();
    glAttachShader(shader, vert);
    glAttachShader(shader, frag);
    glLinkProgram(shader);
    glDeleteShader(vert);
    glDeleteShader(frag);

    // geometry — a simple triangle for now
    std::vector<unsigned int> vertices = {
        0, 0, 0,
        1, 0, 0,
        0, 1, 0
    };

    std::vector<unsigned int> indices = {
        0, 1, 2
    };

    // upload
    VAO vao;
    VBO vbo;
    EBO ebo;

    vao.bind();
    vbo.upload(vertices);
    ebo.upload(indices);
    vao.defineLayout({{0, 3, GL_UNSIGNED_INT, false, 0}}, sizeof(unsigned int) * 3);
    vao.unbind();

    // main loop
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);
        vao.bind();
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        vao.unbind();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}
