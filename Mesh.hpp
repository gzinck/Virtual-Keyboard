/**
 Mesh.hpp
 Virtual Keyboard
 A class useful for storing vertices and placing these
 on buffers on the GPU.
 
 @author Graeme Zinck
 @version 1.0 3/28/2018
 */

#ifndef Mesh_hpp
#define Mesh_hpp

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Shader.hpp"
#include "Camera.hpp"
#include "Transform.hpp"

//--------------------------------------------------------------------------
/**
 Vertex class creates a container for a position and a normal.
*/
class Vertex
{
public:
    Vertex(const glm::vec3& pos, const glm::vec3& normal = glm::vec3(0,0,0))
    {
        this->m_pos = pos;
        this->m_normal = normal;
    } // Vertex(const glm::vec3)
    
    inline glm::vec3* getPos() { return &m_pos; }
    inline glm::vec3* getNormal() { return &m_normal; }
private:
    glm::vec3 m_pos;
    glm::vec3 m_normal;
}; // Vertex

//--------------------------------------------------------------------------
/**
 Model class creates a container for an array of positions, normals,
 and indices which will be associated with the mesh.
*/
class Model
{
public:
    Model() {};
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;
}; // Model

//--------------------------------------------------------------------------
/**
 Mesh class puts vertices, normals, and indices into buffers on the GPU
 and allows the user to easily draw the mesh (updating the material
 properties and model matrix easily).
*/
class Mesh
{
public:
    Mesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices, Shader* shader, Transform transform);
    virtual ~Mesh();
    
    // Methods
    void draw(Camera* camera);
    void setMaterialProperties(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float specularExponent);
    inline Transform* getTransform() { return &m_transform; };
    
protected:
    void initMesh(const Model& model);
    
    // An enumerated type for the vertex buffers which go in the m_vertexArrayBuffers array.
    enum {
        POSITION_VB,
        NORMAL_VB,
        INDEX_VB,
        
        NUM_BUFFERS
    }; // enum
    
    GLuint m_vertexArrayObject; // Big object with all the vertices (or, a pointer to it)
    GLuint* m_vertexArrayBuffers; // Array of all the buffer locations
    unsigned int m_drawCount; // How much of the object we want to draw (how big is m_vertexArrayObject that we want to use?)
    
    // The shader pointer is stored in the mesh because it is
    // permanently associated with the shader (due to how the
    // buffers are created).
    Shader* m_shader; // Stores the shader so that we can query which attributes to use
    Transform m_transform; // Object for the transformations of the mesh
    
    // Store properties of the material:
    float m_specularExponent;
    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;
}; // Mesh

#endif /* Mesh_hpp */
