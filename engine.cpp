#include <windows.h>
#include <windowsx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <ctime>
#include <fstream>
#include "Drawin.h"
//#include "Calc.cpp"
#include "Parse.cpp"
#include "BmpLoader.h"
#include "resource.h"

HWND  g_hWnd = NULL;
HDC   g_hDC  = NULL;
HGLRC g_hRC  = NULL;
int nWidth=640, nHeight=480;
Drawin rt(0,0,252,700,"root");
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void init(void);
void shutDown(void);
void render(void);
void list();
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR     lpCmdLine,
                   int       nCmdShow)
{
    WNDCLASSEX winClass; 
	MSG        uMsg;

    memset(&uMsg,0,sizeof(uMsg));
    
	winClass.lpszClassName = "MY_WINDOWS_CLASS";
	winClass.cbSize        = sizeof(WNDCLASSEX);
	winClass.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	winClass.lpfnWndProc   = WindowProc;
	winClass.hInstance     = hInstance;
    winClass.hIcon	       = LoadIcon(hInstance, (LPCTSTR)IDI_OPENGL_ICON);
    winClass.hIconSm	   = LoadIcon(hInstance, (LPCTSTR)IDI_OPENGL_ICON);
	winClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	winClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winClass.lpszMenuName  = NULL;
	winClass.cbClsExtra    = 0;
	winClass.cbWndExtra    = 0;

	if( !RegisterClassEx(&winClass) )
		return E_FAIL;

	g_hWnd = CreateWindowEx(NULL, "MY_WINDOWS_CLASS", 
                            "Spartacus engine",
						    WS_OVERLAPPEDWINDOW | WS_VISIBLE,
					        0, 0, 640, 480, NULL, NULL, hInstance, NULL);

	if(g_hWnd == NULL)
		return E_FAIL;

    ShowWindow(g_hWnd, nCmdShow);
    UpdateWindow(g_hWnd);

	init();

	while(uMsg.message != WM_QUIT)
	{
		if(PeekMessage( &uMsg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&uMsg);
			DispatchMessage(&uMsg);
		}
        else
			render();
	}

	shutDown();

    UnregisterClass("MY_WINDOWS_CLASS", winClass.hInstance);

	return uMsg.wParam;
}

