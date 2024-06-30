// Your First C++ Program

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <deque>

#define POINT_SIZE 20.0f
#define POINTS_PER_BEZIER_CURVE 200

struct Point {
    float x;
    float y;
};

struct Node : Point {
    bool hasHandle1, hasHandle2;
    Point* handle1;
    Point* handle2;
};

int SCREEN_WIDTH;
int SCREEN_HEIGHT;

std::deque<Node*> nodes;

GLFWwindow* window;

void getCursorPos(double& mouseX, double& mouseY) {
    double x, y;

    glfwGetCursorPos(window, &x, &y);
    y = SCREEN_HEIGHT - y;

    mouseX = x;
    mouseY = y;
}

// Return first control points it's close to.
// Then, reveal control point of that pair
bool closeToControlPoint(Point* &controlPoint1, Point* &controlPoint2, Node* &parentNode) {
    double mouseX, mouseY;

    getCursorPos(mouseX, mouseY);

    for (int i = 0; i < nodes.size(); i++) {
        Node* currentNode = nodes[i];
        Point* handle1 = currentNode->handle1;

        double xLowerBoundHandle1 = handle1->x - POINT_SIZE;
        double xUpperBoundHandle1 = handle1->x + POINT_SIZE;

        double yLowerBoundHandle1 = handle1->y - POINT_SIZE;
        double yUpperBoundHandle1 = handle1->y + POINT_SIZE;

        Point* handle2 = currentNode->handle2;

        double xLowerBoundHandle2;
        double xUpperBoundHandle2;

        double yLowerBoundHandle2;
        double yUpperBoundHandle2;

        if (currentNode->hasHandle2) {
            xLowerBoundHandle2 = handle2->x - POINT_SIZE;
            xUpperBoundHandle2 = handle2->x + POINT_SIZE;

            yLowerBoundHandle2 = handle2->y - POINT_SIZE;
            yUpperBoundHandle2 = handle2->y + POINT_SIZE;
        }


        if (mouseX >= xLowerBoundHandle1 && mouseX <= xUpperBoundHandle1 && mouseY >= yLowerBoundHandle1 && mouseY <= yUpperBoundHandle1) {
            controlPoint1 = handle1;
            controlPoint2 = handle2;

            parentNode = currentNode;

            return true;
        }

        if (!currentNode->hasHandle2) continue;

        if (mouseX >= xLowerBoundHandle2 && mouseX <= xUpperBoundHandle2 && mouseY >= yLowerBoundHandle2 && mouseY <= yUpperBoundHandle2) {
            controlPoint1 = handle2;
            controlPoint2 = handle1;
            parentNode = currentNode;

            return true;
        }
    }

    return false;
}

bool closeToControlPoint() {
    double mouseX, mouseY;

    getCursorPos(mouseX, mouseY);

    for (int i = 0; i < nodes.size(); i++) {
        Node* currentNode = nodes[i];
        Point* handle1 = currentNode->handle1;

        double xLowerBoundHandle1 = handle1->x - POINT_SIZE;
        double xUpperBoundHandle1 = handle1->x + POINT_SIZE;

        double yLowerBoundHandle1 = handle1->y - POINT_SIZE;
        double yUpperBoundHandle1 = handle1->y + POINT_SIZE;

        Point* handle2 = currentNode->handle2;

        double xLowerBoundHandle2;
        double xUpperBoundHandle2;

        double yLowerBoundHandle2;
        double yUpperBoundHandle2;

        if (currentNode->hasHandle2) {
            xLowerBoundHandle2 = handle2->x - POINT_SIZE;
            xUpperBoundHandle2 = handle2->x + POINT_SIZE;

            yLowerBoundHandle2 = handle2->y - POINT_SIZE;
            yUpperBoundHandle2 = handle2->y + POINT_SIZE;
        }

        if (mouseX >= xLowerBoundHandle1 && mouseX <= xUpperBoundHandle1 && mouseY >= yLowerBoundHandle1 && mouseY <= yUpperBoundHandle1) {

            return true;
        }

        if (!currentNode->hasHandle2) continue;

        if (mouseX >= xLowerBoundHandle2 && mouseX <= xUpperBoundHandle2 && mouseY >= yLowerBoundHandle2 && mouseY <= yUpperBoundHandle2) {

            return true;
        }
    }

    return false;
}

