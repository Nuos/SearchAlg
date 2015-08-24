#include "stdafx.h"
#include "MyPoint.h"

//դ����������ʵ����ת��Ϊ��Ļ����
CPoint MyPoint::CvtToCP_Grid(double xRatio, double yRatio, int xPadding, int yPadding)
{
	return CPoint(int(xRatio*x + xPadding), int(yRatio*y + yPadding));
}

//������������ʵ����ת��Ϊ��Ļ����
CPoint MyPoint::CvtToCP_Square(double xRatio, double yRatio, int xPadding, int yPadding,int centerX,int centerY)
{
	//��centerX��centerYΪ����ϵԭ��
	return CPoint(int(centerX+x*xRatio) , int(centerY-y*yRatio));
}