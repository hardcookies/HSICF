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

/* ----------声明定义的函数---------- */

// 定义四个读取列数、行数、波段、类型的函数
int readsamples(FILE *);
int readlines(FILE *);
int readbands(FILE *);
int readdatatype(FILE *);

// 定义读取光谱和波段的函数，分别对应data type 1, 2
unsigned char *readSpec1(unsigned char *bil_data, int row, int col, int samples, int lines, int bands);		/* 返回指针 */
unsigned char *readBand1(unsigned char *bil_data, int band_num, int samples, int lines, int bands);			/* 返回指针 */
short *readSpec2(short *data, int i, int j, int samples, int lines, int bands);
short *readBand2(short *data, int k, int samples, int lines, int bands);


// 定义数据立方体和矩阵的相互转换函数
Mat cube2mat(const vector<Mat> &cube);
vector<Mat> mat2cube(const Mat &matrix, int rows);			/* 数据类型为32F */

#endif