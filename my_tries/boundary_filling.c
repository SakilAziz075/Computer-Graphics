#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

#define pi 3.142857

void myInit()
{
    glClearColor(1, 1, 1, 1);
    glColor3f(0, 0, 0);
    glPointSize(1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Setting Window Dimension in X- and Y- direction
    gluOrtho2D();
}

// Color definitions
float fillColorLeftEye[3] = {1.0f, 1.0f, 0.0f};  // Yellow
float fillColorRightEye[3] = {1.0f, 1.0f, 0.0f}; // Yellow
float fillColorMouth[3] = {1.0f, 0.0f, 0.0f};    // Red
float fillColorCircle[3] = {0.0f, 0.0f, 1.0f};   // Blue for the area between eyes
float boundaryColor[3] = {0.0f, 0.0f, 0.0f};     // Black boundary

// Function to compare two colors
int colorsEqual(float color1[3], float color2[3])
{
    return (fabs(color1[0] - color2[0]) < 0.01f &&
            fabs(color1[1] - color2[1]) < 0.01f &&
            fabs(color1[2] - color2[2]) < 0.01f);
}

// Function to get pixel color at a point
void getPixelColor(int x, int y, float color[3])
{
    glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, color);
}

// Function to set pixel color
void setPixelColor(int x, int y, float color[3])
{
    glColor3fv(color);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
    glFlush();
}

void plot_points(int xc, int yc, int x, int y)
{
    glBegin(GL_POINTS);
    glVertex2i(xc + x, yc + y); // Octant 1
    glVertex2i(xc - x, yc + y); // Octant 2
    glVertex2i(xc + x, yc - y); // Octant 7
    glVertex2i(xc - x, yc - y); // Octant 8
    glVertex2i(xc + y, yc + x); // Octant 4
    glVertex2i(xc - y, yc + x); // Octant 3
    glVertex2i(xc + y, yc - x); // Octant 6
    glVertex2i(xc - y, yc - x); // Octant 5
    glEnd();
    glFlush();
}

void DDA(float xa, float ya, float xb, float yb)
{
    float dx, dy, xinc, yinc, i, k, x, y;

    dx = xb - xa;
    dy = yb - ya;

    x = xa;
    y = ya;

    if (abs(dx) > abs(dy))
    {
        k = abs(dx);
    }
    else
    {
        k = abs(dy);
    }

    xinc = dx / k;
    yinc = dy / k;

    glBegin(GL_POINTS); // Start drawing points
    glVertex2i(round(x), round(y));

    for (i = 0; i < k; i++)
    {
        x = xinc + x;
        y = yinc + y;
        glVertex2i(round(x), round(y));
    }
    glEnd();
    glFlush();
}

void midpoint_circle(int xc, int yc, int r)
{
    int x = 0, y = r;
    int p = 1 - r;

    plot_points(xc, yc, x, y);

    while (x < y)
    {
        x++;
        if (p < 0)
        {
            p = p + (2 * x) + 1;
        }
        else
        {
            y--;
            p = p + (2 * x) + 1 - (2 * y);
        }
        plot_points(xc, yc, x, y);
    }
}

// Boundary fill algorithm
void boundaryFill(int x, int y, float *fc, float *bc)
{
    float color[3];
    glReadPixels(x, y, 1.0, 1.0, GL_RGB, GL_FLOAT, color);

    if (!colorsEqual(color, bc) && !colorsEqual(color, fc))
    {
        glColor3fv(fc);
        glPointSize(2.0);
        glBegin(GL_POINTS);
        glVertex2i(x, y);
        glEnd();
        glFlush();

        // Define the 8 neighboring pixels
        int dx[] = {2, 2, 0, -2, -2, -2, 0, 2};
        int dy[] = {0, 2, 2, 2, 0, -2, -2, -2};

        for (int i = 0; i < 8; i++)
        {
            boundaryFill(x + dx[i], y + dy[i], fc, bc);
        }
    }
    return;
}

// Draw the outer triangle, inner triangle, and inner rectangle
void drawShape(int xmin, int ymin, int xmax, int ymax)
{
    glClear(GL_COLOR_BUFFER_BIT);

    int radius, xc, yc;

    xc = xmin + (xmax - xmin) / 2;
    yc = xmax + (ymax - xmin) / 2;

    radius = (xmax - xmin) / 2;

    midpoint_circle(xc, yc, radius);

    DDA(xc - 70, yc + 20, xc - 30, yc + 20); // Call the DDA function
    DDA(xc - 30, yc + 20, xc - 30, yc - 20);
    DDA(xc - 30, yc - 20, xc - 70, yc - 20);
    DDA(xc - 70, yc - 20, xc - 70, yc + 20);

    DDA(xc + 70, yc + 20, xc + 30, yc + 20); // Call the DDA function
    DDA(xc + 30, yc + 20, xc + 30, yc - 20);
    DDA(xc + 30, yc - 20, xc + 70, yc - 20);
    DDA(xc + 70, yc - 20, xc + 70, yc + 20);

    DDA(xc - 20, yc - 50, xc + 20, yc - 50); // Call the DDA function
    DDA(xc + 20, yc - 50, xc + 20, yc - 90);
    DDA(xc + 20, yc - 90, xc - 20, yc - 90);
    DDA(xc - 20, yc - 90, xc - 20, yc - 50);
}

// Display callback
void display()
{
    int xmin, ymin, xmax, ymax;

    xmin = 0;
    xmax = 300;
    ymin = 0;
    ymax = 300;

    drawShape(xmin, ymin, xmax, ymax);

    int radius, xc, yc;
    xc = xmin + (xmax - xmin) / 2;
    yc = xmax + (ymax - xmin) / 2;

    boundaryFill(xc - 65, yc + 15, fillColorLeftEye, boundaryColor);
    boundaryFill(xc + 65, yc + 15, fillColorRightEye, boundaryColor);
    boundaryFill(xc - 15, yc - 55, fillColorMouth, boundaryColor);
    boundaryFill(xc, yc, fillColorCircle, boundaryColor);
}

// Main function
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Boundary Fill Algorithm - Triangle with Rectangle");
    myInit();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
