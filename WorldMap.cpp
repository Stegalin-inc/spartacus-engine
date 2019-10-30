#ifndef WORLDMAP
#define WORDLMAP
#include "BmpLoader.h"
#include "GL/gl.h"
class WorldMap{
    public:
    float hi=94;
    GLuint index;
    img ii = BmpLoader("heightmap.bmp");
    img nmap = BmpLoader("world_normal.bmp");
    int step=2;
    WorldMap()
    {
        list();
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
};
#endif