/**
 Shader.cpp
 Virtual Keyboard
 Implementation of Shader.hpp.
 
 @author Graeme Zinck
 @version 1.0 3/29/2018
 */

#include "Shader.hpp"
#include <iostream>
#include <fstream>

// Helper methods (these are at the end of the file)
static void checkShaderError(GLuint shader, GLuint flag, bool isAProgram);
static std::string loadShader(const std::string& fileName);
static GLuint createShader(const std::string& text, GLenum shaderType);

//--------------------------------------------------------------------------
/**
 Create a shader program from a shader file, composed
 of one vertex shader and one fragment shader. It compiles
 the program, and then it gets the uniforms which will be
 used to set parameters in the shaders.
 
 @param fileName The full path of the shaders WITHOUT
 their extension (the extensions ".vs" and ".fs" will
 be added automatically).
 */
Shader::Shader(const std::string& fileName)
{    
    m_program = glCreateProgram(); // Returns the location of the program
    
    std::string vertexShaderText = loadShader(fileName + ".vs");
    std::string fragmentShaderText = loadShader(fileName + ".fs");
    
    m_shaders[0] = createShader(vertexShaderText, GL_VERTEX_SHADER);
    m_shaders[1] = createShader(fragmentShaderText, GL_FRAGMENT_SHADER);
    
    for(unsigned int i = 0; i < NUM_SHADERS; i++)
        glAttachShader(m_program, m_shaders[i]);
    
    glLinkProgram(m_program);
    checkShaderError(m_program, GL_LINK_STATUS, true);
    
    // Used for bugfixing
    // glValidateProgram(m_program);
    // checkShaderError(m_program, GL_VALIDATE_STATUS, true);
    
    // Specify the matrices and where the camera is
    m_uniforms[MODEL_MATRIX_U] = glGetUniformLocation(m_program, "modelMatrix"); // Get the uniform from the glsl shader program
    m_uniforms[VIEW_MATRIX_U] = glGetUniformLocation(m_program, "viewMatrix");
    m_uniforms[CAMERA_POS_U] = glGetUniformLocation(m_program, "cameraPosition");
    
    // Specify characteristics of the light
    m_uniforms[LIGHT_POS_U] = glGetUniformLocation(m_program, "light.position");
    m_uniforms[LIGHT_INTENSITIES_U] = glGetUniformLocation(m_program, "light.intensities");
    m_uniforms[LIGHT_AMBIENT_COEFFICIENT_U] = glGetUniformLocation(m_program, "light.ambientCoefficient");
    m_uniforms[LIGHT_ATTENUATION_FACTOR_A_U] = glGetUniformLocation(m_program, "light.attenuationFactorA");
    m_uniforms[LIGHT_ATTENUATION_FACTOR_B_U] = glGetUniformLocation(m_program, "light.attenuationFactorB");
    m_uniforms[LIGHT_ATTENUATION_FACTOR_C_U] = glGetUniformLocation(m_program, "light.attenuationFactorC");
    
    // Specify characteristics of the material
    m_uniforms[MATERIAL_SPECULAR_EXPONENT_U] = glGetUniformLocation(m_program, "material.specularExponent");
    m_uniforms[MATERIAL_AMBIENT_U] = glGetUniformLocation(m_program, "material.ambient");
    m_uniforms[MATERIAL_DIFFUSE_U] = glGetUniformLocation(m_program, "material.diffuse");
    m_uniforms[MATERIAL_SPECULAR_U] = glGetUniformLocation(m_program, "material.specular");
} // Shader()

//--------------------------------------------------------------------------
/**
 Destroys the shader program and its attached
 shaders.
 */
Shader::~Shader()
{
    for(unsigned int i = 0; i < NUM_SHADERS; i++)
    {
        glDetachShader(m_program, m_shaders[i]);
        glDeleteShader(m_shaders[i]);
    } // for
    glDeleteProgram(m_program);
} // ~Shader()

//--------------------------------------------------------------------------
/**
 Binds the program. It also sets up the initial
 lighting settings using a light pointer passed
 as an argument.
 
 @param light A light pointer which is used to set up
 the initial light properties sent to the shader.
 */
void Shader::bind(Light* light)
{
    glUseProgram(m_program);
    
    // Set the initial light settings of the shader program
    glUniform3fv(m_uniforms[LIGHT_POS_U], 1, &light->getPos()[0]);
    glUniform3fv(m_uniforms[LIGHT_INTENSITIES_U], 1, &light->getIntensities()[0]);
    glUniform1f(m_uniforms[LIGHT_AMBIENT_COEFFICIENT_U], light->getAmbientCoefficient());
    
    // Set the attenuation
    glm::vec3 attenuationFactor = light->getAttenuationFactor();
    glUniform1f(m_uniforms[LIGHT_ATTENUATION_FACTOR_A_U], attenuationFactor.x);
    glUniform1f(m_uniforms[LIGHT_ATTENUATION_FACTOR_B_U], attenuationFactor.y);
    glUniform1f(m_uniforms[LIGHT_ATTENUATION_FACTOR_C_U], attenuationFactor.z);
} // bind()

