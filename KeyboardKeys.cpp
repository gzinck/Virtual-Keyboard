/**
 KeyboardKeys.cpp
 Virtual Keyboard
 Implementation of KeyboardKeys.hpp.
 
 @author Graeme Zinck
 @version 1.0 3/28/2018
*/

#include "KeyboardKeys.hpp"
#include "Transform.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <math.h>

// Helper methods
//--------------------------------------------------------------------------
/**
 Callback function for SDL timer function which
 presses down a key in increments until it is at
 the bottom.
 
 @param interval Milliseconds between increments pushing
 down a keyboard key.
 @param param A keyboard key to press down.
*/
Uint32 pressDownKey(Uint32 interval, void* param)
{
    OneKeyboardKey* theKey = (OneKeyboardKey*)param;
    if(!theKey->isAtBottom())
    {
        theKey->keyDown();
        return interval;
    } // if
    return 0;
} // pressDownKey(Uint32, void*)

//--------------------------------------------------------------------------
/**
 Callback function for SDL timer function which
 pulls up a key in increments until it is at
 the top.
 
 @param interval Milliseconds between increments pulling
 up a keyboard key.
 @param param A keyboard key to pull up.
 */
Uint32 liftUpKey(Uint32 interval, void* param)
{
    OneKeyboardKey* theKey = (OneKeyboardKey*)param;
    if(!theKey->isAtTop())
    {
        theKey->keyUp();
        return interval;
    } // if
    return 0;
} // liftUpKey(Uint32, void*)

//--------------------------------------------------------------------------
/**
 Creates the KeyboardKeys object by initializing
 all the white keys and black keys and putting them
 in an array.
 
 @param shader The shader, which the keyboard keys will
 need so that they can set their material properties when
 they are drawn.
*/
KeyboardKeys::KeyboardKeys(Shader* shader, std::string resourceFolder)
{
    resFolder = resourceFolder;
    
    // Initialize the arrays of keys
    whiteKeys = new OneKeyboardKey*[NUM_WHITE_KEYS];
    blackKeys = new OneKeyboardKey*[NUM_BLACK_KEYS];
    
    // No key down at the moment
    m_curKeyDown = -1;
    
    // Initialize the sound as sound 0 (the organ sound)
    m_soundToUse = 0;
    
    // Hold a transformation matrix to move the vertices
    // from model coordinates to world coordinates
    Transform transform;
    
    int whiteKeysFilled = 0;
    int blackKeysFilled = 0;
    
    // Create the A-key to start (has notch in right side)
    makeWhiteKeyR(whiteKeysFilled++, shader, transform, "0a");
    
    // Create the Bb-key
    makeBlackKey(blackKeysFilled++, shader, transform, "0bb");
    transform.moveRight(X_DIFF_BETWEEN_WHITE_KEYS);
    
    // Create the B-key (has notch in left side)
    makeWhiteKeyL(whiteKeysFilled++, shader, transform, "0b");
    transform.moveRight(X_DIFF_BETWEEN_WHITE_KEYS);
    
    // Create octaves from C to B (this is the convention for note names musically)
    for(unsigned int octave = 1; octave <= NUM_OCTAVES; octave++)
    {
        std::string octaveStr = std::to_string(octave);
        // C-key (R)
        makeWhiteKeyR(whiteKeysFilled++, shader, transform, octaveStr + "c");
        // Db-key
        makeBlackKey(blackKeysFilled++, shader, transform, octaveStr + "db");
        transform.moveRight(X_DIFF_BETWEEN_WHITE_KEYS);
        // D-key (LR)
        makeWhiteKeyLR(whiteKeysFilled++, shader, transform, octaveStr + "d");
        // Eb-key
        makeBlackKey(blackKeysFilled++, shader, transform, octaveStr + "eb");
        transform.moveRight(X_DIFF_BETWEEN_WHITE_KEYS);
        // E-key (L)
        makeWhiteKeyL(whiteKeysFilled++, shader, transform, octaveStr + "e");
        transform.moveRight(X_DIFF_BETWEEN_WHITE_KEYS);
        // F-key (R)
        makeWhiteKeyR(whiteKeysFilled++, shader, transform, octaveStr + "f");
        // Gb-key
        makeBlackKey(blackKeysFilled++, shader, transform, octaveStr + "gb");
        transform.moveRight(X_DIFF_BETWEEN_WHITE_KEYS);
        // G-key (LR)
        makeWhiteKeyLR(whiteKeysFilled++, shader, transform, octaveStr + "g");
        // Ab-key
        makeBlackKey(blackKeysFilled++, shader, transform, octaveStr + "ab");
        transform.moveRight(X_DIFF_BETWEEN_WHITE_KEYS);
        // A-key (LR)
        makeWhiteKeyLR(whiteKeysFilled++, shader, transform, octaveStr + "a");
        // Bb-key
        makeBlackKey(blackKeysFilled++, shader, transform, octaveStr + "bb");
        transform.moveRight(X_DIFF_BETWEEN_WHITE_KEYS);
        // B-key (L)
        makeWhiteKeyL(whiteKeysFilled++, shader, transform, octaveStr + "b");
        transform.moveRight(X_DIFF_BETWEEN_WHITE_KEYS);
    } // for
    
    // Create the last C-key (has no notches)
    makeWhiteKey(whiteKeysFilled++, shader, transform, std::to_string(NUM_OCTAVES + 1) + "c");
    transform.moveRight(X_DIFF_BETWEEN_WHITE_KEYS);
} // KeyboardKeys::KeyboardKeys(Shader*)

