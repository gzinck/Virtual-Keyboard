/**
 main.cpp
 Virtual Keyboard
 This class begins the application, displaying a piano keyboard
 on the screen with user interaction (mouse and WSAD keys).
 The user can click the F button to go to fullscreen and back,
 and the ESC button to exit.
 Required: OpenGL, SDL2, SDL2 Mixer, GLEW.
 
 @author Graeme Zinck
 @version 1.0 3/28/2018
*/

#define GLEW_STATIC
#include <GL/glew.h>

#include <iostream>
#include <string>
#include "Display.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Transform.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include "KeyboardKeys.hpp"
#include <SDL2_mixer/SDL_mixer.h>

#define WIDTH 800
#define HEIGHT 600
#define FIELD_OF_VIEW 70.0f
#define Z_NEAR 0.01f
#define Z_FAR 1000.0f
#define SHADER_NAME "/basicShader"

/**
 Begins the application.
 
 @return Zero if the program quit successfully.
*/
int main(int argc, char * argv[]) {
    // Get the shader's path from the user (depends on where they put it)
    std::cout << "Please type the path of the folder containing the resources (without a final slash):" << std::endl;
    // "/Users/graemezinck/Documents/OneDrive/Documents/University/Year2/Winter/COMP-3831/Final_Project/Virtual Keyboard/Virtual Keyboard/res" is the appropriate path
    std::string resPath;
    getline(std::cin, resPath);
    
    std::cout << "Use the W, S, A, D keys to move forward, back, left and right." << std::endl;
    std::cout << "Use the mouse to move around." << std::endl;
    std::cout << "Press F to switch to full-screen mode." << std::endl;
    std::cout << "Press K to switch the sounds from organ to piano and back." << std::endl;
    std::cout << "Press ESC to exit." << std::endl;
    
    // Creeate the display
    Display display(WIDTH, HEIGHT, "Virtual Keyboard");
    
    // Create a light object and a camera object
    Light light(glm::vec3(10,3,10), glm::vec3(1, 1, 1), 0.2, glm::vec3(0.0001, 0.001, 1));
    Camera camera(glm::vec3(0,5,10), FIELD_OF_VIEW, display.getAspectRatio(), Z_NEAR, Z_FAR);

    // Create the shader using the given path.
    Shader shader(resPath + SHADER_NAME);
    shader.bind(&light);
    
    // Create the keyboard keys
    KeyboardKeys keys(&shader, resPath);
    
    // Link the display to the camera and the keys
    display.setCamera(&camera);
    display.setKeyboardKeys(&keys);
    
    // Update the display continually (this will not actually refresh
    // the screen unless some action has been performed).
    while(!display.isClosed())
        display.update();
    
    return 0;
}
