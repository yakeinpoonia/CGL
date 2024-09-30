/* 
 * Airport Simulation Program
 * 
 * Features:
 * 1. Right-click menu with options to land a plane, take off a plane, and quit the program.
 * 2. Planes can be in landing or taking off states.
 * 3. Basic rendering of an airport with runways and ground.
 * 
 * Compile with:
 * g++ airport_simulation.cpp -o airport_simulation -lGL -lGLU -lglut
 */

#include <GL/glut.h>
#include <vector>
#include <cmath>
#include <cstdlib>

// Define constants for window size
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Define plane states
enum PlaneState { LANDING, TAKING_OFF };

// Structure to represent a plane
struct Plane {
    PlaneState state;
    float x, y, z;          // Position
    float angle;            // Rotation angle for movement
    float speed;            // Movement speed
    bool active;            // Is the plane active in the scene

    Plane(PlaneState s) : state(s), angle(0.0f), speed(0.1f), active(true) {
        if (state == LANDING) {
            // Starting position for landing: high altitude, off to one side
            x = -20.0f;
            y = 10.0f;
            z = 20.0f;
        }
        else { // TAKING_OFF
            // Starting position for takeoff: on the runway
            x = 0.0f;
            y = 0.0f;
            z = 0.0f;
        }
    }
};

// Vector to hold all active planes
std::vector<Plane> planes;

// Function to draw the airport
void drawAirport() {
    // Draw Ground
    glColor3f(0.3f, 0.9f, 0.3f); // Green ground
    glBegin(GL_QUADS);
        glVertex3f(-30.0f, -1.0f, -30.0f);
        glVertex3f(-30.0f, -1.0f, 30.0f);
        glVertex3f(30.0f, -1.0f, 30.0f);
        glVertex3f(30.0f, -1.0f, -30.0f);
    glEnd();

    // Draw Runway
    glColor3f(0.7f, 0.7f, 0.7f); // Grey runway
    glBegin(GL_QUADS);
        glVertex3f(-10.0f, 0.0f, -1.0f);
        glVertex3f(-10.0f, 0.0f, 1.0f);
        glVertex3f(10.0f, 0.0f, 1.0f);
        glVertex3f(10.0f, 0.0f, -1.0f);
    glEnd();

    // Draw Runway Center Line
    glColor3f(1.0f, 1.0f, 1.0f); // White center line
    glLineWidth(2.0f);
    glBegin(GL_LINES);
        for(float i = -10.0f; i <= 10.0f; i += 2.0f) {
            glVertex3f(i, 0.01f, -0.5f);
            glVertex3f(i + 1.0f, 0.01f, 0.5f);
        }
    glEnd();
}

// Function to draw a simple plane
void drawPlane() {
    // Main Body
    glColor3f(0.8f, 0.0f, 0.0f); // Red body
    glPushMatrix();
        glScalef(1.0f, 0.2f, 0.5f);
        glutSolidCube(2.0f);
    glPopMatrix();

    // Wings
    glColor3f(0.0f, 0.0f, 0.8f); // Blue wings
    glPushMatrix();
        glTranslatef(0.0f, 0.1f, 0.0f);
        glScalef(3.0f, 0.05f, 1.0f);
        glutSolidCube(1.0f);
    glPopMatrix();

    // Tail
    glColor3f(0.0f, 0.0f, 0.8f); // Blue tail
    glPushMatrix();
        glTranslatef(-0.8f, 0.1f, -0.5f);
        glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
        glScalef(0.5f, 0.05f, 0.3f);
        glutSolidCube(1.0f);
    glPopMatrix();

    // Propeller
    glColor3f(0.0f, 0.0f, 0.0f); // Black propeller
    glPushMatrix();
        glTranslatef(1.0f, 0.2f, 0.0f);
        glRotatef(glutGet(GLUT_ELAPSED_TIME) / 10.0f, 0.0f, 0.0f, 1.0f); // Rotate propeller
        glBegin(GL_LINES);
            glVertex3f(-0.5f, 0.0f, 0.0f);
            glVertex3f(0.5f, 0.0f, 0.0f);
            glVertex3f(0.0f, -0.5f, 0.0f);
            glVertex3f(0.0f, 0.5f, 0.0f);
        glEnd();
    glPopMatrix();
}

