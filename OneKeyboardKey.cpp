/**
 OneKeyboardKey.cpp
 Virtual Keyboard
 Implementation of OneKeyboardKey.hpp
 
 @author Graeme Zinck
 @version 1.0 3/29/2018
 */

#include "OneKeyboardKey.hpp"
#include <iostream>

//--------------------------------------------------------------------------
/**
 Creates a KeyboardKey, which is a Mesh with some added features
 including a sound and keypress depth
 */
OneKeyboardKey::OneKeyboardKey(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices, Shader* shader, Transform transform, std::string organSoundPath, std::string pianoSoundPath)
: Mesh(vertices, numVertices, indices, numIndices, shader, transform)
{
    m_soundEffect = new Mix_Chunk*[NUM_SOUNDS];
    
    m_keyLevel = 0;
    m_soundEffect[ORGAN_SOUND] = Mix_LoadWAV(organSoundPath.c_str());
    m_soundEffect[PIANO_SOUND] = Mix_LoadWAV(pianoSoundPath.c_str());
    if(!m_soundEffect[ORGAN_SOUND])
        std::cout << "Problem with Mix_LoadWAV for the organ sound at key " << organSoundPath << "\n" << Mix_GetError();
    if(!m_soundEffect[PIANO_SOUND])
        std::cout << "Problem with Mix_LoadWAV for the piano sound at key " << pianoSoundPath << "\n" << Mix_GetError();
} // OneKeyboardKey::OneKeyboardKey(Vertex*, unsigned int, unsigned int*, unsigned int, Shader*, Transform, std::string, std::string)

//--------------------------------------------------------------------------
/**
 Destroys the KeyboardKey and stops the sound effect.
 */
OneKeyboardKey::~OneKeyboardKey()
{
    for(int i = 0; i < NUM_SOUNDS; i++)
    {
        Mix_FreeChunk(m_soundEffect[i]);
    } // for
    delete[] m_soundEffect;
    delete[] m_vertexArrayBuffers;
    glDeleteVertexArrays(1, &m_vertexArrayObject);
} // Mesh::~Mesh()

//--------------------------------------------------------------------------
/**
 Presses the key down by one keyLevel by moving
 the model matrix down in the y axis.
 */
void OneKeyboardKey::keyDown()
{
    m_keyLevel -= 1;
    m_transform.moveDown(INCREMENTAL_DEPTH);
} // OneKeyboardKey::keyDown()

//--------------------------------------------------------------------------
/**
 Pulls the key up by one keyLevel by moving the model
 matrix up in the y-axis.
 */
void OneKeyboardKey::keyUp()
{
    m_keyLevel += 1;
    m_transform.moveUp(INCREMENTAL_DEPTH);
} // OneKeyboardKey::keyUp()

//--------------------------------------------------------------------------
/**
 Detects if the key is currently moving (that is,
 it is not in the "down" position below level -5,
 and it is not in the "up" position at level 0).
 
 @return True if the key is in an intermediary
 position.
 */
bool OneKeyboardKey::keyIsMoving()
{
    if(m_keyLevel >= 0 || m_keyLevel < -NUM_INTERVALS)
        return false;
    else
        return true;
} // OneKeyboardKey::keyIsMoving()

//--------------------------------------------------------------------------
/**
 Plays the keyboard key's sound in a new channel.
 
 @param soundToPlay The index of the sound the user
 wants played, starting at index 0. In this case,
 0 is the organ and 1 is the piano.
 */
void OneKeyboardKey::playSound(int soundToPlay)
{
    m_soundChannel = Mix_PlayChannel(-1, m_soundEffect[soundToPlay], 0);
    if(m_soundChannel == -1)
        std::cout << "Mix_PlayChannel error: \n" << Mix_GetError() << "\n";
} // OneKeyboardKey::playSound()

//--------------------------------------------------------------------------
/**
 Stops the keyboard key's sound and frees the channel.
 */
void OneKeyboardKey::stopSound()
{
    Mix_FadeOutChannel(m_soundChannel, DELAY_BEFORE_STOP_SOUND);
} // OneKeyboardKey::stopSound()