//--------------------------------------------------------------------------
/**
 Destroys the KeyboardKeys object.
*/
KeyboardKeys::~KeyboardKeys()
{
    delete[] whiteKeys;
    delete[] blackKeys;
} // KeyboardKeys::~KeyboardKeys()

//--------------------------------------------------------------------------
/**
 Changes the sound effect for the keyboard to the next available
 setting (organ or piano).
 */
void KeyboardKeys::nextSoundSetting()
{
    m_soundToUse = (m_soundToUse + 1) % OneKeyboardKey::NUM_SOUNDS;
} // KeyboardKeys::setSound(int)

//--------------------------------------------------------------------------
/**
 Draws all the white and black keys in the positions
 already defined within the key objects.
 
 @param camera The camera to use when drawing the key.
*/
void KeyboardKeys::draw(Camera* camera)
{
    for(unsigned int i = 0; i < NUM_WHITE_KEYS; i++)
    {
        whiteKeys[i]->draw(camera);
    } // for
    for(unsigned int i = 0; i < NUM_BLACK_KEYS; i++)
    {
        blackKeys[i]->draw(camera);
    } // for
} // KeyboardKeys::draw()

//--------------------------------------------------------------------------
/**
 Gets the key which the user is selecting based on the
 user's x-position. While this seems trivial, it involves
 distinguishing between white and black notes (which follow
 a regular 12-note pattern).
 It identifies the edge cases, and then identifies which
 octave the notes lie in (where Ab0 through G1 on the keyboard
 are all considered in "octave 1" for simpler coding, despite
 the fact that Ab0 does not exist. This is irrelevant, so the
 implementation uses a simpler method).
 Then, it finds which of the 12 black/white notes is being
 selected in the given octave, and the index of the key
 is returned.
 
 @return The index of the note being returned. The convention is
 that 0-51 are for white notes, -1 is for no selection, and
 (-2)-(-37) are for black notes (to distinguish between which
 array the note is in, since if it is below -1, we know the key
 is in the black keys array).
*/
int KeyboardKeys::getSelectedKey(glm::vec3 position)
{
    float x = position.x;
    
    // --------EDGE CASES--------
    // Edge case: too far left, no note
    if(x < 0)   return -1;
    // If it's super small, then just select the first key
    if(x < X_DIFF_BETWEEN_WHITE_KEYS * 0.75)   return 0;
    // Edge case: too far right, no note
    if(x > NUM_WHITE_KEYS * X_DIFF_BETWEEN_WHITE_KEYS)  return -1;
    // If it's super high, then just select the white key
    if(x > ((double)NUM_WHITE_KEYS - 0.5) * X_DIFF_BETWEEN_WHITE_KEYS)    return NUM_WHITE_KEYS - 1;
    
    // Get the index of the key (in terms of white key positions)
    double keyIndex = x / X_DIFF_BETWEEN_WHITE_KEYS;
    int octave = keyIndex/NUM_KEYS_IN_OCTAVE;
    // To make it easier to count, make the index
    // just start from the beginning of the octave
    keyIndex = fmod(keyIndex, NUM_KEYS_IN_OCTAVE);
    
    // Ab key (right end of it)
    if(keyIndex < 0.25)
        return (int)(- octave * 5 - 1);
    // A key
    if(keyIndex < 0.75)
        return (int)(octave * 7);
    // Bb key
    if(keyIndex < 1.25)
        return (int)(- octave * 5 - 2);
    // B key
    if(keyIndex < 2)
        return (int)(octave * 7 + 1);
    // C key
    if(keyIndex < 2.75)
        return (int)(octave * 7 + 2);
    // Db key
    if(keyIndex < 3.25)
        return (int)(- octave * 5 - 3);
    // D
    if(keyIndex < 3.75)
        return (int)(octave * 7 + 3);
    // Eb key
    if(keyIndex < 4.25)
        return (int)(- octave * 5 - 4);
    // E
    if(keyIndex < 5)
        return (int)(octave * 7 + 4);
    // F
    if(keyIndex < 5.75)
        return (int)(octave * 7 + 5);
    // Gb
    if(keyIndex < 6.25)
        return (int)(- octave * 5 - 5);
    // G
    if(keyIndex < 6.75)
        return (int)(octave * 7 + 6);
    // Ab (left end of it)
    return (int)(- octave * 5 - 6);
} // KeyboardKeys::getSelectedKey()

