#include "hsicf.h"

// 将数据立方体转换成二维矩阵，且数据类型为32F
Mat cube2mat(const vector<Mat> &cube)
{
	Mat dst(cube.size(), cube[0].rows*cube[0].cols, CV_32F);
	for (int i = 0; i < cube.size(); ++i)
	{
		Mat image_row = cube[i].clone().reshape(1, 1);		/* (cn-number of channels, rows-number of rows) */
		Mat row_i = dst.row(i);		/* 头文件复制，实际操作同一矩阵 */
		image_row.convertTo(row_i, CV_32F);
	}
	return dst;
}

// 将矩阵转化为图像立方体，数据类型为32F
vector<Mat> mat2cube(const Mat &matrix, int rows)
{
	vector<Mat> cube;
	int bands = matrix.rows;
	for (int i = 0; i < bands; ++i)
	{
		Mat image_i = matrix.row(i).reshape(1, rows);
		cube.push_back(image_i);
	}
	return cube;
}