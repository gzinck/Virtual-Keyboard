/**
 Display.cpp
 Virtual Keyboard
 This file implements display.hpp.
 
 @author Graeme Zinck
 @version 1.0 3/28/2018
*/

#include "Display.hpp"
#include <iostream>
#include <glm/glm.hpp>
#include <SDL2_mixer/SDL_mixer.h>

//--------------------------------------------------------------------------
/**
 Creates a display with a default width, height, and window
 title. It initializes SDL and SDL mixer and initializes some
 OpenGL settings to start off.
 
 @param width Width of the window to create.
 @param height Height of the window to create.
 @param title Title for the window to create.
*/
Display::Display(int width, int height, const std::string& title)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    
    // Frequency, format, channels, chunkSize of audio
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        std::cout << "SDL Mixer could not initialize: \n" << SDL_GetError();
    }
    Mix_AllocateChannels(NUM_AUDIO_CHANNELS);
    
    // Set up SDL using the appropriate sizes for components
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, RGB_SIZE);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, RGB_SIZE);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, RGB_SIZE);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, RGB_SIZE);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, RGB_SIZE * 4);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, RGB_SIZE * 2);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, ON);
    
    // Set the width and height of the window
    m_width = width;
    m_height = height;
    
    // Initialize all the buttons to be NOT pressed
    // (i.e., don't move the camera yet)
    m_forwPressed = false;
    m_backPressed = false;
    m_rightPressed = false;
    m_leftPressed = false;
    
    // Create the window with an OpenGL context
    m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_width, m_height, SDL_WINDOW_OPENGL);
    m_glContext = SDL_GL_CreateContext(m_window);
    m_isClosed = false; // The display will be open until the user quits somehow
    m_justOpened = true; // Triggers the first refresh of the screen
    m_isFullScreen = false; // Starts off in windowed mode
    
    // Make the mouse pointer disappear so the user can navigate
    // the keyboard world in first-person
    SDL_SetRelativeMouseMode(SDL_TRUE);
    
    // Disable legacy OpenGL commands and start up OpenGL
    glewExperimental = GL_TRUE;
    glewInit();
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE); // Don't draw the faces that are NOT facing the camera
    glCullFace(GL_BACK);
    glShadeModel(GL_SMOOTH);
} // Display::Display(int, int, const std::string&)

//--------------------------------------------------------------------------
/**
 Destroys the Display object by deleting the GL context,
 removing the window, and quitting SDL.
*/
Display::~Display()
{
    SDL_GL_DeleteContext(m_glContext);
    SDL_DestroyWindow(m_window);
    Mix_Quit();
    SDL_Quit();
} // Display::~Display()

//--------------------------------------------------------------------------
/**
 Clears the background colour of the window with the
 specified colour.
 
 @param r Red component of colour.
 @param g Green component of colour.
 @param b Blue component of colour.
 @param a Alpha component of colour.
*/
void Display::clear(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
} // clear(float, float, float, float);

//--------------------------------------------------------------------------
/**
 Updates the display if needed.
 Situations where this will occur:
 1) If the display was just created
 2) If a keyboard key is currently moving
 3) If W, S, A, or D is pressed (to move)
 4) If W, S, A, or D was previously pressed and not yet released
 5) If F or ESC is pressed (fullscreen/windowed mode toggles)
 6) If the cursor moves (the camera)
 It then moves the camera accordingly and redraws the scene
 if necessary.
*/
void Display::update()
{
    SDL_Event e;
    bool mustUpdate = false;
    
    // If the display was just opened
    if(m_justOpened)
    {
        mustUpdate = true;
        m_justOpened = false;
    } // if
    
    // If a key is moving down, update!
    if(m_keyboardKeys->keyIsMoving())
        mustUpdate = true;
    
    // Update, depending on interaction
    while(SDL_PollEvent(&e)) // Take the next event in queue, put it in e.
    {
        if(e.type == SDL_QUIT) // If user quit, then close the window
        {
            m_isClosed = true;
        } // if
        else if(e.type == SDL_KEYDOWN)
        {
            switch (e.key.keysym.scancode) {
                case SDL_SCANCODE_W:
                    m_forwPressed = true;
                    break;
                    
                case SDL_SCANCODE_S:
                    m_backPressed = true;
                    break;
                    
                case SDL_SCANCODE_D:
                    m_rightPressed = true;
                    break;
                    
                case SDL_SCANCODE_A:
                    m_leftPressed = true;
                    break;
                    
                case SDL_SCANCODE_F:
                    // Toggle fullscreen mode
                    if(!m_isFullScreen)
                    {
                        int w = getScreenWidth();
                        int h = getScreenHeight();
                        SDL_SetWindowSize(m_window, w, h);
                        SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);
                        SDL_SetWindowDisplayMode(m_window, NULL);
                        m_camera->updateAspectRatio(getScreenAspectRatio());
                        m_isFullScreen = true;
                        mustUpdate = true;
                    } // if
                    else
                    {
                        SDL_SetWindowFullscreen(m_window, 0);
                        glViewport(0, 0, m_width, m_height);
                        SDL_SetWindowSize(m_window, m_width, m_height);
                        m_camera->updateAspectRatio(getAspectRatio());
                        m_isFullScreen = false;
                        mustUpdate = true;
                    } // else
                    break;
                    
                case SDL_SCANCODE_ESCAPE:
                    if(m_isFullScreen)
                    {
                        SDL_SetWindowFullscreen(m_window, 0);
                        glViewport(0, 0, m_width, m_height);
                        SDL_SetWindowSize(m_window, m_width, m_height);
                        m_camera->updateAspectRatio(getAspectRatio());
                        m_isFullScreen = false;
                        mustUpdate = true;
                    } // if
                    else
                    {
                        m_isClosed = true;
                    } // else
                    break;
                    
                case SDL_SCANCODE_K: // Switch the keyboard sound to piano/organ
                    m_keyboardKeys->nextSoundSetting();
                    break;
                    
                default:
                    break;
            } // switch
        } // else if
        else if(e.type == SDL_KEYUP)
        {
            switch (e.key.keysym.scancode) {
                case SDL_SCANCODE_W:
                    m_forwPressed = false;
                    break;
                    
                case SDL_SCANCODE_S:
                    m_backPressed = false;
                    break;
                    
                case SDL_SCANCODE_D:
                    m_rightPressed = false;
                    break;
                    
                case SDL_SCANCODE_A:
                    m_leftPressed = false;
                    break;
                    
                default:
                    break;
            } // switch
        } // else if
        else if (e.type == SDL_MOUSEMOTION)
        {
            m_camera->turnXY(e.motion.xrel, e.motion.yrel);
            mustUpdate = true;
        } // else if
    } // while
    
    // Check if a button is currently pressed down
    if(m_forwPressed)
    {
        m_camera->moveForward();
        mustUpdate = true;
    } // if
    if(m_backPressed)
    {
        m_camera->moveBackward();
        mustUpdate = true;
    } // if
    if(m_rightPressed)
    {
        m_camera->moveRight();
        mustUpdate = true;
    } // if
    if(m_leftPressed)
    {
        m_camera->moveLeft();
        mustUpdate = true;
    } // if
    
    // ONLY UPDATE if there was motion
    if(mustUpdate)
    {
        clear(0.0f, 0.15f, 0.3f, 1.0f);
        // Get the key
        int theKey = m_keyboardKeys->getSelectedKey(m_camera->getPos());
        m_keyboardKeys->keyDown(theKey); // Press the key down (if not already down)
        m_keyboardKeys->draw(m_camera); // Draw all the keys
        SDL_GL_SwapWindow(m_window); // Swap buffers
    } // if
} // Display::update()

