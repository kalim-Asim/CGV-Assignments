#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define N 1000000

int getRandom(int min, int max) {
    return min + rand() % (max - min + 1);
}

// Bresenham's Line Algorithm to draw a line between two points
void drawLine(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1), dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        glVertex2i(x1, y1);
        if (x1 == x2 && y1 == y2) break;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);

    srand(time(NULL)); // Seed for random number generation

    int x1, y1, x2, y2;
    float slope;
    clock_t start = clock(); 

    for (int i = 0; i < N; i++) {
        x1 = getRandom(0, 500);
        y1 = getRandom(0, 500);
        x2 = getRandom(0, 500);
        y2 = getRandom(0, 500);

        slope = (x1 != x2) ? (float)(y2 - y1) / (x2 - x1) : INFINITY;

        drawLine(x1, y1, x2, y2);
    }

    clock_t end = clock(); 
    double time_taken = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Time taken to draw %d lines: %.2f seconds\n", N, time_taken);

    glEnd();
    glFlush();
}

void initOpenGL() {
    glClearColor(0.0, 0.0, 0.0, 0.0); // Black background
    glColor3f(1.0, 1.0, 1.0);         // White points
    glPointSize(1.0);                 // Point size
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 500, 0, 500);       // Set 2D coordinate system
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500); // Window size
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Generate Lines Using OpenGL");

    initOpenGL();              // Initialize OpenGL
    glutDisplayFunc(display);  // Set display callback
    glutMainLoop();            // Enter the GLUT event loop

    return 0;
}

