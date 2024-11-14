#include "camera.h"


//public
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), movement_speed(SPEED),
    mouse_sensitivity(SENSITIVITY)
{
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix(){
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProccessKeyboard(Camera_Movement direction, float deltatime){
    float velocity = movement_speed * deltatime;

    if(direction == FORWARD)
        Position += Front * velocity;
    if(direction == BACKWARD)
        Position -= Front * velocity;
    if(direction == LEFT)
        Position -= Right * velocity;
    if(direction == RIGHT)
        Position += Right * velocity;

    Position.y = 0.0f;
    
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, 
        bool constrainPitch){

    xoffset *= mouse_sensitivity;
    yoffset *= mouse_sensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if(constrainPitch){
        if(Pitch > 89.0f)
            Pitch = 89.0f;
        if(Pitch < -89.0f)
            Pitch = -89.0f;
    }

    updateCameraVectors();

}





//private
void Camera::updateCameraVectors(){
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);

    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up    = glm::normalize(glm::cross(Right, Front));
}