//--------------------------------------------------------------------------
/**
 Checks if the display should be closed.
 
 @return True if the user wishes to close the window.
*/
bool Display::isClosed()
{
    return m_isClosed;
} // isClosed()

//--------------------------------------------------------------------------
/**
 Gets the aspect ratio of the current window.
 
 @return Aspect ratio of the current window, equal to
 width divided by height.
*/
float Display::getAspectRatio()
{
    int w;
    int h;
    SDL_GL_GetDrawableSize(m_window, &w, &h);
    return (float)w / (float)h;
} // Display::getAspectRatio()

//--------------------------------------------------------------------------
/**
 Gets the width of the current window.
 
 @return Integer value representing the width.
*/
int Display::getWidth()
{
    int w;
    int h;
    SDL_GL_GetDrawableSize(m_window, &w, &h);
    return w;
} // Display::getWidth()

//--------------------------------------------------------------------------
/**
 Gets the height of the current window.
 
 @return Integer value representing the height.
 */
int Display::getHeight()
{
    int w;
    int h;
    SDL_GL_GetDrawableSize(m_window, &w, &h);
    return h;
} // Display::getHeight()

//--------------------------------------------------------------------------
/**
 Gets the aspect ratio of the screen.
 
 @return Aspect ratio of the current window, equal to
 width divided by height.
 */
float Display::getScreenAspectRatio()
{
    SDL_DisplayMode dm;
    if(SDL_GetCurrentDisplayMode(0, &dm) != 0)
        std::cout << "SDL_GetDesktopDisplayMode failed: " << SDL_GetError();
    float width = dm.w;
    float height = dm.h;
    return width / height;
} // Display::getScreenAspectRatio()

//--------------------------------------------------------------------------
/**
 Gets the width of the screen
 
 @return Integer value representing the width.
 */
int Display::getScreenWidth()
{
    SDL_DisplayMode dm;
    if(SDL_GetCurrentDisplayMode(0, &dm) != 0)
        std::cout << "SDL_GetDesktopDisplayMode failed: " << SDL_GetError();
    return dm.w;
} // Display::getScreenWidth()

//--------------------------------------------------------------------------
/**
 Gets the height of the screen.
 
 @return Integer value representing the height.
 */
int Display::getScreenHeight()
{
    SDL_DisplayMode dm;
    if(SDL_GetCurrentDisplayMode(0, &dm) != 0)
        std::cout << "SDL_GetDesktopDisplayMode failed: " << SDL_GetError();
    return dm.h;
} // Display::getScreenHeight()

//--------------------------------------------------------------------------
/**
 Sets the camera which the display will be using.
*/
void Display::setCamera(Camera * camera)
{
    m_camera = camera;
} // Display::setCamera(Camera)

//--------------------------------------------------------------------------
/**
 Sets the keys that the display will be drawing.
*/
void Display::setKeyboardKeys(KeyboardKeys* keys)
{
    m_keyboardKeys = keys;
} // Display::setKeyboardKeys(KeyboardKeys*)