//--------------------------------------------------------------------------
/**
 Checks if a given key is already down
 
 @param key Integer representing the index of the
 desired key (-2 or lower if it is a black key, as
 per convention).
 @return True if the key is down.
*/
bool KeyboardKeys::keyIsDown(int key)
{
    return (m_curKeyDown == key);
} // KeyboardKeys::keyIsDown(int)

//--------------------------------------------------------------------------
/**
 Checks if the key that is currently down is moving
 at a given time.
 
 @return True if the currently selected key is moving.
 False if no key is currently down or if it is not moving.
*/
bool KeyboardKeys::keyIsMoving()
{
    // Distinguish between cases (white vs black)
    if(m_curKeyDown >= 0 && m_curKeyDown < NUM_WHITE_KEYS) // WHITE keys
        return whiteKeys[m_curKeyDown]->keyIsMoving();
    else if(m_curKeyDown < -1 && m_curKeyDown > - (NUM_BLACK_KEYS + 2)) // BLACK keys
        return blackKeys[- (m_curKeyDown + 2)]->keyIsMoving();
    else // NO keys
        return false;
} // KeyboardKeys::keyIsMoving()

//--------------------------------------------------------------------------
/**
 Moves down a keyboard key incrementally based on the
 index (which is 0+ for white, -1 for none, and -2- for
 black). Pulls up the key that was previously down.
 Does nothing if the key was already down or if the key
 does not exist.
 
 @param key Index of the key to push down (0+ for white, -2-
 for black).
*/
void KeyboardKeys::keyDown(int key)
{
    // Make sure the current key is different from the key selected
    if(m_curKeyDown != key)
    {
        // Check if the key to press is white
        if(key >= 0 && key < NUM_WHITE_KEYS)
        {
            keyUp(m_curKeyDown);
            m_curKeyDown = key;
            OneKeyboardKey* theKey = whiteKeys[key];
            theKey->playSound(m_soundToUse);
            SDL_AddTimer(DELAY, pressDownKey, theKey);
        } // if
        // Check if the key to press is black
        // Convention: black keys start at -2 and
        // go down
        if(key < -1 && key > - (NUM_BLACK_KEYS + 2))
        {
            keyUp(m_curKeyDown);
            m_curKeyDown = key;
            int positiveKey = - (key + 2);
            OneKeyboardKey* theKey = blackKeys[positiveKey];
            theKey->playSound(m_soundToUse);
            SDL_AddTimer(DELAY, pressDownKey, theKey);
        } // if
    } // if
} // KeyboardKeys::keyDown(int)

