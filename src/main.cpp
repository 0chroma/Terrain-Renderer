#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>

#include "CameraManager.h"
#include "QuadTree.h"
#include "TerrainRenderer.h"

float window_width=600;
float window_height=600;
CameraManager* cm;
TerrainRenderer* tr;
QuadTree* qt;

void init()
{
	glViewport(0, 0, window_width, window_height);
/*
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1, 1, -1, 1, 1.5, 20);
    glMatrixMode(GL_MODELVIEW);*/
}

void display_func()
{
	//printf("Display()\n");
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    
    cm->applyPosition();

	//specify the color to clear the color buffer
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_LIGHTING);
    glShadeModel(GL_FLAT);
    glEnable(GL_LIGHT0);

    
    GLfloat emissionLight0[] = {0.2f, 0.0f, 0.0f, 1.0f};
    GLfloat ambientLight0[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuseLight0[] = { 0.8f, 0.8f, 0.8, 1.0f };
    GLfloat specularLight0[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat position0[] = { -1.5f, 1.0f, 4.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight0);
    glLightfv(GL_LIGHT0, GL_EMISSION, emissionLight0);
    glLightfv(GL_LIGHT0, GL_POSITION, position0);


	glColor3f(0.5f, 0.5f, 0.5f);

    glEnable(GL_DEPTH_TEST);


    tr->render();

	glFinish();
}

void timer_func(int){
    glutTimerFunc(10, timer_func, 0);

    display_func();
    cm->inputTimer();

    glutPostRedisplay();
}

void mouse_func(int but, int state, int x, int y){
    cm->onMouseButton(but, state, x, y);
}

void mouse_move_func(int x, int y){
    cm->onMouseMove(x, y);
}

void key_func(unsigned char key, int x, int y){
    cm->onKey(key, CameraManager::STATE_DOWN);
}

void key_up_func(unsigned char key, int x, int y){
    cm->onKey(key, CameraManager::STATE_UP);
}

void reshape_func(int width, int height)
{
	printf("Reshape()\n");
	window_width=width;
	window_height=height;

    cm->window_width = width;
    cm->window_height = height;

	glViewport(0, 0, window_width, window_height);
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();


    gluPerspective(60, (GLfloat)width / (GLfloat)height, 0.01, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv)
{
    
    //initiate our classes
    Image *img = new Image("./data/black.png");
    qt = new QuadTree();
    qt->initialize(img);
    cm = new CameraManager(window_width, window_height);
    tr = new TerrainRenderer(cm, qt);


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("Renderer");

	init();

    glutDisplayFunc(display_func);
    glutTimerFunc(100, timer_func, 0);    
	glutReshapeFunc(reshape_func);
    glutMouseFunc(mouse_func);
    glutMotionFunc(mouse_move_func);
    glutKeyboardFunc(key_func);
    glutKeyboardUpFunc(key_up_func);
    glutMainLoop();

	return 0;
}