Node* closeToNode(Node* &node) {
    double mouseX, mouseY;

    getCursorPos(mouseX, mouseY);


    for (int i = 0; i < nodes.size(); i++) {
        double xLowerBound = nodes[i]->x - POINT_SIZE;
        double xUpperBound = nodes[i]->x + POINT_SIZE;

        double yLowerBound = nodes[i]->y - POINT_SIZE;
        double yUpperBound = nodes[i]->y + POINT_SIZE;

        if (mouseX >= xLowerBound && mouseX <= xUpperBound && mouseY >= yLowerBound && mouseY <= yUpperBound) {
            //std::cout << "mouse is close to a node: " << std::endl;

            node = nodes[i];
            return nodes[i];
        }
    }

    return NULL;
}

void dragControlPoint(Point* controlPoint1, Point* controlPoint2, Node* nodeOfControlPointToDrag) {
    double mouseX, mouseY;

    getCursorPos(mouseX, mouseY);

    controlPoint1->x = mouseX;
    controlPoint1->y = mouseY;
    
    if (controlPoint2 == NULL) return;
    // If there is another control point, move that point along with us

    double controlPoint1ToNodeX = nodeOfControlPointToDrag->x - controlPoint1->x;
    double controlPoint1ToNodeY = nodeOfControlPointToDrag->y - controlPoint1->y;

    controlPoint2->x = nodeOfControlPointToDrag->x + controlPoint1ToNodeX;
    controlPoint2->y = nodeOfControlPointToDrag->y + controlPoint1ToNodeY;
}

// Drag node and its control points
void dragNode(Node* node) {
    double mouseX, mouseY;

    getCursorPos(mouseX, mouseY);

    Point* controlPoint1 = node->handle1;
    Point* controlPoint2 = node->handle2;

    double differenceCP1X = controlPoint1->x - node->x;
    double differenceCP1Y = controlPoint1->y - node->y;

    double differenceCP2X;
    double differenceCP2Y;

    if (node->hasHandle2) {
        differenceCP2X = controlPoint2->x - node->x;
        differenceCP2Y = controlPoint2->y - node->y;
    }

    node->x = mouseX;
    node->y = mouseY;

    // Move first control point 
    controlPoint1->x = mouseX + differenceCP1X;
    controlPoint1->y = mouseY + differenceCP1Y;
    
    if (!node->hasHandle2) return;

    // Node has a second control point, move that as well.
    controlPoint2->x = mouseX + differenceCP2X;
    controlPoint2->y = mouseY + differenceCP2Y;
}

// Enable antialiasing. NOTE: This turns all rectangular points circular.
void enableMSAA() {
    glfwWindowHint(GLFW_SAMPLES, 4);
    glEnable(GL_MULTISAMPLE);
}

void setup(int SCREEN_WIDTH, int SCREEN_HEIGHT) {
    // Set background color to white
    glClearColor(1, 1, 1, 1);
    // Set line colour to black
    glColor3f(0, 0, 0);

    glPointSize(POINT_SIZE);

    // Set up orthogonal projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, -1, 1);
}


void drawNodes() {
    glDisable(GL_POINT_SMOOTH);
    glColor3f(0, 0, 255);
    const double width = POINT_SIZE / 2;

    for (int i = 0; i < nodes.size(); i++) {
        double xpos = nodes[i]->x;
        double ypos = nodes[i]->y;

        //std::cout << "Drawing point at " << xpos << "," << ypos << std::endl;

        glBegin(GL_QUADS);
            glVertex2f(xpos - width, ypos - width);
            glVertex2f(xpos - width, ypos + width);
            glVertex2f(xpos + width, ypos + width);
            glVertex2f(xpos + width, ypos - width);
        glEnd();
    }
}

