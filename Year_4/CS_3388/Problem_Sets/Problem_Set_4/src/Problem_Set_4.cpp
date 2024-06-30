// Your First C++ Program

#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#define VERTICES 22
#define FRAMES 400

float star[VERTICES] = { 0, 10, 2.5, 2.5, 10, 2.5, 4, -2.5, 7, -10, 0, -5, -7, -10, -4, -2.5, -10, 2.5, -2.5, 2.5, 0, 10 };
float decagon[VERTICES] = { 5, 15.388, 13.0902, 9.51056, 16.1803, 0, 13.0902, -9.51056, 5, -15.388, -5, -15.388, -13.0902, -9.51056, -16.1803, 0, -13.0902, 9.51056, -5, 15.388, 5, 15.388 };

void setup() {
    // Set background color to white
    glClearColor(1, 1, 1, 1);
    // Set line colour to black
    glColor3f(0, 0, 0);

    // Set up orthogonal projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-20, 20, -20, 20, -1, 1);
}

// 0 <= t <= 1 
void drawShape(float t) {
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < VERTICES; i += 2) {
        float tweenX = star[i] * (1 - t) + decagon[i] * t;
        float tweenY = star[i + 1] * (1 - t) + decagon[i + 1] * t;

        glVertex2f(tweenX, tweenY);
    }
    glEnd();
}

int main() {
    int t = 0;
    bool shouldIncrease = true;

    GLFWwindow* window;

    if (!glfwInit())
        return -1;
    window = glfwCreateWindow(1200, 800, "Problem_Set_4", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    setup();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);

        drawShape((float)t/400);

        shouldIncrease ? t++ : t--;

        if (t > 400) {
            t = 400;
            shouldIncrease = false;
        }

        if (t < 0) {
            t = 0;
            shouldIncrease = true;
        }

        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}
