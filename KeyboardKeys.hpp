/**
 KeyboardKeys.hpp
 Virtual Keyboard
 This class creates and stores arrays of both white and
 black keyboard keys. It gives keys all the vertices, the
 material properties, and keyboard sounds.
 It also has useful functions for detecting the state of the
 keys, and moving the keys appropriately.
 
 @author Graeme Zinck
 @version 1.0 3/28/2018
*/

#ifndef KeyboardKeys_hpp
#define KeyboardKeys_hpp

#include "OneKeyboardKey.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include <glm/glm.hpp>
#include <string>

class KeyboardKeys
{
public:
    KeyboardKeys(Shader* shader, std::string resourceFolder); // Constructor
    virtual ~KeyboardKeys(); // Destructor
    
    void nextSoundSetting(); // Set whether use piano or organ sound
    void draw(Camera* camera);
    bool aKeyIsGoingDown();
    bool keyIsDown(int key);
    bool keyIsMoving();
    int getSelectedKey(glm::vec3 position);
    void keyDown(int key);
    void keyUp(int key);
private:
    // Helper methods to create new white keys
    void makeBlackKey(int keysFilled, Shader* shader, Transform transform, std::string keyName);
    void makeWhiteKey(int keysFilled, Shader* shader, Transform transform, std::string keyName);
    void makeWhiteKeyR(int keysFilled, Shader* shader, Transform transform, std::string keyName);
    void makeWhiteKeyL(int keysFilled, Shader* shader, Transform transform, std::string keyName);
    void makeWhiteKeyLR(int keysFilled, Shader* shader, Transform transform, std::string keyName);
    
    // Constants
    const static unsigned int NUM_WHITE_KEYS = 52; // Number of white keys on keyboard
    const static unsigned int NUM_BLACK_KEYS = 36; // Number of black keys on keyboard
    const static unsigned int NUM_OCTAVES = 7; // Number of complete octaves on keyboard
    const static unsigned int NUM_KEYS_IN_OCTAVE = 7; // Number of WHITE keys in octave
    const static unsigned int NUM_WHITE_VERTICES = 92; // Number of WHITE vertices specified
    const static unsigned int NUM_BLACK_VERTICES = 30; // Number of BLACK vertices specified
    const double X_DIFF_BETWEEN_WHITE_KEYS = 2.4; // How far apart white keys are
    const int DELAY = 10; // Milliseconds between movements of pushing down a key
    const int KEY_UP_DELAY = 8; // Milliseconds between movements of pulling up a key
    
    // Material properties
    const float SPECULAR_EXPONENT = 1000; // Shininess of a key
    const glm::vec3 WHITE_A = glm::vec3(1, 1, 1); // WHITE ambient light coefficients
    const glm::vec3 WHITE_D = glm::vec3(1, 1, 1); // WHITE diffuse
    const glm::vec3 WHITE_S = glm::vec3(1, 1, 1); // WHITE specular
    const glm::vec3 BLACK_A = glm::vec3(0, 0, 0);
    const glm::vec3 BLACK_D = glm::vec3(0.05, 0.05, 0.05);
    const glm::vec3 BLACK_S = glm::vec3(1, 1, 1);
    
    // Where to find all the sound files
    const std::string SOUND_EXTENSION = ".aiff";
    const std::string ORGAN_FOLDER = "/organ_sounds/";
    const std::string PIANO_FOLDER = "/piano_sounds/";
    std::string resFolder;
    
    // Holds an array of all the white and black keys
    OneKeyboardKey** whiteKeys;
    OneKeyboardKey** blackKeys;
    
    // Index of the key that is currently down
    int m_curKeyDown;
    
    // Hold which sound to use
    unsigned int m_soundToUse; // 0 for organ, 1 for piano.
    
