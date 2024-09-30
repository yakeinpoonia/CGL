#include <GL/glut.h>

bool isLanding = false;
bool isTakingOff = false;
float planeX = 0.0f;  // X position of the plane
float planeY = 50.0f; // Y position of the plane for landing

void drawAirport() {
    // Drawing a simple runway
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_QUADS);
        glVertex2f(-50.0f, -30.0f);
        glVertex2f(50.0f, -30.0f);
        glVertex2f(50.0f, -20.0f);
        glVertex2f(-50.0f, -20.0f);
    glEnd();
}

void drawPlane() {
    // Drawing a simple plane
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLES);
        glVertex2f(planeX, planeY);
        glVertex2f(planeX + 10.0f, planeY + 5.0f);
        glVertex2f(planeX, planeY + 10.0f);
    glEnd();
}

void updateLanding(int value) {
    if (isLanding) {
        planeY -= 1.0f; // Move plane down for landing
        if (planeY <= -20.0f) { // Landed
            isLanding = false;
            planeY = -20.0f; // Position plane on the runway
        }
        glutPostRedisplay();
        glutTimerFunc(50, updateLanding, 0); // Update landing path every 50ms
    }
}

void updateTakeOff(int value) {
    if (isTakingOff) {
        planeY += 1.0f; // Move plane up for takeoff
        if (planeY >= 50.0f) { // Taken off
            isTakingOff = false;
            planeY = 50.0f; // Reset position after takeoff
        }
        glutPostRedisplay();
        glutTimerFunc(50, updateTakeOff, 0); // Update takeoff path every 50ms
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawAirport();
    drawPlane();
    glutSwapBuffers();
}

void handleMenu(int option) {
    switch (option) {
        case 1: // Land plane
            if (!isLanding && !isTakingOff) {
                isLanding = true;
                glutTimerFunc(50, updateLanding, 0);
            }
            break;
        case 2: // Take off plane
            if (!isTakingOff && !isLanding) {
                isTakingOff = true;
                glutTimerFunc(50, updateTakeOff, 0);
            }
            break;
        case 3: // Exit program
            exit(0);
            break;
    }
}

void createMenu() {
    glutCreateMenu(handleMenu);
    glutAddMenuEntry("Land Plane", 1);
    glutAddMenuEntry("Take Off Plane", 2);
    glutAddMenuEntry("Exit", 3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void init() {
    glClearColor(0.0f, 0.5f, 1.0f, 1.0f); // Background color (sky blue)
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-100.0f, 100.0f, -100.0f, 100.0f);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Airport Simulation");
    init();
    createMenu();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}