// oglt.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <ctime>
#include <fstream>
#include <GL/freeglut.h>
//#include "Lex.cpp"
#include "Drawin.h"
#include "Calc.cpp"
#include "Parse.cpp";
#include "BmpLoader.h"
#include <windows.h>

void keyboard(unsigned char key, int x, int y);
void display(void);
void list();
void mouse(int a, int b, int c, int d);
void reshape(int , int );
void move(int , int );
int winw=300, winh=200;
float hi=2830;
GLuint index;
Drawin rt(0,0,250,700,"root");
Parse* p;
img ii = BmpLoader("heightmap2.bmp");
void generate(Node* n, Drawin* d)
{
	//Drawin* c = new Drawin(atoi(n->values["x"].c_str()),atoi(n->values["y"].c_str()),atoi(n->values["w"].c_str()),atoi(n->values["h"].c_str()),n->values["name"]);
	Drawin* c;
	if(n->name!="img")
	 c= new Drawin(n->values["x"],n->values["y"],n->values["w"],n->values["h"],n->values["name"], d);
	else  c = new Image(n->values["x"],n->values["y"],n->values["w"],n->values["h"],n->values["name"], d, ii);
	d->child.push_back(c);
	for(int i =0;i<n->children.size();++i)
		generate(n->children[i], c);
}
void timer(int) {
    glutPostRedisplay();
    glutTimerFunc(1000/60, timer, 0);
}
int _tmain(int argc, char** argv)
{ glShadeModel(GL_FLAT);
	//timer(0);
	//ii.height-=1;
	//ii.width-=1;
	cout <<sizeof(Drawin)<<" \n";
	ifstream ifs("gui.txt");
	Lex la(ifs);//("panel{x:20 y:7 w:'parw 70 -' h:'parh 70 > ?> parh 12 - ret @52 ' name:'igor' panel{x:2 y:7 w:70 h:50 name:'valera'}}");
	p=new Parse(la);
	try {while(true)
		generate(p->readNode(), &rt);}
	catch(exception e) {}
	srand(time(0));
	//rt.child.push_back(new Drawin(10,10,12,12,"child"));
	Lex l("200 30+"); 
	Calc c(l);
	c.calc();
	c.calc();
	winh=(int)c.calc();
	glutInit(&argc, argv);
	glutInitWindowSize(1000, 700);
	glutCreateWindow("GLUT Test");
	glutKeyboardFunc(&keyboard);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluOrtho2D(0.0,winw,0.0,winh);
	gluPerspective(120,0.7,1,500000);
	glMatrixMode(GL_MODELVIEW);
	glutMouseFunc(&mouse);
	glutPassiveMotionFunc(&move);
	glutDisplayFunc(&display);
	glutReshapeFunc(&reshape);
	
	glRotatef(-12,1,0,0);
	glTranslatef(-7000,-30000,-200000);
	/*glTranslatef(-winw/2,-winh/2,0);*/
	timer(0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//glEnable(GL_COLOR_MATERIAL);
	
	   list();
	glutMainLoop();
	return 0;
}
void list()
{
	index = glGenLists(1); 
// compile the display list, store a triangle in it
glNewList(index, GL_COMPILE);
  
  /*glColor3f(1.0f, 0.0f, 0.0f);
  glPushMatrix();
  glTranslatef(120,120,0);
	glRotatef(45,0,0,1);
  glBegin(GL_POLYGON);
    glVertex2i(0, 0);
    glVertex2i( 0, 30);
    glVertex2i( 30,  30);
    glVertex2i(30,  0);
  glEnd();
  glPopMatrix();
  glBegin(GL_LINES);
  glVertex2i(laa,lbb);
   glVertex2i(aa,bb);
   glEnd();*/
	//rt.upd();
  int sz=132;
  float mlt = 30.0;
  int step=3;glColor3f(0.1f, 0.7f, 0.3f);
  glPushMatrix();
glRotatef(90,0,0,1);
  glBegin(GL_TRIANGLES);
  for(int i = 0; i<ii.height*0.99-1;i+=step)
	  for(int j = 0; j<ii.width*0.99-1;j+=step)
	  {
		  int s=mlt*(int)ii.data[(i*ii.width+j)*3];
		  int l=mlt*(int)ii.data[(i    *ii.width+j+step)*3];
		  int d=mlt*(int)ii.data[((i+step)*ii.width+j+step)*3];
		  int u=mlt*(int)ii.data[((i+step)*ii.width+j)*3];
		  int y1=j*sz,y2=j*sz+step*sz,y3=j*sz+step*sz;
		  int x1=i*sz,x2=i*sz,x3=i*sz+step*sz;
		  int z1=s,z2=l,z3=d;
		  int a = (y1-y2)*(z3-z2)-(z1-z2)*(y3-y2);
			int b =(z1-z2)*(x3-x2)-(x1-x2)*(z3-z2);
		int c =(x1-x2)*(y3-y2)-(y1-y2)*(x3-x2);
		glNormal3i(a,b,c);
		glVertex3i(i*sz,j*sz,s);glVertex3i(i*sz, j*sz+step*sz,l);glVertex3i(i*sz+step*sz,j*sz+step*sz,d);
		y1=j*sz,y2=j*sz,y3=j*sz+step*sz;
		x1=i*sz,x2=i*sz+step*sz,x3=i*sz+step*sz;
		z1=s,z2=u,z3=d;
		a = (y1-y2)*(z3-z2)-(z1-z2)*(y3-y2);
		b =(z1-z2)*(x3-x2)-(x1-x2)*(z3-z2);
		c =(x1-x2)*(y3-y2)-(y1-y2)*(x3-x2);
		glNormal3i(-a,-b,-c);
		  glVertex3i(i*sz,j*sz,s);
		glVertex3i(i*sz+step*sz,j*sz,u);glVertex3i(i*sz+step*sz,j*sz+step*sz,d);
	  }
glEnd();glColor3f(0.0f, 0.0f, 1.0f);

glBegin(GL_POLYGON);
glVertex3i(0,0,hi);
glVertex3i(0,ii.width*sz,hi);
glVertex3i(ii.height*sz,ii.width*sz,hi);
glVertex3i(ii.height*sz,0,hi);
glEnd();
glPopMatrix();

	
	
  
 
glEndList();       
}
void reshape(int w, int h)
{
	//unsigned int start = clock();
	winw=w;winh=h;
	//rt.w=winw/5;
	//rt.h=winh;
	//rt.recalc();
	glViewport(0,0,w,h);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluOrtho2D(0.0,(double)w,0.0,(double)h);
	glMatrixMode(GL_MODELVIEW);
	//  cout<<"elapsed: "<<clock()-start<<endl;
	
}
void keyboard(unsigned char key, int x, int y)
{
	printf("%f:",hi);
  switch (key)
  {
    case '\x1B':
      exit(EXIT_SUCCESS);
      break;
	case 'a':
		glTranslatef(3000,0,0);
		break;
		case 's':glTranslatef(0,3000,0);
		break;
		case 'd':glTranslatef(-3000,0,0);
		break;
		case 'w':glTranslatef(0,-3000,0);
		break;
		case 'z':hi-=10;
		break;
		case 'x':hi+=10;
		break;
		case 'q'://glTranslatef(0,-1000,0);
		glRotatef(2,1,0,0);break;
		case 'e'://glTranslatef(0,-1000,0);
		glRotatef(-2,1,0,0);break;
  }
}
int aa=0,bb=0,laa=0,lbb=0;
void mouse(int a, int b, int c, int d)
{//printf("%d %d %d %d\n",a,b,c,d);
if(a==3)glTranslatef(0,0,-2000);else if(a==4)glTranslatef(0,0,2000);
	if(a==0&&b==1) {aa=c;bb=d;
	rt.onClick(a,b,c,winh-d);}
}

void move(int a, int b)
{
	laa=aa;lbb=bb;
	aa=a;bb=winh-b;
	display();
}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glCallList(index);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0,(double)winw,0.0,(double)winh);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	rt.upd();
	glPopMatrix();
glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	 glEnable(GL_DEPTH_TEST);
  
   glFlush();

}