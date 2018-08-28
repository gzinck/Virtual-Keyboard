//
//  Texture.hpp
//  Virtual Keyboard
//
//  Created by Graeme Zinck on 2018-02-18.
//  Inspired by a tutorial by: BennyQBD
//  https://github.com/BennyQBD/ModernOpenGLTutorial
//

#ifndef Texture_hpp
#define Texture_hpp

#include <string>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

class Texture
{
public:
    Texture(const std::string& fileName);
    virtual ~Texture();
    
    // Methods
    void bind(unsigned int unit);
private:
    const int REQUIRED_COMPONENTS = 4;
    Texture(const Texture& other);
    void operator=(const Texture& rhs);
    
    GLuint m_texture;
}; // Texture

#endif /* Texture_hpp */
