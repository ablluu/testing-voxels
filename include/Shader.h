#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

class Shader {
public:
    unsigned int id;

    Shader(const char* vertPath, const char* fragPath);
    ~Shader();

    void use() const;

    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec3(const std::string& name, const glm::vec3& vec) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;

    // non copyable
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

private:
    void compile(const char* vertSrc, const char* fragSrc);
    void checkErrors(unsigned int shader, const std::string& type) const;
};
