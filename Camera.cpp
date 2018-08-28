/**
 Camera.cpp
 Virtual Keyboard
 Implementation of Camera.hpp.
 
 @author Graeme Zinck
 @version 1.0 3/30/2018
 */

#include "Camera.hpp"

//--------------------------------------------------------------------------
/**
 Creates a camera object. It initializes with the intuitive
 up direction in the positive y-direction, and the rotations
 around x and y axises to be 0 (it's looking in the negative
 z direction).
 
 @param pos Position of the camera.
 @param fov The field of view of the camera.
 @param aspect The aspect ratio for the camera.
 @param zNear The near plane of the frustum.
 @param zFar The far plane of the frustum.
 */
Camera::Camera(const glm::vec3& pos, float fov, float aspect, float zNear, float zFar)
{
    m_fov = fov;
    m_zNear = zNear;
    m_zFar = zFar;
    updateAspectRatio(aspect); // Creates the perspective projection matrix
    
    m_position = pos;
    m_up = glm::vec3(0,1,0); // Default
    
    // Turn the camera in right direction
    m_xRot = 0.0;
    m_yRot = 0.0;
    turnXY(0, 0);
} // Camera::Camera(const glm::vec3&, float, float, float, float)

//--------------------------------------------------------------------------
/**
 Updates the projection matrix when the aspect ratio changes.
 
 @param aspect The aspect ratio for the camera.
 */
void Camera::updateAspectRatio(float aspect)
{
    m_perspective = glm::perspective(m_fov, aspect, m_zNear, m_zFar);
} // Camera::updateAspectRatio(float)

//--------------------------------------------------------------------------
/**
 Gets the view and projection matrix (multiplied together).
 
 @return The view-projection matrix.
 */
glm::mat4 Camera::getViewProjection() const
{
    return m_perspective * glm::lookAt(m_position, m_position + m_forward, m_up);
} // Camera::getViewProjection()

//--------------------------------------------------------------------------
/**
 Useful function for moving the camera forward by the move speed.
 */
void Camera::moveForward()
{
    float x = m_position.x + (m_forward.x * MOVE_SPEED);
    float z = m_position.z + (m_forward.z * MOVE_SPEED);
    m_position = glm::vec3(x, m_position.y, z);
} // Camera::moveForward()

//--------------------------------------------------------------------------
/**
 Useful function for moving the camera backward by the move speed.
 */
void Camera::moveBackward()
{
    float x = m_position.x - (m_forward.x * MOVE_SPEED);
    float z = m_position.z - (m_forward.z * MOVE_SPEED);
    m_position = glm::vec3(x, m_position.y, z);
} // Camera::moveBackward()

//--------------------------------------------------------------------------
/**
 Useful function for moving the camera right by the move speed.
 */
void Camera::moveRight()
{
    float x = m_position.x - (m_forward.z * MOVE_SPEED);
    float z = m_position.z + (m_forward.x * MOVE_SPEED);
    m_position = glm::vec3(x, m_position.y, z);
} // Camera::moveRight()

//--------------------------------------------------------------------------
/**
 Useful function for moving the camera left by the move speed.
 */
void Camera::moveLeft()
{
    float x = m_position.x + (m_forward.z * MOVE_SPEED);
    float z = m_position.z - (m_forward.x * MOVE_SPEED);
    m_position = glm::vec3(x, m_position.y, z);
} // Camera::moveLeft()

//--------------------------------------------------------------------------
/**
 Turns the camera in a direction based on two variables:
 the rotation in the "x" direction (which is really around
 the y-axis) and the rotation in the "y" direction (which
 is really around the x/z axises).
 
 @param x The amount to rotate in the x direction.
 @param y The amount to rotate in the y direction (will be
 cut off so it is less than pi/2 in either direction to avoid
 getting locked in at the top or botom).
 */
void Camera::turnXY(float x, float y)
{
    m_xRot += ROT_SPEED * x;
    m_yRot -= ROT_SPEED * y;
    if(m_yRot >= (M_PI / 2.0))
        m_yRot = M_PI / 2.0 - ROT_SPEED;
    if(m_yRot <= -(M_PI / 2.0))
        m_yRot = - M_PI / 2.0 + ROT_SPEED;
    float forwardX = sin(m_xRot) * cos(m_yRot);
    float forwardY = sin(m_yRot);
    float forwardZ = -cos(m_xRot) * cos(m_yRot);
    m_forward = glm::vec3(forwardX, forwardY, forwardZ);
} // void Camera::turnXY(float, float)
