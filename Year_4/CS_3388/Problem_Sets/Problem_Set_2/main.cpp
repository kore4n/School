#include <GL/glut.h>
void displayMe(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    //glBegin(GL_TRIANGLE_FAN);
    //glVertex2f(0., 0.5);
    //glVertex2f(0.2, 0.5);
    //glVertex2f(0.4, 0.5);
    //glVertex2f(0.6, 0.9);
    //glVertex2f(0.7, 0.2); glVertex2f(0,
    //-0.2); glVertex2f(-0.2, 0.0);
    //glEnd();
    //glBegin(GL_TRIANGLES);
    //for (int i = 1; i < 10; ++i) { glVertex2f(0.09*i, -0.7);
    //glVertex2f(-0.09*i, -0.3);
    //} glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.4,0.4);
    glVertex2f(-0.3,0.2);
    glVertex2f(0.2,-0.3);
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
