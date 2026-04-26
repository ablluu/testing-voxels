#version 330 core
layout (location = 0) in ivec3 position;

void main() {
    gl_Position = vec4(vec3(position), 1.0);
}
