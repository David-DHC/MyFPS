//
// Created by anduin on 23/01/2022.
//

#ifndef MYFPS_CAMERA_H
#define MYFPS_CAMERA_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 FirstPosition;
    glm::vec3 Front;
    glm::vec3 FirstFront;
    glm::vec3 PrevFront;
    glm::vec3 Up;
    glm::vec3 FirstUp;
    glm::vec3 PrevUp;
    glm::vec3 Right;
    glm::vec3 FirstRight;
    glm::vec3 PrevRight;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float FirstYaw;
    float PrevYaw;
    float Pitch;
    float FirstPitch;
    float PrevPitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    bool MouseTriggered;

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        MouseTriggered = true;
        updateCameraVectors();
        storePrevCameraVectors();
        storeFirstCameraVectors();
    }
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        MouseTriggered = true;
        updateCameraVectors();
        storePrevCameraVectors();
        storeFirstCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
        if (direction == UP)
            Position += WorldUp * velocity;
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, int type, GLboolean constrainPitch = true)
    {
        if (MouseTriggered == false)
            return;

        if (type == 3)
        {
            loadPrevCameraVectors();
            return;
        }

        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();


        if (type == 1)
            storePrevCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }

    void loadPrevCameraVectors()
    {
        Yaw = PrevYaw;
        Pitch = PrevPitch;
        Front = PrevFront;
        Up = PrevUp;
        Right = PrevRight;
    }

    void loadFirstCameraVectors()
    {
        Yaw = FirstYaw;
        Pitch = FirstPitch;
        Front = FirstFront;
        Up = FirstUp;
        Right = FirstRight;
        Position = FirstPosition;
        storePrevCameraVectors();
    }

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up    = glm::normalize(glm::cross(Right, Front));
    }

    void storePrevCameraVectors()
    {
        PrevYaw = Yaw;
        PrevPitch = Pitch;
        PrevFront = Front;
        PrevUp = Up;
        PrevRight = Right;
    }

    void storeFirstCameraVectors()
    {
        FirstYaw = Yaw;
        FirstPitch = Pitch;
        FirstFront = Front;
        FirstUp = Up;
        FirstRight = Right;
        FirstPosition = Position;
    }
};
#endif //MYFPS_CAMERA_H