//--------------------------------------------------------------------------
/**
 Moves up a keyboard key incrementally based on the
 index (which is 0+ for white, -1 for none, and -2- for
 black).
 Does nothing if the key was already down or if the key
 does not exist.
 
 @param key Index of the key to pull up (0+ for white, -2-
 for black).
 */
void KeyboardKeys::keyUp(int key)
{
    // If it's white...
    if(key >= 0 && key < NUM_WHITE_KEYS)
    {
        OneKeyboardKey* theKey = whiteKeys[key];
        theKey->stopSound();
        SDL_AddTimer(DELAY, liftUpKey, theKey);
    } // if
    // If it's black...
    if(key < -1 && key > - (NUM_BLACK_KEYS + 2))
    {
        int positiveKey = - (key + 2);
        OneKeyboardKey* theKey = blackKeys[positiveKey];
        theKey->stopSound();
        SDL_AddTimer(DELAY, liftUpKey, theKey);
    } // if
} // KeyboardKeys::keyUp(int)

// HELPER METHODS to create each type of key
//--------------------------------------------------------------------------
/**
 Creates a black key.
 
 @param keysFilled Index of the blackKeys array to add
 the key.
 @param shader Pointer to the shader, used to update the
 material property uniforms in the shader.
 @param transform The model matrix for the key.
 @param keyName the name of the key, used for getting the
 audio file for the key.
*/
void KeyboardKeys::makeBlackKey(int keysFilled, Shader* shader, Transform transform, std::string keyName)
{
    std::string organSoundPath = resFolder + ORGAN_FOLDER + keyName + SOUND_EXTENSION;
    std::string pianoSoundPath = resFolder + PIANO_FOLDER + keyName + SOUND_EXTENSION;
    blackKeys[keysFilled] = new OneKeyboardKey(blackVertices, sizeof(blackVertices)/sizeof(blackVertices[0]), blackKeyIndices, sizeof(blackKeyIndices)/sizeof(blackKeyIndices[0]), shader, transform, organSoundPath, pianoSoundPath);
    blackKeys[keysFilled]->setMaterialProperties(BLACK_A, BLACK_D, BLACK_S, SPECULAR_EXPONENT);
} // KeyboardKeys::makeWhiteKey(int, Shader*, Transform, std::string)

//--------------------------------------------------------------------------
/**
 Creates a white key with no notches.
 
 @param keysFilled Index of the whiteKeys array to add
 the key.
 @param shader Pointer to the shader, used to update the
 material property uniforms in the shader.
 @param transform The model matrix for the key.
 @param keyName the name of the key, used for getting the
 audio file for the key.
 */
void KeyboardKeys::makeWhiteKey(int keysFilled, Shader* shader, Transform transform, std::string keyName)
{
    std::string organSoundPath = resFolder + ORGAN_FOLDER + keyName + SOUND_EXTENSION;
    std::string pianoSoundPath = resFolder + PIANO_FOLDER + keyName + SOUND_EXTENSION;
    whiteKeys[keysFilled] = new OneKeyboardKey(whiteVertices, sizeof(whiteVertices)/sizeof(whiteVertices[0]), whiteKeyIndices, sizeof(whiteKeyIndices)/sizeof(whiteKeyIndices[0]), shader, transform, organSoundPath, pianoSoundPath);
    whiteKeys[keysFilled]->setMaterialProperties(WHITE_A, WHITE_D, WHITE_S, SPECULAR_EXPONENT);
} // KeyboardKeys::makeWhiteKey(int, Shader*, Transform, std::string)

