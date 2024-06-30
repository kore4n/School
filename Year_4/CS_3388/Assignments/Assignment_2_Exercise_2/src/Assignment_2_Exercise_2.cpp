#include <GLFW/glfw3.h>
#include <iostream>
#include <stdlib.h>

void setup() {
    // Make background white
    glClearColor(1, 1, 1, 1);
    // Make polygons black
    glColor3f(0, 0, 0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.1, 1.1, -1.1, 1.1, -1, 1);

}

static inline double frand() {
    double x = ((double)rand()) / (double)RAND_MAX;
    if (rand() % 2) {
        x *= -1.0;
    }
    return x;
}

int generateNegativeOneOrOne() {
    int number = rand();

    if (number % 2) {
        return -1;
    }

    return 1;
}

// Given x and y of a corner, returns a corner not opposite to it
static int* generateCornerNotOppositeTo(int baseCornerX, int baseCornerY) {
    int x = generateNegativeOneOrOne();
    int y = generateNegativeOneOrOne();

    // Case 1. Generated a corner the same as the corner given. Regenerate.
    // Case 2. Generated a corner opposite to the corner given. Regenerate.
    while (x == -baseCornerX && y == -baseCornerY) {
        x = generateNegativeOneOrOne();
        y = generateNegativeOneOrOne();
    }

    int *corner = new int[2];
    corner[0] = x;
    corner[1] = y;
    
    return corner;
}

// Draw all points
void drawPoints(int N) {
    // Generate first corner and point.
    int lastCornerX = generateNegativeOneOrOne();
    int lastCornerY = generateNegativeOneOrOne();
    double lastPointX = frand();
    double lastPointY = frand();

    for (int i = 1; i < N; i++) {
        int* newCorner = generateCornerNotOppositeTo(lastCornerX, lastCornerY);

        int newCornerX = newCorner[0];
        int newCornerY = newCorner[1];

        double halfX = (lastPointX + newCornerX) / 2;
        double halfY = (lastPointY + newCornerY) / 2;

        glVertex2f(halfX, halfY);

        // Update last corner and last point values
        lastCornerX = newCornerX;
        lastCornerY = newCornerY;

        lastPointX = halfX;
        lastPointY = halfY;
        
        delete newCorner;
        newCorner = NULL;
    }
}

int main(int argc, char* argv[]) {
    // Get arguments
    int N = atoi(argv[1]);
    int screenWidth = atoi(argv[2]);
    int screenHeight = atoi(argv[3]);

    //std::cout << N << std::endl;

    GLFWwindow* window;

    if (!glfwInit())
        return -1;
    window = glfwCreateWindow(screenWidth, screenHeight, "Assignment_2_Exercise_2", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);


    setup();
    glClear(GL_COLOR_BUFFER_BIT);


    glPointSize(2.0f);
    glBegin(GL_POINTS);

    drawPoints(N);

    glEnd();

    glfwSwapBuffers(window);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}