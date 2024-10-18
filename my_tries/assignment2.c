#include <GL/glut.h>
#include<stdio.h>
#include<math.h>

void init() {
    glClearColor(1,1,1,1);  

    gluOrtho2D(0, 500,0, 500); 
}



void PlotCirclePoints(int xc, int yc, int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(xc + x, yc + y);
    glVertex2i(xc - x, yc + y);
    glVertex2i(xc + x, yc - y);
    glVertex2i(xc - x, yc - y);
    glVertex2i(xc + y, yc + x);
    glVertex2i(xc - y, yc + x);
    glVertex2i(xc + y, yc - x);
    glVertex2i(xc - y, yc - x);
    glEnd();
}

void MidpointCircle(int xc, int yc, int r) {
    int x = 0;
    int y = r;
    int p = 1 - r;

    PlotCirclePoints(xc, yc, x, y);

    while (x < y) {
        x++;
        if (p < 0) {
            p += 2 * x + 1;
        } else {
            y--;
            p += 2 * x - 2 * y + 1;
        }
        PlotCirclePoints(xc, yc, x, y);
    }
    glFlush();
}

float fillColorOuterTriangle[3] = {1.0f, 1.0f, 0.0f};  
float fillColorInnerTriangle[3] = {0.0f, 1.0f, 0.0f};  
float fillColorRectangle[3] = {1.0f, 0.0f, 0.0f};      
float fillColorBetweenTriangles[3] = {0.0f, 0.0f, 1.0f}; 
float boundaryColor[3] = {0.0f, 0.0f, 0.0f};           

void getPixelColor(int x, int y, float color[3]) {
    glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, color);
}

void setPixelColor(int x, int y, float color[3]) {
    glColor3fv(color);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
    glFlush();
}

void boundaryFill(int x, int y, float *fc, float *bc)
{

    glReadPixels(x, y, 1.0, 1.0, GL_RGB, GL_FLOAT, color);
    // printf(" (%d, %d) ==>  %f %f %f\n", x, y, color[0], color[1], color[2]);

    if (!colorsEqual(color, bc) && !colorsEqual(color, fc))
    {
        glColor3fv(fc);
        glPointSize(2.0);
        glBegin(GL_POINTS);
        glVertex2i(x, y);
        glEnd();
        glFlush();

        // Define the 8 neighboring pixels
        // int dx[] = {1, 1, 0, -1, -1, -1, 0, 1};
        // int dy[] = {0, 1, 1, 1, 0, -1, -1, -1};
        int dx[] = {2, 2, 0, -2, -2, -2, 0, 2};
        int dy[] = {0, 2, 2, 2, 0, -2, -2, -2};
        // int dx[] = {6, 6, 0, -6, -6, -6, 0, 6};
        // int dy[] = {0, 6, 6, 6, 0, -6, -6, -6};

        for (int i = 0; i < 8; i++)
        {
            boundaryFill(x + dx[i], y + dy[i], fc, bc);
        }
    }

    return;
}


void drawShape() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_LOOP);
        glVertex2i(100,100);
        glVertex2i(300,100);
        glVertex2i(250,200);
        glVertex2i(150,200);
    glEnd();
    glBegin(GL_LINE_LOOP);
        glVertex2i(170,120);
        glVertex2i(230,120);
        glVertex2i(230,140);
        glVertex2i(170,140);
    glEnd();
    glBegin(GL_POINTS);
        MidpointCircle(170,170,15);
        MidpointCircle(230,170,15);
    glEnd();
    boundaryFill(150, 150, fillColorOuterTriangle, boundaryColor);
    boundaryFill(170, 170, fillColorInnerTriangle, boundaryColor);
    boundaryFill(230, 170, fillColorInnerTriangle, boundaryColor);
    boundaryFill(220, 130, fillColorBetweenTriangles, boundaryColor);
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Boundary Fill Algorithm - Triangle with Rectangle");
    init();
    glutDisplayFunc(drawShape);
    glutMainLoop();
    return 0;
}