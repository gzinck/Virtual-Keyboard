/**
 Light.cpp
 Virtual Keyboard
 Implementation of Light.hpp
 
 @author Graeme Zinck
 @version 1.0 4/1/2018
 */

#include "Light.hpp"

/**
 Creates a light with the specified properties.
 
 @param position The position of the light.
 @param intensities The intensities of the RGB components of the light.
 @param ambientCoefficient The ambient coefficient of the light.
 @param attenuationFactor The factor by which the light diminishes
 with distance.
 */
Light::Light(glm::vec3 position, glm::vec3 intensities, float ambientCoefficient, glm::vec3 attenuationFactor)
{
    m_position = position;
    m_intensities = intensities;
    m_ambientCoefficient = ambientCoefficient;
    m_attenuationFactor = attenuationFactor;
} // Light::Light(glm::vec3, glm::vec3, float, glm::vec3)
