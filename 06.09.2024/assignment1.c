#include <stdio.h>
#include <math.h>
#include  <GL/glut.h>

#define pi 3.142857

void myInit(void)
{
    glClearColor(0,0,0,1);

    glColor3f(0 ,1,0);

    glPointSize(1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Setting Window Dimension in X- and Y- direction
    //gluOrtho2D(-780, 780, -420, 420);
    gluOrtho2D(-400, 400, -400, 400);

}

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

    // usleep(50000); // 50ms delay (50000 microseconds)
}

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


void figure(int xmin , int ymin ,int xmax , int ymax)
{
    DDA(xmin,ymin,xmax ,ymin); // Call the DDA function
    DDA(xmax ,ymin,xmax,ymax);
    DDA(xmax,ymax ,xmin ,ymax);
    DDA(xmin, ymax ,xmin ,ymin);

    int xc ,yc , radius;
    xc = xmin + ((xmax-xmin)/2);
    yc = ymin + ((ymax-ymin)/2);    
    radius = (ymax-ymin)/2;

    midpoint_circle(xc , yc , radius);

    int x_distance,y_distance ,x1,y1 , x2,y2, x3, y3 ,x4,y4;

    x_distance=cos(pi/4)*radius;
    y_distance =sin(pi/4)*radius;

    x1= xc+x_distance;
    y1= yc +y_distance;

    x2= xc - x_distance;
    y2=yc+y_distance;

    x3 =xc -x_distance;
    y3= yc-y_distance;

    x4 = xc + x_distance;
    y4 = yc -y_distance;

    DDA(x1,y1,x2,y2); // Call the DDA function
    DDA(x2,y2,x3,y3);
    DDA(x3,y3,x4,y4);
    DDA(x4,y4,x1,y1);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer
    
    figure(0,0 ,250 ,250);

    /*
    DDA(100,100,300,100); // Call the DDA function
    DDA(300,100,300,300);
    DDA(300,300,100,300);
    DDA(100,300,100,100);

    midpoint_circle(200,200,100);

    DDA(271,271,129,271); 
    DDA(129,271,129,129);
    DDA(129,129,271,129);
    DDA(271,129,271,271);
    */
}


int main(int argc , char ** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    
    // giving window size in X- and Y- direction
    glutInitWindowSize(1000, 800);
    glutInitWindowPosition(0, 0);

    // Giving name to window

    glutCreateWindow("Circle Drawing");
    myInit();
    glutDisplayFunc(display);
    glutMainLoop();
    
    return 0;
}