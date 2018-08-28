# Virtual Keyboard
Virtual Keyboard is a simulation of a big electric piano rendered in 3D using C++ and OpenGL (with custom shaders in GLSL).

## Getting Started
To get started, you need two folders of files in the "/res/" folder:

- 88 files of organ sounds in ".aiff" file format in the "/res/organ_sounds/" folder, and
- 88 files of piano sounds in ".aiff" file format in the "/res/piano_sounds/" folder.

The files should be named as follows:

- All are in the format [octave][letter][the flat symbol "b" if the note is flat].aiff.
- The first notes are as follows: 0a.aiff, 0bb.aiff, 0b.aiff, 1c.aiff, 1db.aiff...

These files cannot be provided due to licensing issues.

When running the application, enter the path to "/res/" on your machine in the console as prompted.

## Using the Application
Key controls:

- <kbd>W</kbd><kbd>A</kbd><kbd>S</kbd><kbd>D</kbd> keys moves you around the world, pressing keys down when you move over them
- The mouse/trackpad can be used to look around,
- <kbd>F</kbd> toggles fullscreen mode,
- <kbd>ESC</kbd> exits fullscreen mode or the application,
- <kbd>K</kbd> switches between organ and piano sound modes.
