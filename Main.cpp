#include <GL/glut.h>

bool isLanding = false;
bool isTakingOff = false;
float planeX = 50.0f;  // X position of the plane (starting from the air for landing)
float planeY = 80.0f;  // Y position of the plane for landing

void drawAirport() {
    // Sky
    glColor3f(0.0f, 0.8f, 1.0f);
    glBegin(GL_QUADS);
        glVertex2f(-100.0f, 0.0f);
        glVertex2f(100.0f, 0.0f);
        glVertex2f(100.0f, 100.0f);
        glVertex2f(-100.0f, 100.0f);
    glEnd();
    
    // Grass
    glColor3f(0.0f, 0.6f, 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(-100.0f, -50.0f);
        glVertex2f(100.0f, -50.0f);
        glVertex2f(100.0f, 0.0f);
        glVertex2f(-100.0f, 0.0f);
    glEnd();
    
    // Runway
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_QUADS);
        glVertex2f(-40.0f, -20.0f);
        glVertex2f(40.0f, -20.0f);
        glVertex2f(40.0f, -30.0f);
        glVertex2f(-40.0f, -30.0f);
    glEnd();

    // Runway stripes
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
        glVertex2f(-35.0f, -25.0f);
        glVertex2f(-25.0f, -25.0f);
        glVertex2f(-25.0f, -26.0f);
        glVertex2f(-35.0f, -26.0f);
    glEnd();
    
    glBegin(GL_QUADS);
        glVertex2f(-10.0f, -25.0f);
        glVertex2f(0.0f, -25.0f);
        glVertex2f(0.0f, -26.0f);
        glVertex2f(-10.0f, -26.0f);
    glEnd();
    
    glBegin(GL_QUADS);
        glVertex2f(15.0f, -25.0f);
        glVertex2f(25.0f, -25.0f);
        glVertex2f(25.0f, -26.0f);
        glVertex2f(15.0f, -26.0f);
    glEnd();
}

void drawPlane() {
    // Plane body
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(planeX, planeY);
        glVertex2f(planeX + 8.0f, planeY);
        glVertex2f(planeX + 8.0f, planeY + 2.0f);
        glVertex2f(planeX, planeY + 2.0f);
    glEnd();
    
    // Plane tail
    glBegin(GL_TRIANGLES);
        glVertex2f(planeX, planeY + 2.0f);
        glVertex2f(planeX - 3.0f, planeY + 4.0f);
        glVertex2f(planeX, planeY + 6.0f);
    glEnd();
    
    // Plane wing
    glBegin(GL_TRIANGLES);
        glVertex2f(planeX + 4.0f, planeY + 2.0f);
        glVertex2f(planeX + 2.0f, planeY + 5.0f);
        glVertex2f(planeX + 6.0f, planeY + 5.0f);
    glEnd();
}

void updateLanding(int value) {
    if (isLanding) {
        planeX -= 1.0f; // Move left
        planeY -= 0.5f; // Move down
        
        if (planeY <= -20.0f) { // Plane lands
            isLanding = false;
            planeX = 0.0f; // Place on runway
            planeY = -25.0f; // Place on runway
        }
        
        glutPostRedisplay();
        glutTimerFunc(50, updateLanding, 0); // Update every 50ms
    }
}

void updateTakeOff(int value) {
    if (isTakingOff) {
        planeX += 1.0f; // Move right
        planeY += 0.8f; // Move up
        
        if (planeY >= 80.0f) { // Plane takes off
            isTakingOff = false;
            planeX = -50.0f; // Reset to initial position for next takeoff
            planeY = -25.0f; // Reset on runway
        }
        
        glutPostRedisplay();
        glutTimerFunc(50, updateTakeOff, 0); // Update every 50ms
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
                planeX = 50.0f; // Reset starting position in the air
                planeY = 80.0f; // Starting height for landing
                glutTimerFunc(50, updateLanding, 0);
            }
            break;
        case 2: // Take off plane
            if (!isTakingOff && !isLanding) {
                isTakingOff = true;
                planeX = -50.0f; // Starting from the runway
                planeY = -25.0f; // Runway position
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
    glClearColor(0.0f, 0.7f, 1.0f, 1.0f); // Sky blue background
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-100.0f, 100.0f, -50.0f, 100.0f);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Airport Simulation");
    init();
    createMenu();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}