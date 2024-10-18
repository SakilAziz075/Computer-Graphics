#include <stdio.h>
#include <unistd.h>
#include <GL/glut.h>

int xc=0 , yc=0 , r =100;

void plot_points(int xc , int yc , int x , int y)
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

     usleep(50000); // 50ms delay (50000 microseconds)
}

//Midpoint Circle Drawing Algorithm
void midpoint_circle(int xc , int yc , int r)
{
    int x = 0 , y= r;
    int p = 1-r;

    plot_points(xc, yc, x ,y);

    while(x<y)
    {
        x++;
        
        if(p<0)
        {
            p= p+ (2*x) +1;
        }

        else 
        {
            y--;
            p= p+ (2*x) +1 -(2*y);
        }

        plot_points(xc ,yc ,x ,y);
    }
}

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
    gluOrtho2D(-400, 400, -400, 400);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    midpoint_circle(xc,yc,r);
}

int main(int argc , char ** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    
    // giving window size in X- and Y- direction
    glutInitWindowSize(1000,1000);
    glutInitWindowPosition(0, 0);

    // Giving name to window

    glutCreateWindow("Circle Drawing");
    myInit();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}