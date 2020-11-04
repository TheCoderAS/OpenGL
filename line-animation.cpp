#include<iostream>
#include<math.h>
#include<GLEW/glew.h>

#include<GL/gl.h>
#include<GL/glut.h>
#include<GLFW/glfw3.h>
#include<glm/common.hpp>
#define pi 3.142857

static double line_length=0.5;

void display(){
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(1.0,0.0,0.0);
  glBegin(GL_LINES);
  glVertex3f(0.0,line_length,0.0);
  glVertex3f(line_length,0.0,0.0);
  glColor3f(0.0,0.0,1.0);
  glVertex3f(0.0,-line_length,0.0);
  glVertex3f(-line_length,0.0,0.0);
  glColor3f(0.0,1.0,1.0);
  glVertex3f(0.0,-line_length,0.0);
  glVertex3f(+line_length,0.0,0.0);
  glColor3f(0.8,.5,1.0);
  glVertex3f(0.0,+line_length,0.0);
  glVertex3f(-line_length,0.0,0.0);

  glEnd();
  glFlush();
}

void timer(int value){
  static bool flag=true;
  static int count=50;
  if(count==100){
    flag=false;
  }
  if(count==0){
    flag=true;
  }
  if(flag){
    line_length+=0.01;
    count++;
  }else{
    line_length-=0.01;
    count--;
  }
  glutPostRedisplay();
  glutTimerFunc(40,timer,0);
}

int main(int argc, char **argv){
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE |GLUT_RGB);
  glutInitWindowSize(600,600);
  glutInitWindowPosition(100,100);
  glutCreateWindow("Line Animation");
  glutDisplayFunc(display);
  glutTimerFunc(1,timer,0);

  glutMainLoop();
}