    // Holds all the white vertices
    Vertex whiteVertices[NUM_WHITE_VERTICES] =
    {
        //----------------------------------
        // Vertices: Front to back
        //----------------------------------
        // z = 0.2
        Vertex(glm::vec3(0.0, 1.7, 0.2), glm::vec3(0, 0, 1)), // 0
        Vertex(glm::vec3(0.1, 1.9, 0.2), glm::vec3(0, 0, 1)), // 1
        Vertex(glm::vec3(2.1, 1.9, 0.2), glm::vec3(0, 0, 1)), // 2
        Vertex(glm::vec3(2.2, 1.7, 0.2), glm::vec3(0, 0, 1)), // 3
        // z = 0.0
        Vertex(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, 0, 1)), // 4
        Vertex(glm::vec3(0.0, 1.7, 0.0), glm::vec3(0, 0, 1)), // 5
        Vertex(glm::vec3(2.2, 1.7, 0.0), glm::vec3(0, 0, 1)), // 6
        Vertex(glm::vec3(2.2, 0.0, 0.0), glm::vec3(0, 0, 1)), // 7
        // z = -5.0
        Vertex(glm::vec3(0.0, 0.0, -5.0), glm::vec3(0, 0, -1)), // 8
        Vertex(glm::vec3(0.0, 1.7, -5.0), glm::vec3(0, 0, -1)), // 9
        Vertex(glm::vec3(0.1, 1.9, -5.0), glm::vec3(0, 0, -1)), // 10
        Vertex(glm::vec3(0.7, 1.9, -5.0), glm::vec3(0, 0, -1)), // 11
        Vertex(glm::vec3(1.5, 1.9, -5.0), glm::vec3(0, 0, -1)), // 12
        Vertex(glm::vec3(2.1, 1.9, -5.0), glm::vec3(0, 0, -1)), // 13
        Vertex(glm::vec3(2.2, 1.7, -5.0), glm::vec3(0, 0, -1)), // 14
        Vertex(glm::vec3(2.2, 0.0, -5.0), glm::vec3(0, 0, -1)), // 15
        Vertex(glm::vec3(1.5, 0.0, -5.0), glm::vec3(0, 0, -1)), // 16
        Vertex(glm::vec3(0.7, 0.0, -5.0), glm::vec3(0, 0, -1)), // 17
        // z = -15.0
        Vertex(glm::vec3(0.0, 0.0, -15.0), glm::vec3(0, 0, -1)), // 18
        Vertex(glm::vec3(0.0, 1.7, -15.0), glm::vec3(0, 0, -1)), // 19
        Vertex(glm::vec3(0.1, 1.9, -15.0), glm::vec3(0, 0, -1)), // 20
        Vertex(glm::vec3(0.7, 1.9, -15.0), glm::vec3(0, 0, -1)), // 21
        Vertex(glm::vec3(1.5, 1.9, -15.0), glm::vec3(0, 0, -1)), // 22
        Vertex(glm::vec3(2.1, 1.9, -15.0), glm::vec3(0, 0, -1)), // 23
        Vertex(glm::vec3(2.2, 1.7, -15.0), glm::vec3(0, 0, -1)), // 24
        Vertex(glm::vec3(2.2, 0.0, -15.0), glm::vec3(0, 0, -1)), // 25
        Vertex(glm::vec3(1.5, 0.0, -15.0), glm::vec3(0, 0, -1)), // 26
        Vertex(glm::vec3(0.7, 0.0, -15.0), glm::vec3(0, 0, -1)), // 27
        //----------------------------------
        // Vertices: Left to Right
        //----------------------------------
        // x = 0.0
        Vertex(glm::vec3(0.0, 0.0, -15.0), glm::vec3(-1, 0, 0)), // 28
        Vertex(glm::vec3(0.0, 1.7, -15.0), glm::vec3(-1, 0, 0)), // 29
        Vertex(glm::vec3(0.0, 1.7, -5.0), glm::vec3(-1, 0, 0)), // 30
        Vertex(glm::vec3(0.0, 1.7, 0.0), glm::vec3(-1, 0, 0)), // 31
        Vertex(glm::vec3(0.0, 1.7, 0.2), glm::vec3(-1, 0, 0)), // 32 (not used, but for legacy purposes...)
        Vertex(glm::vec3(0.0, 0.0, 0.0), glm::vec3(-1, 0, 0)), // 33
        Vertex(glm::vec3(0.0, 0.0, -5.0), glm::vec3(-1, 0, 0)), // 34
        // x = 0.1
        Vertex(glm::vec3(0.1, 1.9, -15.0), glm::vec3(-1, 0, 0)), // 35 (not used, but for legacy purposes...)
        Vertex(glm::vec3(0.1, 1.9, -5.0), glm::vec3(-1, 0, 0)), // 36 (not used, but for legacy purposes...)
        Vertex(glm::vec3(0.1, 1.9, 0.2), glm::vec3(-1, 0, 0)), // 37 (not used, but for legacy purposes...)
        // x = 0.7
        Vertex(glm::vec3(0.7, 0.0, -15.0), glm::vec3(-1, 0, 0)), // 38
        Vertex(glm::vec3(0.7, 1.9, -15.0), glm::vec3(-1, 0, 0)), // 39
        Vertex(glm::vec3(0.7, 1.9, -5.0), glm::vec3(-1, 0, 0)), // 40
        Vertex(glm::vec3(0.7, 0.0, -5.0), glm::vec3(-1, 0, 0)), // 41
        // x = 1.5
        Vertex(glm::vec3(1.5, 0.0, -15.0), glm::vec3(1, 0, 0)), // 42
        Vertex(glm::vec3(1.5, 1.9, -15.0), glm::vec3(1, 0, 0)), // 43
        Vertex(glm::vec3(1.5, 1.9, -5.0), glm::vec3(1, 0, 0)), // 44
        Vertex(glm::vec3(1.5, 0.0, -5.0), glm::vec3(1, 0, 0)), // 45
        // x = 2.1
        Vertex(glm::vec3(2.1, 1.9, -15.0), glm::vec3(1, 0, 0)), // 46 (not used, but for legacy purposes...)
        Vertex(glm::vec3(2.1, 1.9, -5.0), glm::vec3(1, 0, 0)), // 47 (not used, but for legacy purposes...)
        Vertex(glm::vec3(2.1, 1.9, 0.2), glm::vec3(1, 0, 0)), // 48 (not used, but for legacy purposes...)
        // x = 2.2
        Vertex(glm::vec3(2.2, 0.0, -15.0), glm::vec3(1, 0, 0)), // 49
        Vertex(glm::vec3(2.2, 1.7, -15.0), glm::vec3(1, 0, 0)), // 50
        Vertex(glm::vec3(2.2, 1.7, -5.0), glm::vec3(1, 0, 0)), // 51
        Vertex(glm::vec3(2.2, 1.7, 0.0), glm::vec3(1, 0, 0)), // 52
        Vertex(glm::vec3(2.2, 1.7, 0.2), glm::vec3(1, 0, 0)), // 53 (not used, but for legacy purposes...)
        Vertex(glm::vec3(2.2, 0.0, 0.0), glm::vec3(1, 0, 0)), // 54
        Vertex(glm::vec3(2.2, 0.0, -5.0), glm::vec3(1, 0, 0)), // 55
        //----------------------------------
        // Vertices: Top to Bottom
        //----------------------------------
        // y = 1.9
        Vertex(glm::vec3(0.1, 1.9, 0.2), glm::vec3(0, 1, 0)), // 56
        Vertex(glm::vec3(0.1, 1.9, -5.0), glm::vec3(0, 1, 0)), // 57
        Vertex(glm::vec3(0.1, 1.9, -15.0), glm::vec3(0, 1, 0)), // 58
        Vertex(glm::vec3(0.7, 1.9, -15.0), glm::vec3(0, 1, 0)), // 59
        Vertex(glm::vec3(1.5, 1.9, -15.0), glm::vec3(0, 1, 0)), // 60
        Vertex(glm::vec3(2.1, 1.9, -15.0), glm::vec3(0, 1, 0)), // 61
        Vertex(glm::vec3(2.1, 1.9, -5.0), glm::vec3(0, 1, 0)), // 62
        Vertex(glm::vec3(2.1, 1.9, 0.2), glm::vec3(0, 1, 0)), // 63
        Vertex(glm::vec3(0.7, 1.9, -5.0), glm::vec3(0, 1, 0)), // 64
        Vertex(glm::vec3(1.5, 1.9, -5.0), glm::vec3(0, 1, 0)), // 65
        // y = 0.0
        Vertex(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, -1, 0)), // 66
        Vertex(glm::vec3(0.0, 0.0, -5.0), glm::vec3(0, -1, 0)), // 67
        Vertex(glm::vec3(0.0, 0.0, -15.0), glm::vec3(0, -1, 0)), // 68
        Vertex(glm::vec3(0.7, 0.0, -15.0), glm::vec3(0, -1, 0)), // 69
        Vertex(glm::vec3(1.5, 0.0, -15.0), glm::vec3(0, -1, 0)), // 70
        Vertex(glm::vec3(2.2, 0.0, -15.0), glm::vec3(0, -1, 0)), // 71
        Vertex(glm::vec3(2.2, 0.0, -5.0), glm::vec3(0, -1, 0)), // 72
        Vertex(glm::vec3(2.2, 0.0, 0.0), glm::vec3(0, -1, 0)), // 73
        Vertex(glm::vec3(0.7, 0.0, -5.0), glm::vec3(0, -1, 0)), // 74
        Vertex(glm::vec3(1.5, 0.0, -5.0), glm::vec3(0, -1, 0)), // 75
        //----------------------------------
        // Vertices: Sides
        //----------------------------------
        // Left Side
        Vertex(glm::vec3(0.0, 1.7, 0.2), glm::vec3(-2.0/3.0, 1.0/3.0, 0)), // 76
        Vertex(glm::vec3(0.0, 1.7, -5.0), glm::vec3(-2.0/3.0, 1.0/3.0, 0)), // 77
        Vertex(glm::vec3(0.0, 1.7, -15.0), glm::vec3(-2.0/3.0, 1.0/3.0, 0)), // 78
        Vertex(glm::vec3(0.1, 1.9, -15.0), glm::vec3(-2.0/3.0, 1.0/3.0, 0)), // 79
        Vertex(glm::vec3(0.1, 1.9, -5.0), glm::vec3(-2.0/3.0, 1.0/3.0, 0)), // 80
        Vertex(glm::vec3(0.1, 1.9, 0.2), glm::vec3(-2.0/3.0, 1.0/3.0, 0)), // 81
        // Right side
        Vertex(glm::vec3(2.1, 1.9, 0.2), glm::vec3(2.0/3.0, 1.0/3.0, 0)), // 82
        Vertex(glm::vec3(2.1, 1.9, -5.0), glm::vec3(2.0/3.0, 1.0/3.0, 0)), // 83
        Vertex(glm::vec3(2.1, 1.9, -15.0), glm::vec3(2.0/3.0, 1.0/3.0, 0)), // 84
        Vertex(glm::vec3(2.2, 1.7, -15.0), glm::vec3(2.0/3.0, 1.0/3.0, 0)), // 85
        Vertex(glm::vec3(2.2, 1.7, -5.0), glm::vec3(2.0/3.0, 1.0/3.0, 0)), // 86
        Vertex(glm::vec3(2.2, 1.7, 0.2), glm::vec3(2.0/3.0, 1.0/3.0, 0)), // 87
        //----------------------------------
        // Forgotten vertices: Top to Bottom y = 1.7
        //----------------------------------
        Vertex(glm::vec3(0.0, 1.7, 0.0), glm::vec3(0, -1, 0)), // 88
        Vertex(glm::vec3(2.2, 1.7, 0.0), glm::vec3(0, -1, 0)), // 89
        Vertex(glm::vec3(2.2, 1.7, 0.2), glm::vec3(0, -1, 0)), // 90
        Vertex(glm::vec3(0.0, 1.7, 0.2), glm::vec3(0, -1, 0)), // 91
    };
    
    // Indices for the typical white key
    unsigned int whiteKeyIndices[66] =
    {
        // Front I
        0, 3, 2, 0, 2, 1,
        // Front II
        4, 7, 6, 4, 6, 5,
        // Front III N/A
        // Front IV
        25, 18, 19, 25, 19, 20,
        25, 20, 23, 25, 23, 24,
        // Left I
        28, 33, 31, 28, 31, 29,
        // Left II, III, IV, V N/A
        // Left VI
        54, 49, 50, 54, 50, 52,
        // Top I
        56, 63, 61, 56, 61, 58,
        // Top II
        91, 88, 89, 91, 89, 90,
        // Top III
        66, 68, 71, 66, 71, 73,
        // SIDE L
        78, 76, 81, 78, 81, 79,
        // SIDE R
        82, 87, 85, 82, 85, 84,
    };
    
    // Indices for the white key with a notch in the left side
    unsigned int whiteKeyIndicesL[90] =
    {
        // Front I
        0, 3, 2, 0, 2, 1,
        // Front II
        4, 7, 6, 4, 6, 5,
        // Front III
        8, 9, 17, 17, 9, 10,
        17, 10, 11,
        // Front IV
        27, 21, 23, 27, 23, 24,
        27, 24, 25,
        // Left I
        34, 33, 31, 34, 31, 30,
        // Left II N/A
        // Left III
        38, 41, 40, 38, 40, 39,
        // Left IV, V N/A
        // Left VI
        54, 49, 50, 54, 50, 52,
        // Top I
        56, 63, 62, 56, 62, 57,
        64, 62, 61, 64, 61, 59,
        // Top II
        91, 88, 89, 91, 89, 90,
        // Top III
        66, 67, 72, 66, 72, 73,
        74, 69, 71, 75, 71, 72,
        // SIDE L
        77, 76, 81, 77, 81, 80,
        // SIDE R
        82, 87, 85, 82, 85, 84,
    };
    
    // Indices for the white key with a notch in the right side
    unsigned int whiteKeyIndicesR[90] =
    {
        // Front I
        0, 3, 2, 0, 2, 1,
        // Front II
        4, 7, 6, 4, 6, 5,
        // Front III
        16, 12, 13, 16, 13, 14,
        16, 14, 15,
        // Front IV
        18, 19, 20, 18, 20, 22,
        18, 22, 26,
        // Left I
        28, 33, 31, 28, 31, 29,
        // Left II, III N/A
        // Left IV
        42, 43, 44, 42, 44, 45,
        // Left V N/A
        // Left VI
        55, 51, 52, 55, 52, 54,
        // Top I
        56, 63, 62, 56, 62, 57,
        57, 65, 60, 57, 60, 58,
        // Top II
        91, 88, 89, 91, 89, 90,
        // Top III
        66, 67, 72, 66, 72, 73,
        67, 68, 70, 67, 70, 75,
        // SIDE L
        78, 76, 81, 78, 81, 79,
        // SIDE R
        82, 87, 86, 82, 86, 83,
    };
    
    // Indices for the white key with a notch in both the left and right sides
    unsigned int whiteKeyIndicesLR[102] =
    {
        // Front I
        0, 3, 2, 0, 2, 1,
        // Front II
        4, 7, 6, 4, 6, 5,
        // Front III
        16, 12, 13, 16, 13, 14,
        16, 14, 15, 8, 11, 17,
        8, 9, 10, 8, 10, 11,
        // Front IV
        27, 21, 22, 27, 22, 26,
        // Left I
        34, 33, 31, 34, 31, 30,
        // Left II N/A
        // Left III
        38, 41, 40, 38, 40, 39,
        // Left IV
        42, 43, 44, 42, 44, 45,
        // Left V N/A
        // Left VI
        55, 51, 52, 55, 52, 54,
        // Top I
        56, 63, 62, 56, 62, 57,
        64, 65, 60, 64, 60, 59,
        // Top II
        91, 88, 89, 91, 89, 90,
        // Top III
        66, 67, 72, 66, 72, 73,
        74, 69, 70, 74, 70, 75,
        // SIDE L
        77, 76, 81, 77, 81, 80,
        // SIDE R
        82, 87, 86, 82, 86, 83,
    };
    
    // All the vertices for a black key
    Vertex blackVertices[NUM_BLACK_VERTICES] =
    {
        // Front I
        Vertex(glm::vec3(1.7, 1.9, -5.2), glm::vec3(0, 0, 1)), // 0
        Vertex(glm::vec3(1.7, 0.0, -5.2), glm::vec3(0, 0, 1)), // 1
        Vertex(glm::vec3(2.9, 0.0, -5.2), glm::vec3(0, 0, 1)), // 2
        Vertex(glm::vec3(2.9, 1.9, -5.2), glm::vec3(0, 0, 1)), // 3
        // Front II
        Vertex(glm::vec3(1.7, 0.0, -15), glm::vec3(0, 0, -1)), // 4
        Vertex(glm::vec3(2.9, 0.0, -15), glm::vec3(0, 0, -1)), // 5
        Vertex(glm::vec3(2.9, 3.0, -15), glm::vec3(0, 0, -1)), // 6
        Vertex(glm::vec3(1.7, 3.0, -15), glm::vec3(0, 0, -1)), // 7
        // Left I
        Vertex(glm::vec3(1.7, 0.0, -15), glm::vec3(-1, 0, 0)), // 8
        Vertex(glm::vec3(1.7, 0.0, -5.2), glm::vec3(-1, 0, 0)), // 9
        Vertex(glm::vec3(1.7, 1.9, -5.2), glm::vec3(-1, 0, 0)), // 10
        Vertex(glm::vec3(1.7, 3.0, -5.9), glm::vec3(-1, 0, 0)), // 11
        Vertex(glm::vec3(1.7, 3.0, -15), glm::vec3(-1, 0, 0)), // 12
        // Left II
        Vertex(glm::vec3(2.9, 0.0, -15), glm::vec3(1, 0, 0)), // 13
        Vertex(glm::vec3(2.9, 0.0, -5.2), glm::vec3(1, 0, 0)), // 14
        Vertex(glm::vec3(2.9, 1.9, -5.2), glm::vec3(1, 0, 0)), // 15
        Vertex(glm::vec3(2.9, 3.0, -5.9), glm::vec3(1, 0, 0)), // 16
        Vertex(glm::vec3(2.9, 3.0, -15), glm::vec3(1, 0, 0)), // 17
        // Top I
        Vertex(glm::vec3(1.7, 3.0, -5.9), glm::vec3(0, 1, 0)), // 18
        Vertex(glm::vec3(2.9, 3.0, -5.9), glm::vec3(0, 1, 0)), // 19
        Vertex(glm::vec3(2.9, 3.0, -15), glm::vec3(0, 1, 0)), // 20
        Vertex(glm::vec3(1.7, 3.0, -15), glm::vec3(0, 1, 0)), // 21
        // Top II
        Vertex(glm::vec3(1.7, 0.0, -5.2), glm::vec3(0, -1, 0)), // 22
        Vertex(glm::vec3(2.9, 0.0, -5.2), glm::vec3(0, -1, 0)), // 23
        Vertex(glm::vec3(2.9, 0.0, -15), glm::vec3(0, -1, 0)), // 24
        Vertex(glm::vec3(1.7, 0.0, -15), glm::vec3(0, -1, 0)), // 25
        // Slant
        Vertex(glm::vec3(1.7, 1.9, -5.2), glm::vec3(0.0, 0.7, 1.1)), // 26
        Vertex(glm::vec3(2.9, 1.9, -5.2), glm::vec3(0.0, 0.7, 1.1)), // 27
        Vertex(glm::vec3(2.9, 3.0, -5.9), glm::vec3(0.0, 0.7, 1.1)), // 28
        Vertex(glm::vec3(1.7, 3.0, -5.9), glm::vec3(0.0, 0.7, 1.1)), // 29
    };
    
    // Indices for a black key
    unsigned int blackKeyIndices[48] =
    {
        // Front I
        0, 1, 2, 0, 2, 3,
        // Front II
        4, 7, 6, 4, 6, 5,
        // Left I
        8, 9, 10, 8, 10, 11,
        8, 11, 12,
        // Left II
        13, 17, 16, 13, 16, 15,
        13, 15, 14,
        // Top I
        18, 19, 20, 18, 20, 21,
        // Top II
        22, 25, 24, 22, 24, 23,
        // Slant
        26, 27, 28, 26, 28, 29,
    };
}; // KeyboardKeys

#endif /* KeyboardKeys_hpp */
