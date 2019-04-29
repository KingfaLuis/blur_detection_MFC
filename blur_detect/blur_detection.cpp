#include <cv.h>
#include <highgui.h>
#include <iostream>

using namespace std;

double Tenegrad(IplImage* src)//Tenegrad ��׼
{
	assert(src->nChannels == 8);
	int row = src->height;//height��Ӧ����
	int col = src->width; //width��Ӧ����
	int widthstep = src->widthStep;
	char *data = src->imageData;
	double S = 0;
	for (int x = 1; x < row - 1; x++)
	{
		char *pre_row = data + (x - 1)*widthstep;
		char *cur_row = data + x * widthstep;
		char *nex_row = data + (x + 1)*widthstep;
		int Sx, Sy;
		for (int y = 1; y < col - 1; y++)
		{
			//**********************************************/
			//��ǰ����
			//pre_row[y-1],pre_row[y],pre_row[y+1];
			//cur_row[y-1],cur_row[y],cur_row[y+1];
			//nex_row[y-1],nex_row[y],nex_row[y+1];

			//Gx =-1,0,1       Gy =1, 2, 1
			//    -2,0,2           0, 0, 0
			//    -1,0,1          -1,-2,-1
			//**********************************************/
			Sx = (uchar)pre_row[y + 1] + 2 * (uchar)cur_row[y + 1] + (uchar)nex_row[y + 1]//һ��ҪתΪuchar
				- (uchar)pre_row[y - 1] - 2 * (uchar)cur_row[y - 1] - (uchar)nex_row[y - 1];
			Sy = (uchar)nex_row[y - 1] + 2 * (uchar)nex_row[y] + (uchar)nex_row[y + 1]
				- (uchar)pre_row[y - 1] - 2 * (uchar)pre_row[y] - (uchar)pre_row[y + 1];
			S += Sx * Sx + Sy * Sy;
		}
	}
	return S / (row - 2) / (col - 2);
}

int main()
{
	IplImage* src = cvLoadImage("E:\\kindfa\\test_image\\yuanping.bmp", 0);
	cvNamedWindow("src");
	cvShowImage("src", src);
	cout << Tenegrad(src);
	cvWaitKey(0);
	cvReleaseImage(&src);
	cvDestroyWindow("src");
	return 0;
}