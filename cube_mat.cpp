#include "hsicf.h"

// ������������ת���ɶ�ά��������������Ϊ32F
Mat cube2mat(const vector<Mat> &cube)
{
	Mat dst(cube.size(), cube[0].rows*cube[0].cols, CV_32F);
	for (int i = 0; i < cube.size(); ++i)
	{
		Mat image_row = cube[i].clone().reshape(1, 1);		/* (cn-number of channels, rows-number of rows) */
		Mat row_i = dst.row(i);		/* ͷ�ļ����ƣ�ʵ�ʲ���ͬһ���� */
		image_row.convertTo(row_i, CV_32F);
	}
	return dst;
}

// ������ת��Ϊͼ�������壬��������Ϊ32F
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