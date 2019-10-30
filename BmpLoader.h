#ifndef BMPLOADER
#define BMPLOADER
#include <string>
#include <cstdio>
struct img{unsigned char* data; unsigned int width, height;img(unsigned char* d, unsigned int w, unsigned int h):data(d),width(w),height(h){}};
img BmpLoader(std::string path)
{
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int width, height;
	unsigned int imageSize;
	unsigned char * data;
	FILE * file = fopen(path.c_str(),"rb");
	if (!file) {
		printf("����������� �� ����� ���� �������n");
		//return 0;
	}
	if ( fread(header, 1, 54, file) != 54 ) {
		printf("������������ BMP-����n");
		//return false;
	}
	if ( header[0]!='B' || header[1]!='M' ){
		printf("������������ BMP-����n");
		//return 0;
	}
	dataPos    = *(int*)&(header[0x0A]); // �������� ������ ����������� � �����
	imageSize  = *(int*)&(header[0x22]); // ������ ����������� � ������
	width      = *(int*)&(header[0x12]); // ������
	height     = *(int*)&(header[0x16]); // ������
	if (imageSize==0)    imageSize=width*height*3; // ������ * ������ * 3, ��� 3 - 3 ���������� ����� (RGB)
	if (dataPos==0)      dataPos=54;
	data = new unsigned char [imageSize];
	fread(data,1,imageSize,file);
	fclose(file);
	
	return img(data, width, height);
}
#endif