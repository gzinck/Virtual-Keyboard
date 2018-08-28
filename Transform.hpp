/**
 Transform.hpp
 Virtual Keyboard
 Class for a transform object which can be moved aroud and ultimately
 converted into a matrix. This is especially useful for a model matrix.
 
 @author Graeme Zinck
 @version 1.0 3/30/2018
 */

#ifndef Transform_hpp
#define Transform_hpp

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Transform
{
public:
    Transform();
    Transform(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale);
    glm::mat4 getModel() const;
    
    /**
     Gets the position of the transformation.
     */
    inline glm::vec3& getPos() { return m_pos; }
    /**
     Gets the rotation of the transformation.
     */
    inline glm::vec3& getRot() { return m_rot; }
    /**
     Gets the scale of the transformation.
     */
    inline glm::vec3& getScale() { return m_scale; }
    
    /**
     Sets the position.
     
     @param pos The position to set it to.
     */
    inline void setPos(const glm::vec3& pos) { m_pos = pos; }
    /**
     Sets the rotation.
     
     @param rot The rotation to set it to.
     */
    inline void setRot(const glm::vec3& rot) { m_rot = rot; }
    /*
     Sets the scale.
     
     @param scale The scale to set it to.
     */
    inline void setScale(const glm::vec3& scale) { m_scale = scale; }
    
    /**
     Moves the transform right.
     
     @param offset Distance to move.
     */
    inline void moveRight(double offset) { m_pos.x = m_pos.x + offset; };
    /**
     Moves the transform left.
     
     @param offset Distance to move.
     */
    inline void moveLeft(double offset) { m_pos.x = m_pos.x - offset; };
    /**
     Moves the transform up.
     
     @param offset Distance to move.
     */
    inline void moveUp(double offset) { m_pos.y = m_pos.y + offset; };
    /**
     Moves the transform down.
     
     @param offset Distance to move.
     */
    inline void moveDown(double offset) { m_pos.y = m_pos.y - offset; };
private:
    glm::vec3 m_pos;
    glm::vec3 m_rot;
    glm::vec3 m_scale;
}; // Transform

#endif /* Transform_hpp */
