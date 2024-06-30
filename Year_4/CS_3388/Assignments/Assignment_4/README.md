# Description

This OpenGL program reads from 10 ply and bmp files to recreate objects in the environment.
Uses VAOs and VBOs to render objects.

## How it was made

1. Each textured mesh has its own VAO and 3 VBOs, one for element vertices, texture coordinates, and indices.
2. Each textured mesh has its own vertex and fragment shaders.
3. Added `
#define GLM_ENABLE_EXPERIMENTAL
#define _CRT_SECURE_NO_WARNINGS`
   to the top of the file to prevent some errors.
4. Made with Microsoft Visual Studio on Windows but should run on any environment once compiled.

### Bugs

1. Slight jitter when the camera starts moving left/right with the arrow keys

## How to run

1. Run the code/executable.
2. Make sure the following files are present at the same directory level as the code/executable file.

## Dependencies

1. glew
2. GLFW
3. glm

## How to move around the scene

1. Up and down arrows to move forward and back
2. Left and right arrows to turn to the left and right

## Screenshots

How to recreate them

1. Screenshot 1. Do nothing. This is the default view.
2. Turn 90 degrees to the left by holding the left arrow key for 1 second.
3. Turn 90 degrees to the right by holding the right arrow key for 1 second.
