/**
 Display.hpp
 Virtual Keyboard
 This class is for a display object which initializes the
 windowing system and creates an OpenGL context.
 This object is used to refresh the window while the program
 is running.
 
 @author Graeme Zinck
 @version 1.0 3/28/2018
*/

#ifndef Display_hpp
#define Display_hpp

#define GLEW_STATIC
#include <GL/glew.h>

#include <string>
#include <SDL2/SDL.h>
#include "Camera.hpp"
#include "Transform.hpp"
#include "Shader.hpp"
#include "KeyboardKeys.hpp"

class Display
{
public:
    Display(int width, int height, const std::string& title); // Constructor
    virtual ~Display(); // Destructor
    
    // Methods
    void clear(float r, float g, float b, float a);
    void update(); // Updates the display depending on user input
    bool isClosed();
    // Get characteristics of the window
    float getAspectRatio();
    int getHeight();
    int getWidth();
    // Get characteristics of the screen
    float getScreenAspectRatio();
    int getScreenHeight();
    int getScreenWidth();
    void setCamera(Camera * camera);
    void setKeyboardKeys(KeyboardKeys* keys);
private:
    SDL_Window* m_window; // Points to the SDL window object
    SDL_GLContext m_glContext; // Holds the OpenGL context to use
    Camera * m_camera; // Camera associated with the display (used for updating)
    KeyboardKeys* m_keyboardKeys; // KeyboardKeys (used for updating)
    
    // Variables with important facts about the display
    int m_width;
    int m_height;
    bool m_justOpened; // True when first initialize the display
    bool m_isClosed;
    bool m_isFullScreen;
    
    // Hold whether certain keys are pressed or not
    bool m_forwPressed;
    bool m_backPressed;
    bool m_rightPressed;
    bool m_leftPressed;
    
    // Class Constants
    const int RGB_SIZE = 8;
    const int ON = 1;
    const int NUM_AUDIO_CHANNELS = 64;
}; // Display

#endif /* Display_hpp */
