# Assignment 6 (Computer Graphics)

# Description

1. This OpenGL program creates a plane of vertices that move according to Gerstner waves.
2. Utilizes tessellation and phong shading.
3. Displacement map is used to add offset the vertical height of vertices, and scrolling textures are used to move the water texture.

## How it was made

1. Added `
#define GLM_ENABLE_EXPERIMENTAL` and `
#define _CRT_SECURE_NO_WARNINGS`
   to the top of the file to prevent some errors.
2. Made with Microsoft Visual Studio on Windows but should run on any environment once compiled.

## How to run

1. Make sure the following files are present at the same directory level as the code/executable file.
   - `Assignment_6.cpp`
   - `PlaneMesh.hpp`
   - `WaterVertexShader.glsl`
   - `WaterTessellationControlShader.glsl`
   - `WaterTessellationEvaluationShader.glsl`
   - `WaterGeometryShader.glsl`
   - `WaterFragmentShader.glsl`
   - `water.bmp`
   - `displacement-map1.bmp`
   - `boat.bmp`
   - `boat.ply`
   - `eyes.bmp`
   - `eyes.ply`
   - `head.head`
   - `head.ply`
2. Command line arguments are in the following order:
   - Screen width (default 1500)
   - Screen height (default 1500)
   - Step value (default 1)
3. Run the code/executable.

## Dependencies

1. glew
2. GLFW
3. glm

## How to move around the scene

1. Click and drag the mouse1 button to rotate the camera.
2. Up and down arrow keys or scroll wheel to zoom in and out of the scene.
