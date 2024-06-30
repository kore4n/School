#include <GL/glut.h>
void displayMe(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POLYGON);
        glColor3f(0, 255, 255);
        glVertex2f(0,0.5);
        glColor3f(0, 255, 255);
        glVertex2f(0.5,-0.25);
        glColor3f(0, 255, 255);
        glVertex2f(-0.5,-0.25);
    glEnd();
    glFlush();
}int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(1280, 1000);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Hello World");
    glutDisplayFunc(displayMe);
    glutMainLoop();
    return 0;
}
