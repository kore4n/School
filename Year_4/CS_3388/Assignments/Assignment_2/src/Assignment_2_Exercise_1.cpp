#define _USE_MATH_DEFINES

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <GLFW/glfw3.h>

void setup() {
    // Set background color to white
    glClearColor(1, 1, 1, 1);
    // Set line colour to black
    glColor3f(0, 0, 0);

    // Set up orthogonal projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 60, 0, 60, -1, 1);
}

// Read points from file
std::vector<float>* readPoints() {
    std::vector<float> *points = new std::vector<float>;

    std::ifstream myfile; 
    myfile.open("dog.txt");
    //std::cout << "Tried opening file";

    std::string mystring;
    if (myfile.is_open()) { // always check whether the file is open
        //std::cout << "Opened file";
        while (myfile.good()) {
            myfile >> mystring; // pipe file's content into stream

            float floatStr = std::stof(mystring);

            //std::cout << mystring << "*" << floatStr << "|"; // pipe stream's content to standard output

            points->push_back(floatStr);
        }
    }

    return points;
}

void drawDog(std::vector<float> *points) {
    glBegin(GL_LINE_STRIP);

    for (int i = 1; i < points->size(); i = i + 2) {
        float x = (*points)[i - 1];
        float y = (*points)[i];
        glVertex2f(x, y);
    }
    glEnd();
}

void drawDogOnCircleAt(int radius, float radiansClockwiseOnCircle, int rotationDegrees, std::vector<float>*points) {
    float x = sin(radiansClockwiseOnCircle) * radius;
    float y = cos(radiansClockwiseOnCircle) * radius;

    glLoadIdentity();
    glTranslatef(30, 30, 0);
    glTranslatef(x, y, 0);
    glRotatef(rotationDegrees, 0, 0, 1);
    drawDog(points);
}

int main(int argc, char** argv)
{
    // Read points from file
    std::vector<float> *points = readPoints();

    // Current rotation of all dogs
    int rotationDegrees = 0;

    // Radius of circle dogs should lie on
    const int radius = 25;

    GLFWwindow* window;

    if (!glfwInit())
        return -1;
    window = glfwCreateWindow(1200, 800, "Assignment_2", NULL, NULL);
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

        // Draw 8 dogs on circle at 45 degree intervals
        for (int degreesCCWOnCircle = 0; degreesCCWOnCircle < 360; degreesCCWOnCircle += 45) {
            float radiansCCWOnCircle = degreesCCWOnCircle * M_PI / 180;

            drawDogOnCircleAt(radius, radiansCCWOnCircle, rotationDegrees, points);
        }

        rotationDegrees += 1;
        rotationDegrees = rotationDegrees % 360;

        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}
