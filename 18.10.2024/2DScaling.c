#include <GL/glut.h>
#include <stdio.h>

float tx = 0 , ty= 0;

void init()
{
    glClearColor(1.0,1.0,1.0,1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-500,500,-500,500);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0, 0.0, 1.0);
    glBegin(GL_TRIANGLES);
        glVertex2f(-100,-100);
        glVertex2f(100,-100);
        glVertex2f(0,100);
    glEnd();

    //Draw the Scaling Triangle
    glPushMatrix();
    glTranslatef(tx,ty,0.0);
    glScalef(3.0,1.0,1.0);
    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_TRIANGLES);
        glVertex2f(-100,-100);
        glVertex2f(100,-100);
        glVertex2f(0,100);
    glEnd();
    glPopMatrix();

    glFlush();
}

void translateTriangle()
{
    printf("enter translation values for x and y\n");
    scanf("%f %f",&tx ,&ty);
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc ,argv);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500,500);
    glutCreateWindow("Original and translated ");
    init();
    glutDisplayFunc(display);
    translateTriangle();
    glutMainLoop();

    return 0;

}




