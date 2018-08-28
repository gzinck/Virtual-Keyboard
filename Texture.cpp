//
//  Texture.cpp
//  Virtual Keyboard
//
//  Created by Graeme Zinck on 2018-02-18.
//  Inspired by a tutorial by: BennyQBD
//  https://github.com/BennyQBD/ModernOpenGLTutorial
//

#include "Texture.hpp"
#include "io/stb_image.h"
#include <cassert>
#include <iostream>

Texture::Texture(const std::string& fileName)
{
    int width, height, numComponents;
    unsigned char* imageData = stbi_load(fileName.c_str(), &width, &height, &numComponents, REQUIRED_COMPONENTS);
    
    if(imageData == NULL)
        std::cerr << "Texture loading failed for texture: " << fileName << std::endl;
    
    glGenTextures(1, &m_texture); // Make 1 texture in the address m_texture
    glBindTexture(GL_TEXTURE_2D, m_texture);
    
    // How the texture repeats
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    // Interpolating/extrapolating the texture (when zoom in/out)
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // For minification
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // For magnification
    
    // Type of texture data
    // level (mipmaping—textures of different sizes of same texture—use higher resultion texture when close, lower when further)
    // internalformat (how store pixels on GPU
    // width
    // height
    // border
    // input format sent to gpu
    // data type: how stored
    // *pixels which has the pixel data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
    
    stbi_image_free(imageData);
} // Texture()

Texture::~Texture()
{
    glDeleteTextures(1, &m_texture); // deletes one texture at location m_texture
} // ~Texture()

void Texture::bind(unsigned int unit)
{
    assert(unit >= 0 && unit <= 31); // Ensures stays within bounds of the possible texture index
    
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, m_texture);
}
