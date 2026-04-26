#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Shader.h"

int main() {
    // init glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Voxel", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // init glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to init GLAD\n";
        return -1;
    }

    // shaders
    Shader shader("shaders/chunk.vert", "shaders/chunk.frag");

    // geometry
    std::vector<unsigned int> vertices = {
        0, 0, 0,
        1, 0, 0,
        0, 1, 0
    };

    std::vector<unsigned int> indices = {
        0, 1, 2
    };

    // upload to gpu
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

        shader.use();
        vao.bind();
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        vao.unbind();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
