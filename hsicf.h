#ifndef HSICF_H
#define HSICF_H

#include <iostream>
#include <sstream>

#include "opencv2/opencv_modules.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/features2d.hpp"

using namespace std;
using namespace cv;

/* ----------��������ĺ���---------- */

// �����ĸ���ȡ���������������Ρ����͵ĺ���
int readsamples(FILE *);
int readlines(FILE *);
int readbands(FILE *);
int readdatatype(FILE *);

// �����ȡ���׺Ͳ��εĺ������ֱ��Ӧdata type 1, 2
unsigned char *readSpec1(unsigned char *bil_data, int row, int col, int samples, int lines, int bands);		/* ����ָ�� */
unsigned char *readBand1(unsigned char *bil_data, int band_num, int samples, int lines, int bands);			/* ����ָ�� */
short *readSpec2(short *data, int i, int j, int samples, int lines, int bands);
short *readBand2(short *data, int k, int samples, int lines, int bands);


// ��������������;�����໥ת������
Mat cube2mat(const vector<Mat> &cube);
vector<Mat> mat2cube(const Mat &matrix, int rows);			/* ��������Ϊ32F */

#endif