LRESULT CALLBACK WindowProc(HWND   hWnd, 
							UINT   msg, 
							WPARAM wParam, 
							LPARAM lParam)
{
    switch(msg)
	{
		// Обработка нажатий клавиш
        case WM_KEYDOWN:
		{
			switch(wParam)
			{
				// Если нажали Esc - выходим
				case VK_ESCAPE:
					PostQuitMessage(0);
					break;
                case VK_LEFT:
                glTranslatef(300,0,0);
                break;
                case VK_DOWN:glTranslatef(0,300,0);
                break;
                case VK_RIGHT:glTranslatef(-300,0,0);
                break;
                case VK_UP:glTranslatef(0,-300,0);
                break;
			}
		}
        break;
        case WM_LBUTTONUP:
            {int xPos = GET_X_LPARAM(lParam); 
            int yPos = GET_Y_LPARAM(lParam);
            rt.onClick(0,0,xPos,nHeight-yPos);
            }
        break;
        case WM_MOUSEWHEEL:
        glTranslatef(0,0,GET_WHEEL_DELTA_WPARAM(wParam));
        break;
		// Изменение размера окна
		case WM_SIZE:
		{
			nWidth  = LOWORD(lParam);		// Ширина
			nHeight = HIWORD(lParam);		// Высота
			glViewport(0, 0, nWidth, nHeight);	// Вьюпорт OpenGL

			glMatrixMode(GL_PROJECTION);		// Матрица проекции
			glLoadIdentity();					// Сбрасываем

			// Устанавливаем перспективу OpenGL (угол взгляда, аспект ширины и высоты, ближайшее и дальнее отсечение взгляда)
			gluPerspective( 120.0, (GLdouble)nWidth / (GLdouble)nHeight, 1, 50000.0);
            glMatrixMode(GL_MODELVIEW);rt.recalc();
		}
		break;
		// Если закрыли окно - выходим 
		case WM_CLOSE:
		{
			PostQuitMessage(0);	
		}

		// Если уничтожили окно - выходим
        case WM_DESTROY:
		{
            PostQuitMessage(0);
		}
        break;
		
		// Умолчание
		default:
		{
            //std::cout<<msg<<" : "<<wParam<<"\n";
			return DefWindowProc( hWnd, msg, wParam, lParam );
		}
		break;
	}

	return 0;
}
float hi=94;
GLuint index;
Parse* p;
img ii = BmpLoader("heightmap.bmp");
img nmap = BmpLoader("world_normal.bmp");
int step=2;
bool drawmap=false;
void generate(Node* n, Drawin* d)
{
    Drawin* c;
	if(n->name=="panel") c= new Drawin(n->values["x"],n->values["y"],n->values["w"],n->values["h"],n->values["name"], d);
	else if(n->name=="img") c = new Image(n->values["x"],n->values["y"],n->values["w"],n->values["h"],n->values["name"], d, ii);
	else if(n->name=="button") c= new Button(n->values["x"],n->values["y"],n->values["w"],n->values["h"],n->values["name"], d);
	else c= new Drawin(n->values["x"],n->values["y"],n->values["w"],n->values["h"],n->values["name"], d);
	
    d->child.push_back(c);
	for(int i =0;i<n->children.size();++i)
		generate(n->children[i], c);
}
Drawin* byname(Drawin* d,string name)
{
    if(d->name==name) return d;
    for(int i=0;i<d->child.size();++i)
    {
    Drawin* t = byname(d->child[i],name);
    if(t!=nullptr) return t;
    }
    return nullptr;
}
void list()
{
	index = glGenLists(1); 
    glNewList(index, GL_COMPILE);
    int sz=8;
    float mlt = 1;
    glColor3f(0.1f, 0.7f, 0.3f);glColor4f(1.f, 1.f, 1.f,0.5f);
    glPushMatrix();
    glRotatef(90,0,0,1);
    
    float fac=0;
	int txon=15;
    float tolow=hi*0.9;
	img gras = BmpLoader("grass.bmp");
	GLuint grasid;
	glGenTextures(1, &grasid);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, grasid);
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, gras.width, gras.height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, gras.data);
	glBegin(GL_TRIANGLES);
    for(int i = 0; i<ii.height-step;i+=step)
	  for(int j = 0; j<ii.width-step;j+=step)
	  {
		  int s=mlt*(int)ii.data[(i*ii.width+j)*3];
		  int l=mlt*(int)ii.data[(i*ii.width+j+step)*3];
		  int d=mlt*(int)ii.data[((i+step)*ii.width+j+step)*3];
		  int u=mlt*(int)ii.data[((i+step)*ii.width+j)*3];
          s=s<hi*mlt?tolow:s+fac;l=l<hi*mlt?tolow:l+fac;d=d<hi*mlt?tolow:d+fac;u=u<hi*mlt?tolow:u+fac;
		  int y1=j*sz,y2=j*sz+step*sz,y3=j*sz+step*sz;
		  int x1=i*sz,x2=i*sz,x3=i*sz+step*sz;
		  int z1=s,z2=l,z3=d;
		  float a = (y1-y2)*(z3-z2)-(z1-z2)*(y3-y2);
			float b =(z1-z2)*(x3-x2)-(x1-x2)*(z3-z2);
		float c =(x1-x2)*(y3-y2)-(y1-y2)*(x3-x2);
		//glNormal3f(a,b,c);
		unsigned char* clr = nmap.data+((i/2*nmap.width+j/2)*3);
		unsigned char* clr1 = nmap.data+((i/2*nmap.width+(j+1)/2)*3);
		unsigned char* clr2 = nmap.data+(((i+1)/2*nmap.width+(j+1)/2)*3);
		unsigned char* clr3 = nmap.data+(((i+1)/2*nmap.width+j/2)*3);
		glNormal3f(clr[2],clr[1],clr[0]);glTexCoord2f(i*(1.f/txon),(j)*(1.f/txon));glVertex3i(i*sz,j*sz,s);
		glNormal3f(clr1[2],clr1[1],clr1[0]);glTexCoord2f(i*(1.f/txon),(j+step)*(1.f/txon));glVertex3i(i*sz, j*sz+step*sz,l);
		glNormal3f(clr2[2],clr2[1],clr2[0]);glTexCoord2f((i+step)*(1.f/txon),(j+step)*(1.f/txon));glVertex3i(i*sz+step*sz,j*sz+step*sz,d);
		y1=j*sz,y2=j*sz,y3=j*sz+step*sz;
		x1=i*sz,x2=i*sz+step*sz,x3=i*sz+step*sz;
		z1=s,z2=u,z3=d;
		a = (y1-y2)*(z3-z2)-(z1-z2)*(y3-y2);
		b =(z1-z2)*(x3-x2)-(x1-x2)*(z3-z2);
		c =(x1-x2)*(y3-y2)-(y1-y2)*(x3-x2);
		//glNormal3f(-a,-b,-c);
		glNormal3f(clr[2],clr[1],clr[0]);glTexCoord2f((i)*(1.f/txon),(j)*(1.f/txon));glVertex3i(i*sz,j*sz,s);
		glNormal3f(clr3[2],clr3[1],clr3[0]);glTexCoord2f((i+step)*(1.f/txon),(j)*(1.f/txon));glVertex3i(i*sz+step*sz,j*sz,u);
		glNormal3f(clr2[2],clr2[1],clr2[0]);glTexCoord2f((i+step)*(1.f/txon),(j+step)*(1.f/txon));glVertex3i(i*sz+step*sz,j*sz+step*sz,d);
	  }
    glEnd();	glDisable(GL_TEXTURE_2D);glColor3f(0.0f, 0.0f, 1.0f);

    glBegin(GL_POLYGON);
	glNormal3f(1,1,1);
    glVertex3i(0,0,hi*mlt);
    glVertex3i(0,ii.width*sz,hi*mlt);
    glVertex3i(ii.height*sz,ii.width*sz,hi*mlt);
    glVertex3i(ii.height*sz,0,hi*mlt);
    glEnd();
    glPopMatrix();
    glEndList();       
}