void drawControlPoints() {
    //glEnable(GL_POINT_SMOOTH);
    glColor3f(0, 0, 0);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_POINTS);

    for (int i = 0; i < nodes.size(); i++) {
        Node* currentNode = nodes[i];

        if (currentNode->hasHandle1) {
            glVertex2f(currentNode->handle1->x, currentNode->handle1->y);
        }
        if (currentNode->hasHandle2) {
            glVertex2f(currentNode->handle2->x, currentNode->handle2->y);
        }
    }
    glEnd();
}

void drawControlPointDottedLines() {
    glColor3f(0, 1, 1);
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(2, 0xAAAA);
    glLineWidth(2);
    glBegin(GL_LINES);
    for (int i = 0; i < nodes.size(); i++) {
        Node* currentNode = nodes[i];

        double handle1X = currentNode->handle1->x;
        double handle1Y = currentNode->handle1->y;

        glVertex2f(handle1X, handle1Y);

        if (!currentNode->hasHandle2) {
            glVertex2f(currentNode->x, currentNode->y);
        }
        else {
            Point* handle2 = currentNode->handle2;
            double handle2X = handle2->x;
            double handle2Y = handle2->y;

            glVertex2f(handle2X, handle2Y);
        }
    }
    glEnd();
    glDisable(GL_LINE_STIPPLE);
    glColor3f(0, 0, 0);
}

void drawNodesAndPoints() {
    drawNodes();
    drawControlPoints();
}

double cubicInterpolate(double pointA, double pointB, double pointC, double pointD, double t) {
    return pow((1 - t), 3) * pointA + 3 * pow((1 - t), 2) * t * pointB + 3 * (1 - t) * pow(t ,2) * pointC + pow(t, 3) * pointD;
}

// Draw one cubic bezier curve
void drawSingleCubicBezierCurve(Node* start, Node* end) {
    glLineWidth(5.0f);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_LINE_STRIP);

    for (int i = 0; i < POINTS_PER_BEZIER_CURVE; i++) {
        double t = (double)i / POINTS_PER_BEZIER_CURVE;
        //std::cout << "t: " << t << std::endl;

        
        double curX;
        double curY;
        if (start->hasHandle2) {
            curX = cubicInterpolate(start->x, start->handle2->x, end->handle1->x, end->x, t);
            curY = cubicInterpolate(start->y, start->handle2->y, end->handle1->y, end->y, t);
        }
        else {
            curX = cubicInterpolate(start->x, start->handle1->x, end->handle1->x, end->x, t);
            curY = cubicInterpolate(start->y, start->handle1->y, end->handle1->y, end->y, t);
        }

        glVertex2f(curX, curY);
    }

    glEnd();
}

void drawBezierCurves() {
    if (nodes.size() < 2) { return; }
    
    for (int i = 0; i < nodes.size() - 1; i++) {
        double xpos = nodes[i]->x;
        double ypos = nodes[i]->y;

        //std::cout << "Drawing point at " << xpos << "," << ypos << std::endl;

        drawSingleCubicBezierCurve(nodes[i], nodes[i + 1]);
    }
}