//--------------------------------------------------------------------------
/**
 Creates a white key with no right notch.
 
 @param keysFilled Index of the whiteKeys array to add
 the key.
 @param shader Pointer to the shader, used to update the
 material property uniforms in the shader.
 @param transform The model matrix for the key.
 @param keyName the name of the key, used for getting the
 audio file for the key.
 */
void KeyboardKeys::makeWhiteKeyR(int keysFilled, Shader* shader, Transform transform, std::string keyName)
{
    std::string organSoundPath = resFolder + ORGAN_FOLDER + keyName + SOUND_EXTENSION;
    std::string pianoSoundPath = resFolder + PIANO_FOLDER + keyName + SOUND_EXTENSION;
    whiteKeys[keysFilled] = new OneKeyboardKey(whiteVertices, sizeof(whiteVertices)/sizeof(whiteVertices[0]), whiteKeyIndicesR, sizeof(whiteKeyIndicesR)/sizeof(whiteKeyIndicesR[0]), shader, transform, organSoundPath, pianoSoundPath);
    whiteKeys[keysFilled]->setMaterialProperties(WHITE_A, WHITE_D, WHITE_S, SPECULAR_EXPONENT);
} // KeyboardKeys::makeWhiteKeyR(int, Shader*, Transform, std::string)

//--------------------------------------------------------------------------
/**
 Creates a white key with left notch.
 
 @param keysFilled Index of the whiteKeys array to add
 the key.
 @param shader Pointer to the shader, used to update the
 material property uniforms in the shader.
 @param transform The model matrix for the key.
 @param keyName the name of the key, used for getting the
 audio file for the key.
 */
void KeyboardKeys::makeWhiteKeyL(int keysFilled, Shader* shader, Transform transform, std::string keyName)
{
    std::string organSoundPath = resFolder + ORGAN_FOLDER + keyName + SOUND_EXTENSION;
    std::string pianoSoundPath = resFolder + PIANO_FOLDER + keyName + SOUND_EXTENSION;
    whiteKeys[keysFilled] = new OneKeyboardKey(whiteVertices, sizeof(whiteVertices)/sizeof(whiteVertices[0]), whiteKeyIndicesL, sizeof(whiteKeyIndicesL)/sizeof(whiteKeyIndicesL[0]), shader, transform, organSoundPath, pianoSoundPath);
    whiteKeys[keysFilled]->setMaterialProperties(WHITE_A, WHITE_D, WHITE_S, SPECULAR_EXPONENT);
} // KeyboardKeys::makeWhiteKeyL(int, Shader*, Transform, std::string)

//--------------------------------------------------------------------------
/**
 Creates a white key with both a left and right notch.
 
 @param keysFilled Index of the whiteKeys array to add
 the key.
 @param shader Pointer to the shader, used to update the
 material property uniforms in the shader.
 @param transform The model matrix for the key.
 @param keyName the name of the key, used for getting the
 audio file for the key.
 */
void KeyboardKeys::makeWhiteKeyLR(int keysFilled, Shader* shader, Transform transform, std::string keyName)
{
    std::string organSoundPath = resFolder + ORGAN_FOLDER + keyName + SOUND_EXTENSION;
    std::string pianoSoundPath = resFolder + PIANO_FOLDER + keyName + SOUND_EXTENSION;
    whiteKeys[keysFilled] = new OneKeyboardKey(whiteVertices, sizeof(whiteVertices)/sizeof(whiteVertices[0]), whiteKeyIndicesLR, sizeof(whiteKeyIndicesLR)/sizeof(whiteKeyIndicesLR[0]), shader, transform, organSoundPath, pianoSoundPath);
    whiteKeys[keysFilled]->setMaterialProperties(WHITE_A, WHITE_D, WHITE_S, SPECULAR_EXPONENT);
} // KeyboardKeys::makeWhiteKeyLR(int, Shader*, Transform, std::string)
