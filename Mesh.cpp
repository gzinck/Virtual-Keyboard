/**
 Mesh.cpp
 Virtual Keyboard
 Implementation of Mesh.hpp.
 
 @author Graeme Zinck
 @version 1.0 3/28/2018
 */

#include "Mesh.hpp"
#include <vector>
#include <iostream>

//--------------------------------------------------------------------------
/**
 Creates a mesh object and assigns instance variables by
 filling up a Model object with the positions, indices,
 and normals.
*/
Mesh::Mesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices, Shader* shader, Transform transform)
{
    m_shader = shader;
    m_transform = transform;
    m_specularExponent = 1; // Default
    
    // Put everything into a model (this makes it easy to put into
    // vertex buffers later).
    Model model;
    for(unsigned int i = 0; i < numVertices; i++)
    {
        model.positions.push_back(*vertices[i].getPos());
        model.normals.push_back(*vertices[i].getNormal());
    } // for
    
    for(unsigned int i = 0; i < numIndices; i++)
        model.indices.push_back(indices[i]);
    
    initMesh(model);
} // Mesh::Mesh(Vertex*, unsigned int, unsigned int*, unsigned int, Shader*, Transform)

//--------------------------------------------------------------------------
/**
 Uses a model to finish creating the mesh.
 Allocates space on the GPU for all of the elements, and
 binds the buffers.
 
 @param model A model with positions, normals, and indices
 for the mesh.
*/
void Mesh::initMesh(const Model& model)
{
    m_drawCount = model.indices.size();
    
    glGenVertexArrays(1, &m_vertexArrayObject);
    glBindVertexArray(m_vertexArrayObject);
    
    m_vertexArrayBuffers = new GLuint[NUM_BUFFERS]; // Needed to avoid having EXC_BAD_ACCESS error
    glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
    
    // BUFFER 1: for the vector positions
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
    glBufferData(GL_ARRAY_BUFFER, model.positions.size() * sizeof(model.positions[0]), &model.positions[0], GL_STATIC_DRAW); // STATIC_DRAW means the data will not be changed
    
    GLint posAttrib = glGetAttribLocation(m_shader->getShaderProgram(), "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    // BUFFER 2: for the normals of the vertices
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[NORMAL_VB]);
    glBufferData(GL_ARRAY_BUFFER, model.normals.size() * sizeof(model.normals[0]), &model.normals[0], GL_STATIC_DRAW); // STATIC_DRAW means the data will not be changed; it's the draw hint
    
    GLint normAttrib = glGetAttribLocation(m_shader->getShaderProgram(), "normal");
    glEnableVertexAttribArray(normAttrib);
    glVertexAttribPointer(normAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    // BUFFER 3: for the indices of the vertices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]); // Array, but references the data of other arrays
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indices.size() * sizeof(model.indices[0]), &model.indices[0], GL_STATIC_DRAW);
    
    glBindVertexArray(0);
} // Mesh::initMesh(const Model&)

//--------------------------------------------------------------------------
/**
 Destroys the mesh.
*/
Mesh::~Mesh()
{
    delete[] m_vertexArrayBuffers;
    glDeleteVertexArrays(1, &m_vertexArrayObject);
} // Mesh::~Mesh()

//--------------------------------------------------------------------------
/**
 Draws the mesh.
 
 @param camera The camera to use when drawing the key.
*/
void Mesh::draw(Camera* camera)
{
    glBindVertexArray(m_vertexArrayObject);
    
    m_shader->setMaterial(m_ambient, m_diffuse, m_specular, m_specularExponent);
    m_shader->update(m_transform, camera);
    glDrawElements(GL_TRIANGLES, m_drawCount, GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);
} // Mesh::draw()

//--------------------------------------------------------------------------
/**
 Sets the properties of the material for the mesh.
 
 @param ambient Amount the material reflects ambient R, G, and B light.
 @param diffuse Amount the material reflects diffuse R, G, and B light.
 @param specular Amount the material reflects specular R, G, and B light.
 @param specularExponent How mirror-like the surface is.
*/
void Mesh::setMaterialProperties(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float specularExponent)
{
    m_ambient = ambient;
    m_diffuse = diffuse;
    m_specular = specular;
    m_specularExponent = specularExponent;
} // Mesh::setMaterialProperties(glm::vec3, glm::vec3, glm::vec3, float)
