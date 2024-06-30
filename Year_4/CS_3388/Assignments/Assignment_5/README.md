# Assignment 5 (Computer Graphics)

# Description

1. This OpenGL program creates a product based on a function that returns a 3d scalar field.
2. There are 3 scalar field functions, `f1`, `f2`, and `f3`.
3. Utilizes phong shading.

## How it was made

1. Added `
#define GLM_ENABLE_EXPERIMENTAL
#define _CRT_SECURE_NO_WARNINGS`
   to the top of the file to prevent some errors.
2. Made with Microsoft Visual Studio on Windows but should run on any environment once compiled.

## How to run

1. Make sure the files `DiffuseShader.fragmentshader` and `DiffuseShader.vertexshader` are present at the same directory level as the code/executable file.
2. Command line arguments are in the following order:
   - Screen width (default 1400)
   - Screen height (default 900)
   - Step value (default 0.1)
   - Minimum value for the grid (default -10)
   - Maximum value for the grid (default 10)
   - Isovalue (default 0)
   - Function to use (1-3 which uses `f1-f3`, default 1)
3. Run the code/executable.
4. The corresponding `.ply` files will be generated and placed in the same directory.

## Dependencies

1. glew
2. GLFW
3. glm

## How to move around the scene

1. Click and drag the mouse1 button to rotate the camera.
2. Up and down arrow keys or scroll wheel to zoom in and out of the scene.

## Other files

1. `WaveyThing.ply` output of running the program with function `f1`
2. `ConeyThing.ply` output of running the program with function `f2`
3. `SphereThing.ply` output of running the program with function `f3`