// Display callback
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Set camera
    gluLookAt(0.0f, 15.0f, 30.0f,   // Eye position
              0.0f, 0.0f, 0.0f,     // Look at point
              0.0f, 1.0f, 0.0f);    // Up vector

    // Draw the airport
    drawAirport();

    // Draw all active planes
    for(auto &plane : planes) {
        if(plane.active) {
            glPushMatrix();
                glTranslatef(plane.x, plane.y, plane.z);
                glRotatef(plane.angle, 0.0f, 1.0f, 0.0f); // Rotate plane based on movement
                drawPlane();
            glPopMatrix();
        }
    }

    glutSwapBuffers();
}

// Idle callback for animation
void idle() {
    bool needRedisplay = false;

    for(auto &plane : planes) {
        if(plane.active) {
            if(plane.state == LANDING) {
                // Move plane towards the runway
                plane.x += plane.speed * cosf(45.0f * M_PI / 180.0f); // Diagonal movement
                plane.z -= plane.speed * sinf(45.0f * M_PI / 180.0f);

                // Rotate plane to face the runway
                plane.angle = 45.0f;

                // Check if plane has landed
                if(plane.z <= 0.0f && plane.x >= -1.0f && plane.x <= 1.0f) {
                    plane.active = false; // Plane has landed
                }
            }
            else if(plane.state == TAKING_OFF) {
                // Move plane away from the runway
                plane.x -= plane.speed * cosf(45.0f * M_PI / 180.0f); // Diagonal movement
                plane.z += plane.speed * sinf(45.0f * M_PI / 180.0f);

                // Rotate plane to face away
                plane.angle = -135.0f;

                // Remove plane if it's far away
                if(plane.x <= -20.0f || plane.z >= 20.0f) {
                    plane.active = false;
                }
            }

            needRedisplay = true;
        }
    }

    // Remove inactive planes from the vector
    planes.erase(std::remove_if(planes.begin(), planes.end(),
                                [](const Plane &p) { return !p.active; }),
                planes.end());

    if(needRedisplay) {
        glutPostRedisplay();
    }
}

// Function to add a new plane
void addPlane(PlaneState state) {
    if(planes.size() >= 10) { // Limit the number of planes
        return;
    }
    planes.emplace_back(state);
}

// Menu callback
void menu(int option) {
    switch(option) {
        case 1: // Land a plane
            addPlane(LANDING);
            break;
        case 2: // Take off a plane
            addPlane(TAKING_OFF);
            break;
        case 3: // Quit
            exit(0);
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

// Initialize OpenGL settings
void initGL() {
    glClearColor(0.529f, 0.808f, 0.922f, 1.0f); // Sky blue background
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Set up light parameters
    GLfloat light_pos[] = { 0.0f, 20.0f, 20.0f, 1.0f };
    GLfloat light_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat light_diffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glShadeModel(GL_SMOOTH);
}

// Reshape callback to handle window resizing
void reshape(int width, int height) {
    if(height == 0) height = 1; // Prevent division by zero
    float aspect = (float)width / (float)height;

    glViewport(0, 0, width, height);

    // Set projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, aspect, 1.0f, 100.0f);

    // Return to modelview matrix mode
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Main function
int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Airport Simulation");

    // Initialize OpenGL settings
    initGL();

    // Register callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);

    // Create right-click menu
    glutCreateMenu(menu);
    glutAddMenuEntry("Land a Plane", 1);
    glutAddMenuEntry("Take Off a Plane", 2);
    glutAddMenuEntry("Quit", 3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    // Enter the main loop
    glutMainLoop();

    return 0;
}