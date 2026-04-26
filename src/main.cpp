#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"

// unit cube centered at origin — 8 vertices, 6 faces, 12 triangles
static const std::vector<float> cubeVertices = {
    // x      y      z
    -0.5f, -0.5f,  0.5f,  // 0 front-bottom-left
     0.5f, -0.5f,  0.5f,  // 1 front-bottom-right
     0.5f,  0.5f,  0.5f,  // 2 front-top-right
    -0.5f,  0.5f,  0.5f,  // 3 front-top-left
    -0.5f, -0.5f, -0.5f,  // 4 back-bottom-left
     0.5f, -0.5f, -0.5f,  // 5 back-bottom-right
     0.5f,  0.5f, -0.5f,  // 6 back-top-right
    -0.5f,  0.5f, -0.5f,  // 7 back-top-left
};

static const std::vector<unsigned int> cubeIndices = {
    0, 1, 2,  2, 3, 0,  // front
    5, 4, 7,  7, 6, 5,  // back
    4, 0, 3,  3, 7, 4,  // left
    1, 5, 6,  6, 2, 1,  // right
    3, 2, 6,  6, 7, 3,  // top
    4, 5, 1,  1, 0, 4,  // bottom
};

int main() {
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

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to init GLAD\n";
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader shader("shaders/chunk.vert", "shaders/chunk.frag");
    Mesh cube(cubeVertices, cubeIndices);

    Camera camera(glm::vec3(2.0f, 2.0f, 5.0f), 45.0f, 800.0f / 600.0f, 0.1f, 100.0f);

    // lock and hide cursor for FPS-style mouse look
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // store camera pointer so the mouse callback can reach it
    glfwSetWindowUserPointer(window, &camera);

    // track last mouse position to compute delta each frame
    double lastMouseX = 400.0, lastMouseY = 300.0;
    bool firstMouse = true;

    glfwSetCursorPosCallback(window, [](GLFWwindow* win, double x, double y) {
        // static locals persist across calls — store last position here
        static double lastX = 400.0, lastY = 300.0;
        static bool first = true;

        if (first) {
            lastX = x;
            lastY = y;
            first = false;
            return;
        }

        float dx = (float)(x - lastX);
        float dy = (float)(y - lastY);
        lastX = x;
        lastY = y;

        Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(win));
        cam->rotate(dx, dy);
    });

    float lastTime = (float)glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        // delta time keeps movement speed frame-rate independent
        float now = (float)glfwGetTime();
        float dt = now - lastTime;
        lastTime = now;
        const float speed = 5.0f * dt;

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // WASD = horizontal, QE = down/up
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.moveForward(speed);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.moveForward(-speed);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.moveRight(speed);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.moveRight(-speed);
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) camera.moveUp(speed);
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) camera.moveUp(-speed);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model(1.0f); // cube sits still in world space now that you can fly around it

        shader.use();
        shader.setMat4("model", model);
        shader.setMat4("view", camera.viewMatrix());
        shader.setMat4("projection", camera.projectionMatrix());

        cube.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
