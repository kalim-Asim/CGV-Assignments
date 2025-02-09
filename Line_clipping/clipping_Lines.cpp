#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define X_MIN 250
#define X_MAX 750
#define Y_MIN 250
#define Y_MAX 750
#define N 10  // Number of random lines

typedef struct {
    int x, y;
} Point;

typedef struct {
    Point start, end;
} Line;

// Region codes for Cohen-Sutherland Algorithm
#define INSIDE 0  // 0000
#define LEFT   1  // 0001
#define RIGHT  2  // 0010
#define BOTTOM 4  // 0100
#define TOP    8  // 1000

Line lines[N];

// Function to generate random points within the screen space
int randInRange(int min, int max) {
    return min + rand() % (max - min + 1);
}

// Assign region code for Cohen-Sutherland clipping
int computeCode(int x, int y) {
    int code = INSIDE;
    if (x < X_MIN) code |= LEFT;
    else if (x > X_MAX) code |= RIGHT;
    if (y < Y_MIN) code |= BOTTOM;
    else if (y > Y_MAX) code |= TOP;
    return code;
}

// Cohen-Sutherland Line Clipping Algorithm
int cohenSutherlandClip(Line* line) {
    int x1 = line->start.x, y1 = line->start.y;
    int x2 = line->end.x, y2 = line->end.y;

    int code1 = computeCode(x1, y1);
    int code2 = computeCode(x2, y2);

    int accept = 0;

    while (1) {
        if ((code1 == 0) && (code2 == 0)) {
            accept = 1;  // Both points inside
            break;
        }
        else if (code1 & code2) {
            return 0;  // Line completely outside
        }
        else {
            int code_out;
            int x, y;

            if (code1 != 0) code_out = code1;
            else code_out = code2;

            if (code_out & TOP) {
                x = x1 + (x2 - x1) * (Y_MAX - y1) / (y2 - y1);
                y = Y_MAX;
            }
            else if (code_out & BOTTOM) {
                x = x1 + (x2 - x1) * (Y_MIN - y1) / (y2 - y1);
                y = Y_MIN;
            }
            else if (code_out & RIGHT) {
                y = y1 + (y2 - y1) * (X_MAX - x1) / (x2 - x1);
                x = X_MAX;
            }
            else if (code_out & LEFT) {
                y = y1 + (y2 - y1) * (X_MIN - x1) / (x2 - x1);
                x = X_MIN;
            }

            if (code_out == code1) {
                x1 = x;
                y1 = y;
                code1 = computeCode(x1, y1);
            }
            else {
                x2 = x;
                y2 = y;
                code2 = computeCode(x2, y2);
            }
        }
    }

    line->start.x = x1;
    line->start.y = y1;
    line->end.x = x2;
    line->end.y = y2;
    return accept;
}

// Bresenham's Line Drawing Algorithm
void drawBresenhamLine(int x1, int y1, int x2, int y2) {
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

// OpenGL display function
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);

    // Draw the bounding square
    for (int x = X_MIN; x <= X_MAX; x++) {
        glVertex2i(x, Y_MIN);
        glVertex2i(x, Y_MAX);
    }
    for (int y = Y_MIN; y <= Y_MAX; y++) {
        glVertex2i(X_MIN, y);
        glVertex2i(X_MAX, y);
    }

    // Draw the clipped lines
    for (int i = 0; i < N; i++) {
        Line clippedLine = lines[i];
        if (cohenSutherlandClip(&clippedLine)) {
            drawBresenhamLine(clippedLine.start.x, clippedLine.start.y,
                clippedLine.end.x, clippedLine.end.y);
        }
    }

    glEnd();
    glFlush();
}

// Initialize OpenGL
void init() {
    glClearColor(0, 0, 0, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 1000, 0, 1000);
}

// Generate random lines
void generateRandomLines() {
    for (int i = 0; i < N; i++) {
        lines[i].start.x = randInRange(1, 1000);
        lines[i].start.y = randInRange(1, 1000);
        lines[i].end.x = randInRange(1, 1000);
        lines[i].end.y = randInRange(1, 1000);
    }
}

// Main function
int main(int argc, char** argv) {
    srand(time(0));
    generateRandomLines();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1000, 1000);
    glutCreateWindow("Bresenham Line Drawing with Clipping");

    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
