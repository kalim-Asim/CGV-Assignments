#include <GL/glut.h>
#include <iostream>
#include <cmath>

using namespace std;

// Window size
const int WIDTH = 800, HEIGHT = 800;

// Function Type for Pattern Matching
enum CurveType {
    CIRCLE,
    ELLIPSE,
    HYPERBOLA,
    PARABOLA
};

// Selected curve type
CurveType selectedCurve = CIRCLE;

// General function F(x, y) = 0
double F(int x, int y) {
    switch (selectedCurve) {
    case CIRCLE:
        return x * x + y * y - 10000;  // Circle (radius 100)
    case ELLIPSE:
        return (x * x) / 10000.0 + (y * y) / 2500.0 - 1;  // Ellipse
    case HYPERBOLA:
        return (x * x) / 10000.0 - (y * y) / 2500.0 - 1;  // Hyperbola
    case PARABOLA:
        return y - (x * x) / 100;  // Parabola y = x²/100
    default:
        return 0;
    }
}

// Bresenham’s algorithm for implicit curves
void drawImplicitCurve() {
    int x = -WIDTH / 2, y = -HEIGHT / 2; // Start from bottom-left
    glBegin(GL_POINTS);

    for (x = -WIDTH / 2; x <= WIDTH / 2; x++) {
        for (y = -HEIGHT / 2; y <= HEIGHT / 2; y++) {
            if (abs(F(x, y)) < 5) { // Bresenham-like approximation
                glVertex2i(x, y);
            }
        }
    }

    glEnd();
}

// Display function
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0); // White color
    drawImplicitCurve();
    glutSwapBuffers();
}

// Initialization
void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0); // Black background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-WIDTH / 2, WIDTH / 2, -HEIGHT / 2, HEIGHT / 2);
}

// Keyboard controls to change curves
void keyboard(unsigned char key, int, int) {
    switch (key) {
    case '1': selectedCurve = CIRCLE; break;
    case '2': selectedCurve = ELLIPSE; break;
    case '3': selectedCurve = HYPERBOLA; break;
    case '4': selectedCurve = PARABOLA; break;
    case 27: exit(0); // ESC key to exit
    }
    glutPostRedisplay();
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Bresenham & Pattern Matching for Implicit Functions");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
