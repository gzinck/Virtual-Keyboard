#version 150

/**
 basicShader.vs
 Virtual Keyboard
 This is a vertex shader for modern OpenGL which uses model and
 view matrices to determine the vertex coordinates and normals.
 
 @author Graeme Zinck
 @version 1.0 3/28/2018
 */

// Variable set by CPU
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

in vec3 position;
in vec3 normal;

out vec3 fragNormal;
out vec3 fragPosition;

void main()
{
    // Position of the fragment in world coordinates
    fragPosition = vec3(modelMatrix * vec4(position, 1.0));
    
    // Calculate the normal in world coordinates and pass it to the vertex shader
    mat3 normalMatrix = transpose(inverse(mat3(modelMatrix))); // To remove the scaling and translation
    fragNormal = normalize(normalMatrix * normal);
    
    // Get the position of the vertex by first multiplying by the model,
    // then the view matrix (reverse order)
    gl_Position = viewMatrix * modelMatrix * vec4(position, 1.0); // Set to 0 if we want no translation
} // main
