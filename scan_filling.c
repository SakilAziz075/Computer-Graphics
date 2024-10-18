#include <stdio.h>
#include <math.h>
#include <GL/glut.h>

#define pi 3.142857
#define MAX_EDGES 1000

typedef struct {
    float x;
    float y;
    float xinc;
} Edge;

Edge aet[MAX_EDGES]; // Active Edge Table
int aetSize = 0;

// Function prototypes
void DDA(float xa, float ya, float xb, float yb);
void midpoint_circle(int xc, int yc, int r);
void plot_points(int xc, int yc, int x, int y);
void scanfill(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
void scanfill_circle(int xc, int yc, int radius);
void figure(int xmin, int ymin, int xmax, int ymax);
void myInit(void);
void display(void);

// Initialize OpenGL settings
void myInit(void)
{
    glClearColor(0, 0, 0, 1);       // Set background color to black
    glColor3f(0, 1, 0);             // Set default color to green
    glPointSize(1.0);               // Set point size
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-400, 400, -400, 400); // Set the orthographic projection
}

// Function to add an edge to the Scanline Edge Table (SET)
void addEdgeToSET(float x1, float y1, float x2, float y2, Edge *set, int *setSize)
{
    if (y1 > y2) { // Ensure y1 <= y2
        float temp = x1;
        x1 = x2;
        x2 = temp;
        temp = y1;
        y1 = y2;
        y2 = temp;
    }

    Edge e;
    e.x = x1;
    e.y = y1;
    e.xinc = (x2 - x1) / (y2 - y1);

    set[*setSize] = e;
    (*setSize)++;
}

// Function to add an edge to the Active Edge Table (AET)
void addEdgeToAET(float x1, float y1, float x2, float y2)
{
    if (aetSize < MAX_EDGES) {
        addEdgeToSET(x1, y1, x2, y2, aet, &aetSize);
    }
}

// Update AET by incrementing x for all edges
void updateAET(float y)
{
    for (int i = 0; i < aetSize; i++) {
        if (aet[i].y <= y) {
            aet[i].x += aet[i].xinc;
        }
    }
}

// Sort AET by x-coordinate
void sortAET()
{
    for (int i = 0; i < aetSize - 1; i++) {
        for (int j = i + 1; j < aetSize; j++) {
            if (aet[i].x > aet[j].x) {
                Edge temp = aet[i];
                aet[i] = aet[j];
                aet[j] = temp;
            }
        }
    }
}

// Scanline Fill Algorithm for polygons using AET and SET
void scanfill(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
    Edge set[MAX_EDGES];
    int setSize = 0;

    // Clear AET
    aetSize = 0;

    // Add edges to SET
    addEdgeToSET(x1, y1, x2, y2, set, &setSize);
    addEdgeToSET(x2, y2, x3, y3, set, &setSize);
    addEdgeToSET(x3, y3, x4, y4, set, &setSize);
    addEdgeToSET(x4, y4, x1, y1, set, &setSize);

    for (int y = -400; y < 400; y++) {
        // Add edges to AET
        for (int i = 0; i < setSize; i++) {
            if ((int)set[i].y <= y && (int)set[i].y + 1 > y) {
                addEdgeToAET(set[i].x, set[i].y, set[i].x + set[i].xinc, set[i].y + 1);
            }
        }

        // Remove edges from AET
        int newSize = 0;
        for (int i = 0; i < aetSize; i++) {
            if ((int)aet[i].y < y) {
                // Skip edge
            } else {
                aet[newSize++] = aet[i];
            }
        }
        aetSize = newSize;

        // Sort AET by x-coordinate
        sortAET();

        // Fill between edges
        glBegin(GL_POINTS);
        for (int i = 0; i < aetSize - 1; i += 2) {
            for (int x = (int)aet[i].x; x < (int)aet[i + 1].x; x++) {
                glVertex2i(x, y);
            }
        }
        glEnd();

        // Update AET
        updateAET(y + 1);
    }
}

