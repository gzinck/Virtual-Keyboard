/**
 Light.hpp
 Virtual Keyboard
 Class for a light object which stores its position, intensities, and properties.
 
 @author Graeme Zinck
 @version 1.0 4/1/2018
 */

#ifndef Light_hpp
#define Light_hpp

#include <glm/glm.hpp>

class Light
{
public:
    Light(glm::vec3 position, glm::vec3 intensities, float ambientCoefficient, glm::vec3 attenuationFactor);
    virtual ~Light() {};
    
    /**
     Gets the position of the light.
     */
    inline glm::vec3 getPos() { return m_position; };
    /**
     Gets the intensities of the light.
     */
    inline glm::vec3 getIntensities() { return m_intensities; };
    /**
     Gets the ambient coefficient of the light.
     */
    inline float getAmbientCoefficient() { return m_ambientCoefficient; };
    /**
     Gets the attenuation factor of the light.
     */
    inline glm::vec3 getAttenuationFactor() { return m_attenuationFactor; };
private:
    glm::vec3 m_position;
    glm::vec3 m_intensities;
    float m_ambientCoefficient;
    glm::vec3 m_attenuationFactor;
}; // Light

#endif /* Light_hpp */
