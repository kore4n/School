#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int screenW = 500.0f;
int screenH = 500.0f;

// Nearest to furthest: Incorrect
void version1() {
	glMatrixMode(GL_PROJECTION);
	glm::mat4 P = glm::perspective(glm::radians(45.0f), (GLfloat)screenW / screenH, 0.001f, 1000.0f);
	glLoadMatrixf(glm::value_ptr(P));
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_TRIANGLES);
	glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
	glVertex3f(0.0f, 1.0f, -3.0f);
	glVertex3f(-1.0f, 1.0f, -3.0f);
	glVertex3f(-1.0f, -1.0f, -3.0f);
	glVertex3f(0.0f, -1.0f, -3.0f);
	glVertex3f(0.0f, 1.0f, -3.0f);
	glVertex3f(-1.0f, -1.0f, -3.0f);
	glColor4f(0.0f, 1.0f, 0.0f, 0.3f);
	glVertex3f(-0.5f, 0.0f, -5.0f);
	glVertex3f(-0.5f, -1.0f, -5.0f);
	glVertex3f(1.f, -1.0f, -5.0f);
	glVertex3f(1.f, 0.0f, -5.0f);
	glVertex3f(-0.5f, 0.0f, -5.0f);
	glVertex3f(1.f, -1.0f, -5.0f);
	glEnd();

}

// Furthest to nearest: Correct
void version2() {
	glMatrixMode(GL_PROJECTION); glm::mat4 P = glm::perspective(glm::radians(45.0f), (GLfloat)screenW / screenH, 0.001f, 1000.0f); 
	glLoadMatrixf(glm::value_ptr(P));
	glEnable(GL_BLEND); 
	//glBlendFunc(GL_ONE, GL_ZERO);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_TRIANGLES); 
	glColor4f(0.0f, 1.0f, 0.0f, 0.3f); 
	glVertex3f(-0.5f, 0.0f, -5.0f);
	glVertex3f(-0.5f, -1.0f, -5.0f); 
	glVertex3f(1.f, -1.0f, -5.0f); 
	glVertex3f(1.f, 0.0f, -5.0f);
	glVertex3f(-0.5f, 0.0f, -5.0f); 
	glVertex3f(1.f, -1.0f, -5.0f);
	glColor4f(1.0f, 0.0f, 0.0f, 0.5f); 
	glVertex3f(0.0f, 1.0f, -3.0f); 
	glVertex3f(-1.0f, 1.0f, -3.0f);
	glVertex3f(-1.0f, -1.0f, -3.0f); 
	glVertex3f(0.0f, -1.0f, -3.0f); 
	glVertex3f(0.0f, 1.0f, -3.0f);
	glVertex3f(-1.0f, -1.0f, -3.0f); 
	glEnd();
}

void exercise3() {

}

int main() {
    int t = 0;
    bool shouldIncrease = true;

    GLFWwindow* window;

    if (!glfwInit())
        return -1;
    window = glfwCreateWindow(1200, 800, "Problem_Set_6", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

		//version1();
		//version2();
		exercise3();
		
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}