//--------------------------------------------------------------------------
/**
 Updates the model and view matrixes and the camera
 position.
 
 @param transform The model matrix transformation object.
 @param camera The camera object used for positioning in the
 world.
 */
void Shader::update(const Transform& transform, Camera* camera)
{
    glm::mat4 model = transform.getModel();
    glm::mat4 view = camera->getViewProjection();
    glUniformMatrix4fv(m_uniforms[MODEL_MATRIX_U], 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(m_uniforms[VIEW_MATRIX_U], 1, GL_FALSE, &view[0][0]);
    glUniform3fv(m_uniforms[CAMERA_POS_U], 1, &camera->getPos()[0]);
} // update(const Transform&, const Camera&)

//--------------------------------------------------------------------------
/**
 Sets the material properties of objects drawn using the
 shader program.
 
 @param ambient The R, G, and B colour component
 coefficients for how much ambient light to reflect.
 @param diffuse The R, G, and B colour component
 coefficients for how much diffuse light to reflect.
 @param specular The R, G, and B colour component
 coefficients for how much specular light to reflect.
 @param specularExponent The shininess of the surface.
 */
void Shader::setMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float specularExponent)
{
    // Specify characteristics of the material
    glUniform3fv(m_uniforms[MATERIAL_AMBIENT_U], 1, &ambient[0]);
    glUniform3fv(m_uniforms[MATERIAL_DIFFUSE_U], 1, &diffuse[0]);
    glUniform3fv(m_uniforms[MATERIAL_SPECULAR_U], 1, &specular[0]);
    glUniform1f(m_uniforms[MATERIAL_SPECULAR_EXPONENT_U], specularExponent);
} // Shader::setMaterial(float)

//--------------------------------------------------------------------------
/**
 Creates a single shader from a text string (which was read
 from the original shader file). It also compiles the shader.
 
 @param text The shader program in string form.
 @shaderType The GLenum for the type of shader to create.
 @return The OpenGL reference to the shader.
 */
static GLuint createShader(const std::string& text, GLenum shaderType)
{
    // Make a shader
    GLuint newShader = glCreateShader(shaderType);
    
    // Assign it a source
    const GLchar* textCStr = text.c_str();
    const GLint length = (GLint)text.length();
    glShaderSource(newShader, 1, &textCStr, &length);
    
    // Compile it
    glCompileShader(newShader);
    
    // Check if compiled
    checkShaderError(newShader, GL_COMPILE_STATUS, false);
    
    // Return the shader reference
    return newShader;
} // createShader(const std::string&, GLenum);

//--------------------------------------------------------------------------
/**
 Loads a shader from a path and returns the full text
 of the shader file.
 
 @param fileName The full path of the shader file.
 @return The full text of the shader file.
 */
static std::string loadShader(const std::string& fileName)
{
    std::ifstream file;
    file.open((fileName).c_str());
    
    std::string output;
    std::string line;
    
    if(file.is_open())
    {
        while(file.good())
        {
            getline(file, line);
            output.append(line + "\n");
        } // while
    } // if
    else
    {
        std::cerr << "Unable to load shader: " << fileName << std::endl;
    } // else
    return output;
} // loadShader(const std::string&)

//--------------------------------------------------------------------------
/**
 Bugfixing method to examine if there were issues compiling
 or validating a shader or shader program.
 
 @param shader The OpenGL reference to the shader.
 @param flag The OpenGL flag to check (such as GL_LINK_STATUS).
 @param isAProgram True if the shader is actually an entire
 shader program for validation.
 */
static void checkShaderError(GLuint shader, GLuint flag, bool isAProgram)
{
    static GLint BUFFER_SIZE = 512;
    GLint status;
    if(!isAProgram)
    {
        glGetShaderiv(shader, flag, &status);
        if(status != GL_TRUE)
        {
            char error[BUFFER_SIZE]; // Allocates space for the error message
            glGetShaderInfoLog(shader, BUFFER_SIZE, NULL, error);
            std::cout << error << std::endl;
        } // if
    } // if
    else
    {
        glGetProgramiv(shader, flag, &status);
        if(status != GL_TRUE)
        {
            char error[BUFFER_SIZE]; // Allocates space for the error message
            glGetProgramInfoLog(shader, BUFFER_SIZE, NULL, error);
            std::cout << error << std::endl;
        } // if
    } // else
} // checkShaderError(GLuint)