// Function to fill a circle using horizontal scanlines
void scanfill_circle(int xc, int yc, int radius)
{
    for (int y = -radius; y <= radius; y++)
    {
        int dy = y;
        int dx = sqrt(radius * radius - dy * dy); // Solve for x from the circle equation

        int x_left = xc - dx;
        int x_right = xc + dx;

        // Draw the horizontal line between the two x points for this scanline
        glBegin(GL_POINTS);
        for (int x = x_left; x <= x_right; x++)
        {
            glVertex2i(x, yc + dy);
        }
        glEnd();
    }
    glFlush();
}

// Function to draw the figure and apply fill algorithms
void figure(int xmin, int ymin, int xmax, int ymax)
{
    glColor3f(1.0, 0.0, 0.0);    // Set color for square
    DDA(xmin, ymin, xmax, ymin); // Draw the square
    DDA(xmax, ymin, xmax, ymax);
    DDA(xmax, ymax, xmin, ymax);
    DDA(xmin, ymax, xmin, ymin);

    glColor3f(0.0, 1.0, 1.0); // Set color for filling
    glBegin(GL_POINTS);
    scanfill(xmin, ymin, xmax, ymin, xmax, ymax, xmin, ymax); // Apply scan fill on the Outer square

    // Calculate the center and radius for the circle
    int xc = xmin + ((xmax - xmin) / 2);
    int yc = ymin + ((ymax - ymin) / 2);
    int radius = (ymax - ymin) / 2;

    // Draw the circle
    midpoint_circle(xc, yc, radius);
    glColor3f(1.0, 1.0, 0.0); // Yellow for filling the circle
    scanfill_circle(xc, yc, radius); // Apply scan fill on the inner Circle

    // Vertices for the rotated square
    int x_distance = cos(pi / 4) * radius;
    int y_distance = sin(pi / 4) * radius;

    float x1 = xc + x_distance;
    float y1 = yc + y_distance;

    float x2 = xc - x_distance;
    float y2 = yc + y_distance;

    float x3 = xc - x_distance;
    float y3 = yc - y_distance;

    float x4 = xc + x_distance;
    float y4 = yc - y_distance;

    glColor3f(0.0, 1.0, 0.0); // Set color for filling
    glBegin(GL_POINTS);
    scanfill(x1, y1, x2, y2, x3, y3, x4, y4); // Apply scan fill on the inner square
    glEnd();
}

// Function to draw a line using DDA algorithm
void DDA(float xa, float ya, float xb, float yb)
{
    float dx, dy, xinc, yinc, i, k, x, y;
    dx = xb - xa;
    dy = yb - ya;
    xinc = dx > dy ? 1.0 : dx / dy;
    yinc = dy > dx ? 1.0 : dy / dx;
    x = xa;
    y = ya;
    k = dx > dy ? dx : dy;

    glBegin(GL_POINTS);
    for (i = 0; i <= k; i++)
    {
        glVertex2i(x, y);
        x += xinc;
        y += yinc;
    }
    glEnd();
}

// Function to draw a circle using midpoint algorithm
void midpoint_circle(int xc, int yc, int r)
{
    int x = 0;
    int y = r;
    int d = 1 - r; // Initial decision parameter
    plot_points(xc, yc, x, y);
    while (x < y)
    {
        if (d < 0)
        {
            d = d + 2 * x + 3;
        }
        else
        {
            d = d + 2 * (x - y) + 5;
            y--;
        }
        x++;
        plot_points(xc, yc, x, y);
    }
}

// Plot points for a circle in all octants
void plot_points(int xc, int yc, int x, int y)
{
    glVertex2i(xc + x, yc + y);
    glVertex2i(xc - x, yc + y);
    glVertex2i(xc + x, yc - y);
    glVertex2i(xc - x, yc - y);
    glVertex2i(xc + y, yc + x);
    glVertex2i(xc - y, yc + x);
    glVertex2i(xc + y, yc - x);
    glVertex2i(xc - y, yc - x);
}

// Main function to initialize and run GLUT
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer
    figure(-100, -100, 100, 100); // Draw figure and fill
    glFlush(); // Flush the buffer
}

// Main function
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Scanline Fill Algorithm");
    myInit();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
