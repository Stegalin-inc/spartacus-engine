#ifndef DRAWIN
#define DRAWIN
#include <vector>
#include <iostream>
#include <functional>
#include <string>
#include <cstdio>
#include <ctime>
#include <cmath>
#include "Reflection.cpp"
#include "Calc.cpp"
#include "BmpLoader.h"
#include "Gobj.cpp"


class Drawin:public Gobj
{
public:
	float r,g,b;
	std::string name;
	std::vector<Drawin*> child;
	Calc *xcalc,*ycalc,*wcalc,*hcalc;
	bool ready;
	bool anim;
	unsigned int start;
	int oldw;
	Drawin* par;
	Drawin(int xx, int yy, int ww, int hh, std::string s)
	{VAR(x);VAR(y);VAR(w);VAR(h);VAR(name);
		nextcol();
		x=xx;y=yy;w=ww;h=hh;
		name = s;
	}
	Drawin(std::string  xx, std::string  yy, std::string  ww, std::string  hh, std::string s, Drawin* parent)
	{par=parent;VAR(w);VAR(h);VAR(x);VAR(y);
		ready=false;anim=false;
		nextcol();
		Lex xlex(xx);Lex ylex(yy);Lex wlex(ww);Lex hlex(hh);
	    xcalc = new Calc(xlex);ycalc = new Calc(ylex);wcalc = new Calc(wlex);hcalc = new Calc(hlex);
		/* xcalc->addvar(&(parent->x),"parx");ycalc->addvar(&(parent->x),"parx");wcalc->addvar(&(parent->x),"parx");hcalc->addvar(&(parent->x),"parx");
		xcalc->addvar(&(parent->y),"pary");ycalc->addvar(&(parent->y),"pary");wcalc->addvar(&(parent->y),"pary");hcalc->addvar(&(parent->y),"pary");
		xcalc->addvar(&(parent->w),"parw");ycalc->addvar(&(parent->w),"parw");wcalc->addvar(&(parent->w),"parw");hcalc->addvar(&(parent->w),"parw");
		xcalc->addvar(&(parent->h),"parh");ycalc->addvar(&(parent->h),"parh");wcalc->addvar(&(parent->h),"parh");hcalc->addvar(&(parent->h),"parh");
		 */ready = true;
		name = s;
		recalc();
		
		
	}
	
	int getww()
	{
		static unsigned int duration = 500;
		static int targh = 1000;
		const float pi = 3.14159265359;
		if(anim)
		{
			unsigned int delta = clock()-start;
			recalc();
			if(delta>duration){anim=false;int t = w;w=targh;targh=oldw;return w;}
			//float phase = (1+tanhf(-2+4*(float)delta/(float)duration))/2;
			float phase = sin(0.5*pi*(((float)delta)/((float)duration)));
			w = (int)(oldw-((oldw-targh)*phase));
			return w;

		}
		else return w;
	}
	Drawin* byname(string nam)
	{
		if(name==nam) return this;
		//cout<<name+" "<<nam<<"; ";
		for(int i=0;i<child.size();++i)
		{
			Drawin* t = child[i]->byname(nam);
			if(t!=nullptr) return t;
		}
		//throw invalid_argument(nam+" not fount");
		return nullptr;
	}
	void recalc()
	{
		if(ready)
		{
			x=(int)xcalc->calc(par);//cout<<"X"<<x<<":";
			y=(int)ycalc->calc(par);//cout<<"Y"<<x<<":";
			w=(int)wcalc->calc(par);//cout<<"W"<<x<<":";
			h=(int)hcalc->calc(par);//cout<<"H"<<x<<":";
			//std::cout<<name;printf(" %d %d %d %d\n",x,y,w,h);
		}
		for(int i=0;i<child.size();++i)child[i]->recalc();
	}
	void nextcol()
	{
		r = (rand()%255)/255.0;
		g = (rand()%255)/255.0;
		b = (rand()%255)/255.0;
	}
	void upd()
	{
		
		glPushMatrix(); 
		glTranslatef(x,y,0);
		glColor3f(r,g,b);
		onDraw();
		for(int i=child.size()-1;i>=0;--i)child[i]->upd();
		glPopMatrix();
	}
	virtual void onClick(int a, int b, int c, int d)
	{
		
		bool f=true;
		
		int inmyx = c-x;
		int inmyy = d-y;
		
		for(int i = child.size()-1;i>=0;--i)
			if(inmyx>child[i]->x&&inmyy>child[i]->y&&inmyx<(child[i]->x+child[i]->w)&&inmyy<(child[i]->y+child[i]->h))
			{
				child[i]->onClick(a,b,inmyx,inmyy);
				f=false;
			}
		if(f)
		{
			nextcol();
			if(!anim){start = clock();oldw=w;
			anim=true;}
			
			std::cout<<name<< " "<< inmyx<<" "<<inmyy<<'\n';
		}
	}

	virtual void onDraw()
	{
		glBegin(GL_POLYGON);
		glVertex2i(0, 0);
		glVertex2i( 0, h);
		glVertex2i(getww(),  h);
		glVertex2i(getww(),  0);
		glEnd();
	}
	~Drawin(void){};
};

class Image:public Drawin
{public:
	img* ii;
	GLuint textureID;
	Image(std::string  xx, std::string  yy, std::string  ww, std::string  hh, std::string s, Drawin* parent,img& i):Drawin(xx,yy,ww,hh,s,parent),ii(&i)
	{
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, ii->width, ii->height, 0, GL_RGB, GL_UNSIGNED_BYTE, ii->data);
		glBindTexture(GL_TEXTURE_2D, 0);
		printf("%d, %d",ii->width,ii->height);
	}
	
	virtual void onClick(int a, int b, int c, int d)
	{}
	virtual void onDraw()
	{
		glEnable(GL_TEXTURE_2D);
		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glColor4f(1,1,1,0.5);
		glBegin(GL_QUADS);
		glTexCoord2f(0,0); glVertex2i(0, 0);
		glTexCoord2f(0,1); glVertex2i( 0, h);
		glTexCoord2f(1,1); glVertex2i(getww(),  h);
		glTexCoord2f(1,0); glVertex2i(getww(),  0);glTexCoord2f(0,0); 
		glEnd();
		glDisable(GL_TEXTURE_2D);
		/* glBegin(GL_QUADS);
		glVertex2i(0, 0);
		glVertex2i( 0, h);
		glVertex2i(getww(),  h);
		glVertex2i(getww(),  0); 
		glEnd(); */
	}
};
class Button: public Drawin
{
	public:
	std::function<void(void)> action;
	Button(std::string  xx, std::string  yy, std::string  ww, std::string  hh, std::string s, Drawin* parent):Drawin(xx,yy,ww,hh,s,parent){}
	virtual void onClick(int a, int b, int c, int d)
	{//Drawin::onClick(a,b,c,d);
	action();}

};
//
#endif