
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

void setPixel(int x, int y, float intensity) {
    glColor3f(intensity, intensity, intensity);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

float fractionalPart(float x) {
    return x - floor(x);
}

float reverseFractionalPart(float x) {
    return 1 - fractionalPart(x);
}

// 1. DDA Algorithm
void drawLineDDA(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1, dy = y2 - y1, steps;
    float xInc, yInc, x = x1, y = y1;
    steps = (abs(dx) > abs(dy)) ? abs(dx) : abs(dy);
    xInc = dx / (float)steps;
    yInc = dy / (float)steps;
    for (int i = 0; i <= steps; i++) {
        setPixel(round(x), round(y), 1.0);
        x += xInc;
        y += yInc;
    }
}

// 2. Bresenham’s Algorithm using signbit
void drawLineBresenham(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1), dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    while (x1 != x2 || y1 != y2) {
        setPixel(x1, y1, 1.0);
        int e2 = err * 2;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx) { err += dx; y1 += sy; }
    }
}

// 3. Xiaolin Wu’s Line Algorithm (Anti-aliased)
void drawLineWu(int x1, int y1, int x2, int y2) {
    int steep = abs(y2 - y1) > abs(x2 - x1);
    if (steep) {
        int temp;
        temp = x1; x1 = y1; y1 = temp;
        temp = x2; x2 = y2; y2 = temp;
    }
    if (x1 > x2) {
        int temp;
        temp = x1; x1 = x2; x2 = temp;
        temp = y1; y1 = y2; y2 = temp;
    }
    float dx = x2 - x1, dy = y2 - y1;
    float gradient = (dx == 0) ? 1 : dy / dx;
    float xEnd = round(x1);
    float yEnd = y1 + gradient * (xEnd - x1);
    float xGap = reverseFractionalPart(x1 + 0.5);
    int xPixel1 = xEnd;
    int yPixel1 = floor(yEnd);
    if (steep) {
        setPixel(yPixel1, xPixel1, reverseFractionalPart(yEnd) * xGap);
        setPixel(yPixel1 + 1, xPixel1, fractionalPart(yEnd) * xGap);
    }
    else {
        setPixel(xPixel1, yPixel1, reverseFractionalPart(yEnd) * xGap);
        setPixel(xPixel1, yPixel1 + 1, fractionalPart(yEnd) * xGap);
    }
    float interY = yEnd + gradient;
    for (int x = xPixel1 + 1; x <= x2; x++) {
        if (steep) {
            setPixel(floor(interY), x, reverseFractionalPart(interY));
            setPixel(floor(interY) + 1, x, fractionalPart(interY));
        }
        else {
            setPixel(x, floor(interY), reverseFractionalPart(interY));
            setPixel(x, floor(interY) + 1, fractionalPart(interY));
        }
        interY += gradient;
    }
}

// 4. Bresenham’s Circle Algorithm
void drawCircleBresenham(int xc, int yc, int r) {
    int x = 0, y = r, d = 3 - 2 * r;
    while (y >= x) {
        setPixel(xc + x, yc + y, 1.0);
        setPixel(xc - x, yc + y, 1.0);
        setPixel(xc + x, yc - y, 1.0);
        setPixel(xc - x, yc - y, 1.0);
        setPixel(xc + y, yc + x, 1.0);
        setPixel(xc - y, yc + x, 1.0);
        setPixel(xc + y, yc - x, 1.0);
        setPixel(xc - y, yc - x, 1.0);
        x++;
        if (d > 0) { y--; d += 4 * (x - y) + 10; }
        else { d += 4 * x + 6; }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawLineDDA(50, 50, 200, 200);
    drawLineBresenham(100, 50, 200, 150);
    drawLineWu(50, 300, 200, 350);
    drawCircleBresenham(300, 300, 50);
    glFlush();
}

void init() {
    glClearColor(0, 0, 0, 1);
    glColor3f(1, 1, 1);
    glPointSize(2);
    gluOrtho2D(0, 500, 0, 500);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Line and Circle Drawing");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
