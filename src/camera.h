#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;


class Camera
{
    public:

        //camera attributes
        glm::vec3 Position;
        glm::vec3 Up;
        glm::vec3 Front;
        glm::vec3 Right;
        glm::vec3 WorldUp;

        //euler angles
        float Yaw;
        float Pitch;

        //camera options
        float movement_speed;
        float mouse_sensitivity;

        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
                glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
                float yaw = YAW, float pitch = PITCH);

        //return the view matrix calculated using euler angles and lookat matrix
        glm::mat4 GetViewMatrix();

        void ProccessKeyboard(Camera_Movement direction, float deltatime);

        void ProcessMouseMovement(float xoffset, float yoffset, 
                bool constrainPitch = true);




        
    private:

        void updateCameraVectors();
};
#endif

