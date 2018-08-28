/**
 OneKeyboardKey.hpp
 Virtual Keyboard
 A specialized mesh for holding one keyboard key,
 including the sound, keypress level, etc.
 It has special functions for pressing down and
 lifting up a key.
 
 @author Graeme Zinck
 @version 1.0 3/29/2018
 */

#ifndef OneKeyboardKey_hpp
#define OneKeyboardKey_hpp

#include "Mesh.hpp"
#include <iostream>
#include <SDL2_mixer/SDL_mixer.h>
#include <string>

class OneKeyboardKey : public Mesh
{
public:
    // Constructor/Destructor
    OneKeyboardKey(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices, Shader* shader, Transform transform, std::string organSoundPath, std::string pianoSoundPath);
    virtual ~OneKeyboardKey();
    
    // Methods
    void keyDown();
    void keyUp();
    bool keyIsMoving();
    void playSound(int soundToPlay);
    void stopSound();
    inline bool isAtBottom() { return m_keyLevel == -NUM_INTERVALS; }
    inline bool isAtTop() { return m_keyLevel >= 0; }
    
    // Public enum for which index the organ and piano sounds
    // are.
    enum {
        ORGAN_SOUND,
        PIANO_SOUND,
        NUM_SOUNDS
    };
private:
    inline float getKeyLevel() { return m_keyLevel; };
    int m_keyLevel; // Stores what level the key is at (if it is being pressed)
    
    // Sound information
    Mix_Chunk** m_soundEffect; // Stores the sound effect for the key
    int m_soundChannel; // Stores the channel the sound is playing on
    
    // Constants
    const int NUM_INTERVALS = 5; // How many different levels the key can go down to
    const int KEYPRESS_DEPTH = 1; // How far down a key goes down
    const float INCREMENTAL_DEPTH = (float)KEYPRESS_DEPTH / (float)NUM_INTERVALS; // How much to go down each time
    const int DELAY_BEFORE_STOP_SOUND = 1000; // How long note lasts after keyUp
    
}; // OneKeyboardKey

#endif /* OneKeyboardKey_hpp */
