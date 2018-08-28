/**
 Transform.cpp
 Virtual Keyboard
 Implementation of Transform.hpp.
 
 @author Graeme Zinck
 @version 1.0 3/30/2018
 */

#include "Transform.hpp"

//--------------------------------------------------------------------------
/**
 Creates a transform with position at (0, 0, 0),
 no rotation, and default 1.0 scaling.
 */
Transform::Transform()
{
    m_pos = glm::vec3();
    m_rot = glm::vec3();
    m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
} // Transform::Transform()

//--------------------------------------------------------------------------
/**
 Creates a transform with the specified properties.
 
 @param pos The position for the transform.
 @param rot The rotation of the transform.
 @param scale The scale of the transform.
 */
Transform::Transform(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale)
{
    m_pos = pos;
    m_rot = rot;
    m_scale = scale;
} // Transform::Transform(const glm::vec3&, const glm::vec3&, const glm::vec3&)

//--------------------------------------------------------------------------
/**
 Gets a matrix for the transform based on the pos, rot,
 and scale.
 
 @return The model matrix for the transform.
 */
glm::mat4 Transform::getModel() const
{
    glm::mat4 posMatrix = glm::translate(m_pos);
    
    // Rotation
    glm::mat4 rotXMatrix = glm::rotate(m_rot.x, glm::vec3(1,0,0));
    glm::mat4 rotYMatrix = glm::rotate(m_rot.y, glm::vec3(0,1,0));
    glm::mat4 rotZMatrix = glm::rotate(m_rot.z, glm::vec3(0,0,1));
    glm::mat4 rotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;
    
    glm::mat4 scaleMatrix = glm::scale(m_scale);
    
    return posMatrix * rotMatrix * scaleMatrix;
} // Transform::getModel()