void addPoint() {
    double xpos, ypos;

    getCursorPos(xpos, ypos);

    Node *nodeToAdd = new Node();

    nodeToAdd->x = xpos;
    nodeToAdd->y = ypos;

    // Create control point 50px above
    Point* controlPoint = new Point();

    controlPoint->x = xpos;
    controlPoint->y = ypos + 50;

    nodeToAdd->handle1 = controlPoint;
    nodeToAdd->hasHandle1 = true;
    
    //std::cout << "Adding point at " << xpos << "," << ypos << std::endl;


    // Add a second control node colinear to the last to previous, non-end nodes
    if (nodes.size() <= 1) {
        nodes.push_back(nodeToAdd);
    }
    else if (nodes.size() >= 2) {
        // Determine which end point to make intermediate; the first or last one
        Node* closestNode;

        double distanceToNode1X = nodes[0]->x - nodeToAdd->x;
        double distanceToNode1Y = nodes[0]->y - nodeToAdd->y;

        double distanceToNode2X = nodes[nodes.size() - 1]->x - nodeToAdd->x;
        double distanceToNode2Y = nodes[nodes.size() - 1]->y - nodeToAdd->y;

        double distanceToNode1 = std::sqrt(distanceToNode1X * distanceToNode1X + distanceToNode1Y * distanceToNode1Y);
        double distanceToNode2 = std::sqrt(distanceToNode2X * distanceToNode2X + distanceToNode2Y * distanceToNode2Y);

        // Second control point being added to closest node
        Point* controlPointToAdd = new Point();

        if (distanceToNode1 <= distanceToNode2) {
            // Node 1, the one at the beginning is closer
            // Step 1: Make first node intermediate node
            // Step 2: Push new node to front

            // Where to place new control point?
            // At node, minus vector to other control point
            closestNode = nodes[0];
            double controlPointToAddX = closestNode->x + (closestNode->x - closestNode->handle1->x);
            double controlPointToAddY = closestNode->y + (closestNode->y - closestNode->handle1->y);

            controlPointToAdd->x = controlPointToAddX;
            controlPointToAdd->y = controlPointToAddY;

            nodes.push_front(nodeToAdd);
        }
        else {
            // Node 2, the one at the end is closer
            // Step 1: Make first node intermediate node
            // Step 2: Push new node to back
            // Where to place new control point?
            // At node, minus vector to other control point
            closestNode = nodes[nodes.size() - 1];
            double controlPointToAddX = closestNode->x + (closestNode->x - closestNode->handle1->x);
            double controlPointToAddY = closestNode->y + (closestNode->y - closestNode->handle1->y);

            controlPointToAdd->x = controlPointToAddX;
            controlPointToAdd->y = controlPointToAddY;

            
            nodes.push_back(nodeToAdd);
        }

        closestNode->hasHandle2 = true;
        closestNode->handle2 = controlPointToAdd;
    }
}

void resetAllPoints() {
    for (int i = 0; i < nodes.size(); i++) {
        free(nodes[i]->handle1);
        free(nodes[i]->handle2); // Ok to call free on possible null pointer
        free(nodes[i]);
    }

    nodes.clear();

    //std::cout << "E was pressed. Deleting nodes and control points." << std::endl;
}

int main(int argc, char* argv[]) {
    SCREEN_WIDTH = atoi(argv[1]);
    SCREEN_HEIGHT = atoi(argv[2]);

    if (!glfwInit())
        return -1;

    enableMSAA();

    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Assignment_3", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    setup(SCREEN_WIDTH, SCREEN_HEIGHT);

    bool leftButtonPressed = false;

    bool draggingNode = false;
    bool draggingControlPoint = false;
    Node* nodeToDrag = NULL;
    Node* nodeOfControlPointToDrag = NULL;
    Point* firstControlPointToDrag = NULL;
    Point* secondControlPointToDrag = NULL;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);

        drawBezierCurves();
        drawNodesAndPoints();
        drawControlPointDottedLines();

        int leftState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
        
        // "E is pressed"
        // Delete all nodes
        if (glfwGetKey(window, GLFW_KEY_E)) {
            resetAllPoints();
        }

        // "Button pressed"
        if (leftState == GLFW_PRESS && leftButtonPressed) {
            if (!draggingNode && !draggingControlPoint) {
                if (closeToControlPoint(firstControlPointToDrag, secondControlPointToDrag, nodeOfControlPointToDrag)) {
                    draggingControlPoint = true;
                    //std::cout << "The user has clicked on a control node" << std::endl;
                }
                else if (closeToNode(nodeToDrag)) {
                    draggingNode = true;
                }
            }
            else if (draggingNode) {
                dragNode(nodeToDrag);
            }
            else if (draggingControlPoint) {
                dragControlPoint(firstControlPointToDrag, secondControlPointToDrag, nodeOfControlPointToDrag);
            }
        }
        // "Button down"
        else if (leftState == GLFW_PRESS && !leftButtonPressed) {
            leftButtonPressed = true;

            // Make sure user isn't clicking on node or control point
            if (!closeToNode(nodeToDrag) && !closeToControlPoint()) {
                addPoint();
            } 
        }
        // "Button up"
        else if (leftState == GLFW_RELEASE && leftButtonPressed){
            leftButtonPressed = false;
            draggingNode = false;
            draggingControlPoint = false;
        }

        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}
