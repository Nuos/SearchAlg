#ifndef MYPOINT_H
#define MYPOINT_H

#include <atltypes.h>
using namespace std;
struct MyPoint{
	MyPoint(){//Ĭ�Ϲ��캯��
		x = 0.0;
		y = 0.0;
	}
	MyPoint(double x1, double y1){//���캯��
		x = x1;
		y = y1;
	}
	double x;
	double y;
	CPoint CvtToCP_Grid(double xRatio, double yRatio, int xPadding, int yPadding);//դ����������ʵ����ת��Ϊ��Ļ����
	CPoint CvtToCP_Square(double xRatio, double yRatio, int xPadding, int yPadding, int centerX, int centerY);//������������ʵ����ת��Ϊ��Ļ����

};
#endif