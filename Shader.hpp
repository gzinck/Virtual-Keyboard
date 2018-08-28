/**
 Shader.hpp
 Virtual Keyboard
 Class for a shader program which has a collection of properties
 that are set by the camera, light, and objects for display.
 
 @author Graeme Zinck
 @version 1.0 3/29/2018
 */

#ifndef Shader_hpp
#define Shader_hpp

#define GLEW_STATIC
#include <GL/glew.h>

#include <string>
#include "Transform.hpp"
#include "Camera.hpp"
#include "Light.hpp"

class Shader
{
public:
    Shader(const std::string& fileName);
    virtual ~Shader();
    void bind(Light* light);
    void update(const Transform& transform, Camera* camera);
    void setMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float specularExponent);
    inline GLuint getShaderProgram() { return m_program; }
private:
    static const unsigned int NUM_SHADERS = 2; // Only 2 shaders: vertex and fragment shader
    
    // Enumerated type for all the uniforms sent to the GPU
    enum
    {
        MODEL_MATRIX_U,
        VIEW_MATRIX_U,
        CAMERA_POS_U,
        LIGHT_POS_U,
        LIGHT_INTENSITIES_U,
        LIGHT_AMBIENT_COEFFICIENT_U,
        LIGHT_ATTENUATION_FACTOR_A_U,
        LIGHT_ATTENUATION_FACTOR_B_U,
        LIGHT_ATTENUATION_FACTOR_C_U,
        MATERIAL_SPECULAR_EXPONENT_U,
        MATERIAL_AMBIENT_U,
        MATERIAL_DIFFUSE_U,
        MATERIAL_SPECULAR_U,
        
        NUM_UNIFORMS
    };
    
    // Stores the program, shaders, and uniforms
    GLuint m_program;
    GLuint m_shaders[NUM_SHADERS];
    GLuint m_uniforms[NUM_UNIFORMS];
}; // Shader

#endif /* Shader_hpp */
