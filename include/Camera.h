#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    // --- Position & orientation in world space ---
    glm::vec3 position;

    // Yaw   = rotation around Y axis (left/right), starts facing -Z
    // Pitch = rotation around X axis (up/down), clamped so you can't flip over
    float yaw;
    float pitch;

    // --- Projection parameters ---
    float fov;    // vertical field of view in degrees
    float aspect; // viewport width / height
    float near;
    float far;

    Camera(glm::vec3 position, float fov, float aspect, float near, float far)
        : position(position)
        , yaw(-90.0f)   // -90 so forward starts at -Z (OpenGL convention)
        , pitch(0.0f)
        , fov(fov)
        , aspect(aspect)
        , near(near)
        , far(far)
    {
        updateVectors();
    }

    // --- View matrix ---
    // Transforms world space into camera/view space.
    // Equivalent to placing the camera at the origin and rotating the entire world
    // so the camera looks down -Z.
    //
    // Math: lookAt builds a rotation matrix from three axes (right, up, forward),
    // then multiplies by a translation that moves the world by -position.
    //
    //   right   = normalize(front x worldUp)  -- cross product gives perpendicular axis
    //   up      = normalize(right x front)    -- recomputed so it's orthogonal
    //   forward = -front                      -- negated because camera looks down -Z
    //
    //   view = rotation * translation
    //        = | right.x   right.y   right.z   -dot(right, pos) |
    //          | up.x      up.y      up.z      -dot(up, pos)    |
    //          | -front.x  -front.y  -front.z   dot(front, pos) |
    //          | 0         0         0          1               |
    glm::mat4 viewMatrix() const {
        return glm::lookAt(position, position + front, up);
    }

    // --- Projection matrix ---
    // Warps the view frustum (pyramid shape) into the NDC unit cube [-1,1]^3.
    // After this transform, OpenGL divides by W (perspective divide) to get NDC.
    //
    // X and Y: scale by f = 1/tan(fov/2), divide by Z → things far away appear smaller
    //   x' = (x * f/aspect) / z
    //   y' = (y * f) / z
    //
    // Z: remap [near, far] to [-1, +1] using:
    //   A = -(far + near) / (far - near)
    //   B = -2 * far * near / (far - near)
    //   z' = (A*z + B) / z     (after perspective divide)
    //
    // The -1 in the last row sets W = -z_view, which triggers the perspective divide.
    glm::mat4 projectionMatrix() const {
        return glm::perspective(glm::radians(fov), aspect, near, far);
    }

    // --- Mouse look ---
    // Converts mouse delta (pixels moved) into yaw/pitch angle changes.
    // Pitch is clamped to [-89, 89] so the camera never flips upside down
    // (at exactly +-90 the up vector becomes ambiguous).
    void rotate(float deltaX, float deltaY, float sensitivity = 0.1f) {
        yaw   += deltaX * sensitivity;
        pitch -= deltaY * sensitivity; // subtract: moving mouse up = positive pitch
        if (pitch >  89.0f) pitch =  89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
        updateVectors();
    }

    // --- Movement ---
    // Move along the camera's own axes so WASD always feels relative to where you look.
    // front and right are already normalized so speed is consistent regardless of direction.
    void moveForward(float amount) { position += front * amount; }
    void moveRight(float amount)   { position += right * amount; }
    void moveUp(float amount)      { position += glm::vec3(0, 1, 0) * amount; }

private:
    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 up;

    // Recompute front/right/up from yaw and pitch.
    //
    // Yaw and pitch are Euler angles. We convert them to a direction vector:
    //   front.x = cos(pitch) * cos(yaw)   -- horizontal component scaled by vertical tilt
    //   front.y = sin(pitch)               -- vertical component
    //   front.z = cos(pitch) * sin(yaw)   -- horizontal component scaled by vertical tilt
    //
    // Then right and up are derived via cross products to form an orthonormal basis:
    //   right = normalize(front x worldUp)  -- perpendicular to both front and world up
    //   up    = normalize(right x front)    -- perpendicular to both, gives true camera up
    void updateVectors() {
        float p = glm::radians(pitch);
        float y = glm::radians(yaw);

        front = glm::normalize(glm::vec3(
            glm::cos(p) * glm::cos(y),
            glm::sin(p),
            glm::cos(p) * glm::sin(y)
        ));

        const glm::vec3 worldUp(0.0f, 1.0f, 0.0f);
        right = glm::normalize(glm::cross(front, worldUp));
        up    = glm::normalize(glm::cross(right, front));
    }
};
