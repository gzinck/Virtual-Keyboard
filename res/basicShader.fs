#version 150

/**
 basicShader.fs
 Virtual Keyboard
 This is a fragment shader for modern OpenGL which implements
 the Phong Shading Model.
 
 @author Graeme Zinck
 @version 1.0 3/28/2018
*/

// Variable from CPU
uniform vec3 cameraPosition;

// Structure which stores the properties of the light
uniform struct Light
{
    vec3 position;
    vec3 intensities; // color of light
    float ambientCoefficient;
    float attenuationFactorA;
    float attenuationFactorB;
    float attenuationFactorC;
} light;

// Structure which stores the properties of the material
uniform struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float specularExponent;
} material;

in vec3 fragPosition;
in vec3 fragNormal;

out vec4 outColor;

void main()
{
    // Create the vector for gamma correction, which is constant for CRT screens
    // which modern monitors mimic.
    vec3 gamma = vec3(1.0/2.2);
    
    // Calculates the vector from the fragment to the light source
    vec3 surfaceToLight = normalize(light.position - fragPosition);
    
    // AMBIENT COMPONENT:
    vec3 ambient = light.ambientCoefficient * light.intensities * material.ambient;
    
    // DIFFUSE COMPONENT:
    // Determines how bright it should be (i.e. cos theta, which is in [-1, 1]).
    // Clamps it to [0, 1] to avoid negative brightness of colours.
    float diffuseCoefficient = clamp( dot(fragNormal, surfaceToLight), 0.0, 1.0);
    vec3 diffuse = diffuseCoefficient * light.intensities * material.diffuse;
    
    // SPECULAR COMPONENT:
    // To avoid having shininess on the backside:
    float specularCoefficient = 0.0;
    if(diffuseCoefficient > 0.0)
    {
        vec3 incidenceVector = -surfaceToLight;
        vec3 reflectionVector = reflect(incidenceVector, fragNormal);
        // To get the camera's coordinates...
        vec3 surfaceToCamera = normalize(cameraPosition - fragPosition);
        float cosAngle = max(0.0, dot(reflectionVector, surfaceToCamera));
        specularCoefficient = pow(cosAngle, material.specularExponent);
    } // if
    vec3 specular = specularCoefficient * light.intensities * material.specular;
    
    // Getting attenuation
    float distanceToLight = distance(light.position, fragPosition);
    float attenuation = 1.0 / (light.attenuationFactorA * pow(distanceToLight, 2) + light.attenuationFactorB * distanceToLight + light.attenuationFactorC);
    
    vec3 linearColor = ambient + attenuation * (diffuse + specular);
    outColor = vec4(pow(linearColor, gamma), 1.0);
} // main
