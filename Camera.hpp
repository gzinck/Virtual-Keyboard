/**
 Camera.hpp
 Virtual Keyboard
 Class for a camera object which has useful functions for moving around
 and then finally turning the camera into a view matrix.
 
 @author Graeme Zinck
 @version 1.0 3/30/2018
 */

#ifndef Camera_hpp
#define Camera_hpp

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

class Camera
{
public:
    Camera(const glm::vec3& pos, float fov, float aspect, float zNear, float zFar);
    
    void updateAspectRatio(float aspect);
    glm::mat4 getViewProjection() const;
    void moveForward();
    void moveBackward();
    void moveRight();
    void moveLeft();
    void turnXY(float x, float y);
    
    /**
     Gets the position of the camera.
     
     @return Vec3 of the position.
     */
    inline glm::vec3 getPos() { return m_position; };
private:    
    const float MOVE_SPEED = 0.2;
    const float ROT_SPEED = 0.003;
    
    // Constants/variables for rotating camera's direction
    float m_xRot;
    float m_yRot;
    
    // For the projection matrix
    float m_fov; // field of view
    float m_zNear;
    float m_zFar;
    glm::mat4 m_perspective;
    
    glm::vec3 m_position;
    glm::vec3 m_forward;
    glm::vec3 m_up;
}; // Camera

#endif /* Camera_hpp */

