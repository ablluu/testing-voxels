#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char* vertPath, const char* fragPath) {
    // read files
    std::ifstream vertFile(vertPath);
    std::ifstream fragFile(fragPath);

    if (!vertFile.is_open()) {
        std::cerr << "Failed to open vertex shader: " << vertPath << "\n";
        return;
    }
    if (!fragFile.is_open()) {
        std::cerr << "Failed to open fragment shader: " << fragPath << "\n";
        return;
    }

    std::stringstream vertStream, fragStream;
    vertStream << vertFile.rdbuf();
    fragStream << fragFile.rdbuf();

    std::string vertStr = vertStream.str();
    std::string fragStr = fragStream.str();

    compile(vertStr.c_str(), fragStr.c_str());
}

Shader::~Shader() {
    glDeleteProgram(id);
}

void Shader::compile(const char* vertSrc, const char* fragSrc) {
    // vertex shader
    unsigned int vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &vertSrc, nullptr);
    glCompileShader(vert);
    checkErrors(vert, "VERTEX");

    // fragment shader
    unsigned int frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &fragSrc, nullptr);
    glCompileShader(frag);
    checkErrors(frag, "FRAGMENT");

    // link program
    id = glCreateProgram();
    glAttachShader(id, vert);
    glAttachShader(id, frag);
    glLinkProgram(id);
    checkErrors(id, "PROGRAM");

    // shaders compiled into program, no longer needed
    glDeleteShader(vert);
    glDeleteShader(frag);
}

void Shader::checkErrors(unsigned int shader, const std::string& type) const {
    int success;
    char log[1024];

    if (type == "PROGRAM") {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, log);
            std::cerr << "Shader link error:\n" << log << "\n";
        }
    } else {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, log);
            std::cerr << "Shader compile error (" << type << "):\n" << log << "\n";
        }
    }
}

void Shader::use() const {
    glUseProgram(id);
}

void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setVec3(const std::string& name, const glm::vec3& vec) const {
    glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(vec));
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const {
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}