void init( void )
{
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

    pfd.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion   = 1;
    pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 16;
	pfd.cDepthBits = 16;

	g_hDC = GetDC( g_hWnd );
	GLuint iPixelFormat = ChoosePixelFormat( g_hDC, &pfd );

	if( iPixelFormat != 0 )
	{
		PIXELFORMATDESCRIPTOR bestMatch_pfd;
		DescribePixelFormat( g_hDC, iPixelFormat, sizeof(pfd), &bestMatch_pfd );

		if( bestMatch_pfd.cDepthBits < pfd.cDepthBits )
		{
			return;
		}

		if( SetPixelFormat( g_hDC, iPixelFormat, &pfd) == FALSE )
		{
			DWORD dwErrorCode = GetLastError();
			return;
		}
	}
	else
	{
		DWORD dwErrorCode = GetLastError();
		return;
	}

	g_hRC = wglCreateContext( g_hDC );
	wglMakeCurrent( g_hDC, g_hRC );
    SelectObject (g_hDC, GetStockObject (SYSTEM_FONT));  
    wglUseFontBitmaps (g_hDC, 0, 255, 1000); 
	//glClearColor( 0.0f, 1.0f, 0.0f, 1.0f ); // Цвет вьюпорта OpenGL

    { 
    glShadeModel(GL_FLAT);
	ifstream ifs("gui.txt");
	Lex la(ifs);
    p=new Parse(la);
    
	try {while(true)
		generate(p->readNode(), &rt);}
	//catch(exception e) {cout<<e.what();}
    catch(invalid_argument e) {cout<<e.what();}
    ((Button*)byname(&rt,"valera"))->action=[](){--hi;list();};
    ((Button*)byname(&rt,"volodya"))->action=[](){++hi;list();};
	((Button*)byname(&rt,"step"))->action=[](){cin>>step;list();};
	((Button*)byname(&rt,"draw"))->action=[](){drawmap=!drawmap;};
	srand(time(0));
	glMatrixMode(GL_MODELVIEW);
	glRotatef(-12,1,0,0);
    glTranslatef(3000,-3000,-20000);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glPushMatrix();
      glLoadIdentity();
      glTranslatef(1.0, 1.0, 1000.0);
      GLfloat light0_position[] = { 0.0, 0.0, 0.0, 1.0 };
      glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
      glPopMatrix();
    list();
}
}

void shutDown( void )	
{
	if( g_hRC != NULL )
	{
		wglMakeCurrent( NULL, NULL );
		wglDeleteContext( g_hRC );
		g_hRC = NULL;
	}

	if( g_hDC != NULL )
	{
		ReleaseDC( g_hWnd, g_hDC );
		g_hDC = NULL;
	}
}

void render( void )	
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	if(drawmap)glCallList(index);
    //glFlush();
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0,(double)nWidth,0.0,(double)nHeight);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	rt.upd();
	glPopMatrix();
    glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	SwapBuffers( g_hDC );
}