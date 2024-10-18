#include <stdio.h>
#include <GL/glut.h>
#include <math.h>

#define pi 3.142857

float xa =0, ya=0 , xb= -100 ,yb=-100;

// Initialization Initialization
void myInit(void)
{
    // making background color as black
    glClearColor(0.0, 0.0, 0.0, 1.0);

    // making picture color green(in RGB mode)
    glColor3f(0.0, 1.0, 0.0);

    // breath of picture boundary is 1 pixel
    glPointSize(1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Setting Window Dimension in X- and Y- direction
    gluOrtho2D(-780, 780, -420, 420);
}


/*
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);

    float x, y, i;

    // itertion y upto 2*pi , i.e 360 degrees
    // with small increment in angle as
    // glVertex2i just draws a point on specified co-ordinated

    for (i = 0; i < (2 * pi); i += 0.001)
    {
        x = 200 * cos(i);
        y = 200 * sin(i);

        glVertex2i(x, y);
    }
    glEnd();
    glFlush();
}
*/

void DDA(float xa , float ya , float xb , float yb)
{
    float dx , dy , xinc , yinc , i ,k ,x, y ;

    dx = xb-xa;
    dy = yb-ya;

    x = xa;
    y = ya;
    
    if(abs(dx) > abs(dy) )
    {
        k = abs(dx);
    } 

    else
    {
        k = abs(dy);
    }

    xinc = dx/k;
    yinc = dy/k;

    glBegin(GL_POINTS); // Start drawing points

    glVertex2i(round(x), round(y));
    for(i=0 ; i < k ; i++)
    {
        x = xinc + x;
        y = yinc + y;
        glVertex2i(round(x), round(y));

    }
    glEnd();
    glFlush();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer
    DDA(xa, ya, xb, yb); // Call the DDA function
}

int main(int argc , char ** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    
    // giving window size in X- and Y- direction
    glutInitWindowSize(500, 600);
    glutInitWindowPosition(0, 0);

    // Giving name to window

    glutCreateWindow("Circle Drawing");
    myInit();
    glutDisplayFunc(display);
    glutMainLoop();
}
