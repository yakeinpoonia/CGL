#include <GL/glut.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

// Static variables
static double x[10] = {0}, x2 = 0.0, r1 = 0.0;
static double yaxis[10] = {-15, -15, -15, -15, -15, -15, -15, -15, -15, -15};
static double max = 0;
static bool takeOff = false;

void stroke_output(GLfloat x, GLfloat y, const char *format, ...) {
    va_list args;
    char buffer[200], *p;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    glPushMatrix();
    glTranslatef(-2.5, y, 0);
    glScaled(0.003, 0.005, 0.005);
    for (p = buffer; *p; p++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
    }
    glPopMatrix();
}

// Runway strip
void strip(float x1) {
    glPushMatrix();
    glRotatef(-65, 0, 1, 0);
    glColor3f(1, 1, 1);
    glTranslatef(x1, -3.5, 7.8);
    glScaled(1, 0.15, 0.1);
    glutSolidCube(1);
    glPopMatrix();
}

// Draw plane
void drawPlane(float y1) {
    glPushMatrix();
    // Main Body
    glPushMatrix();
    glScalef(.3, 0.3, 1.5);
    if (y1 <= 15) glColor3f(1, 1.0, 0.5);
    if (y1 >= 15) glColor3f(1, 0.3, 0.5);
    glutSolidSphere(2.0, 50, 50);
    glPopMatrix();

    // Plane details (fins, tail, wheels, etc.)
    // --- Add other plane parts similarly (as in original code) ---
    glPopMatrix();
}

// Animate plane
void animate(float y1, float x1) {
    glPushMatrix();
    if (y1 <= -2) {
        glTranslatef(5.5 + y1, 3, 0);
        glRotatef(-90, 0, 1, 0);
    }
    if (y1 >= 15 && takeOff) {
        glRotatef(140, 0, 1, 0);
        if (y1 >= 15 && y1 <= 20) glTranslatef(2 + 15 - y1, -3, -3);
        if (y1 >= 20) glTranslatef(2 + 15 - y1, -3 - 20 + y1, -3);
    }

    if (y1 >= -2 && y1 <= 2) glTranslatef(3.0, 3.0, 0.0);
    if (y1 >= 2 && y1 <= 6.5) glTranslatef(3, 3 - y1 + 2, 0);
    if (y1 >= 6.5 && y1 <= 8.2) glTranslatef(3 - y1 + 6.5, 3 - y1 + 2, 0);
    if (y1 >= 8.2 && y1 <= 15) glTranslatef(3 - y1 + 6.5, 3 - 8.2 + 2, 0);

    if (y1 >= -2) glRotatef(x1, 0, 1, 0);

    glPushMatrix();
    glTranslatef(1, 0, 0);
    glScaled(0.3, 0.3, 0.15);
    if (y1 <= 8.2 && yaxis[0] >= -2) glRotatef(15, 0, 0, 1);

    if (y1 <= 15) drawPlane(y1);
    if (y1 >= 15 && takeOff) drawPlane(y1);

    glPopMatrix();
    glPopMatrix();
}

// Airport layout
void airport() {
    glColor3f(0, 1, 0);
    glBegin(GL_POLYGON);
    glVertex3f(-19, -3.5, 19);
    glVertex3f(-19, -3.5, -19);
    glVertex3f(19, -3.5, -19);
    glVertex3f(19, -3.5, 19);
    glEnd();

    strip(-2);
    strip(0);
    strip(2);
    strip(4);
    strip(6);
    strip(8);
    strip(10);

    // --- Add additional airport details here ---
}

void controller() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -25.0);
    animate(yaxis[0], x[0]);

    for (int i = 0; i < max; i++) {
        if (yaxis[i] >= -5) {
            animate(yaxis[i + 1], x[i + 1]);
            if (yaxis[i + 1] >= -2 && yaxis[i + 1] <= 6.7) x[i + 1] += 3.5;
            if (yaxis[i + 1] <= 0) yaxis[i + 1] += 0.15;
            else if (yaxis[i + 1] >= 0 && yaxis[i + 1] <= 6.7) yaxis[i + 1] += 0.06;
            else if (yaxis[i + 1] >= 6.7 && yaxis[i + 1] <= 15) yaxis[i + 1] += 0.1;
            else if (takeOff && yaxis[i + 1] <= 30) yaxis[i + 1] += 0.1;
        }
    }

    airport();
    x2 += 15.0;

    if (yaxis[0] >= -2 && yaxis[0] <= 6.7) x[0] += 3.5;
    if (yaxis[0] <= 0) yaxis[0] += 0.15;
    else if (yaxis[0] >= 0 && yaxis[0] <= 6.7) yaxis[0] += 0.06;
    else if (yaxis[0] >= 6.7) yaxis[0] += 0.1;

    glFlush();
    glutSwapBuffers();
}

void doInit() {
    glClearColor(1.0, 1.0, 1.0, 1.0);  // Set background to white
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 200.0);
    glMatrixMode(GL_MODELVIEW);
}

// Main function
int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Airport Simulation");
    doInit();
    glutDisplayFunc(controller);
    glutIdleFunc(controller);
    glEnable(GL_DEPTH_TEST);
    glutMainLoop();
    return 0;
}
