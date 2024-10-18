#include <GL/glut.h>
#include <stdio.h>

int xa, ya, xb, yb;

void myInit()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-780, 780, -420, 420);
}

void draw_pixel(int x, int y)
{
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

void draw_line(int xa, int ya, int xb, int yb)
{
    int dx, dy, i, p;
    int incx, incy, inc1, inc2;
    int x, y;

    dx = xb - xa;
    dy = yb - ya;

    // Determine direction of increment
    if (dx >= 0)
    {
        incx = 1;
    }
    else
    {
        incx = -1;
        dx = -dx; // Use absolute value of dx
    }

    if (dy >= 0)
    {
        incy = 1;
    }
    else
    {
        incy = -1;
        dy = -dy; // Use absolute value of dy
    }

    x = xa;
    y = ya;

    if (dx > dy)
    {
        draw_pixel(x, y);
        p = 2 * dy - dx;

        for (i = 0; i < dx; i++)
        {
            if (p >= 0)
            {
                y += incy;
                p += 2 * (dy - dx);
            }
            else
            {
                p += 2 * dy;
            }
            x += incx;
            draw_pixel(x, y);
        }
    }
    else
    {
        draw_pixel(x, y);
        p = 2 * dx - dy;

        for (i = 0; i < dy; i++)
        {
            if (p >= 0)
            {
                x += incx;
                p += 2 * (dx - dy);
            }
            else
            {
                p += 2 * dx;
            }
            y += incy;
            draw_pixel(x, y);
        }
    }
}

void myDisplay()
{
    draw_line(xa, ya, xb, yb);
    glFlush();
}

int main(int argc, char **argv)
{
    printf("Enter (xstart, ystart, xend, yend)\n");
    scanf("%d %d %d %d", &xa, &ya, &xb, &yb);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1080, 768);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Bresenham's Line Algorithm Visualization");
    myInit();
    glutDisplayFunc(myDisplay);
    glutMainLoop();
    return 0;
}
