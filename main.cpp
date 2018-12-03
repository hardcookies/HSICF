/* HSICF.cpp */
#include "hsicf.h"

static void help()
{
	cout
		<< "\n--------------------------------------------------------------------------" << endl
		<< "This program shows how to process hyperspectral images with opencv, including " << endl
		<< "reading the headrer file, reading the BIL binary data, reading the spectrum " << endl
		<< "of a given pixel, showing a selected band image, classification and fusion." << endl
		<< "Usage:" << endl
		<< "HSICF hdrName dataName panImage" << endl
		<< endl;
}

int main(int argc, char *argv[])
{
	FILE *fp_hdr, *fp_data;
	char *prog = argv[0];			// 记下程序名
	char *hdr_file = argv[1];		// 记下头文件名
	char *data_file = argv[2];		// 记下数据文件名
	char *img_pan_file = argv[3];	// 记下全色图像名

	// 初始化指针为空
	unsigned char *data1 = 0;			// data type 1
	short *data2 = 0;					// data type 2

	/* ----------读取头文件---------- */
	int samples, lines, bands;		// 定义需要读取的变量

	if (argc != 4)							// 如果没有命令行参数，则返回错误
	{
		printf("参数输入格式不正确!\n");
		help();
		exit(1);
	}

	if ((fp_hdr = fopen(hdr_file, "r")) == NULL)
	{
		fprintf(stderr, "%s: can't open %s\n", prog, hdr_file);
		exit(2);
	}
	else
	{
		printf("Reading...\n");
		samples = readsamples(fp_hdr);
		lines = readlines(fp_hdr);
		bands = readbands(fp_hdr);
		fclose(fp_hdr); 
		printf("samples: %d\n", samples);
		printf("lines: %d\n", lines);
		printf("bands: %d\n", bands);
	}

	int datatype;		// 重新打开文件重读，避免遗漏
	fp_hdr = fopen(hdr_file, "r");
	datatype = readdatatype(fp_hdr);
	fclose(fp_hdr);
	printf("data type: %d\n", datatype);

	/* ----------读取BIL高光谱数据---------- */
	// 按照格式分别读取
	if ((fp_data = fopen(data_file, "rb")) == NULL)
	{
		fprintf(stderr, "%s: can't open %s\n", prog, data_file);
		exit(3);
	}
	int size = lines * samples * bands;		// 元素总数
	if (datatype == 1)
	{
		data1 = new unsigned char[size];
		fread(data1, sizeof(unsigned char), size, fp_data);
	}
	else if (datatype == 2)
	{
		data2 = new short[size];
		fread(data2, sizeof(short), size, fp_data);
	}
	else
	{
		fprintf(stderr, "data type error.\n");
		exit(4);
	}
	fclose(fp_data);

	// 将高光谱数据转换为图像立方体，保存对应格式
	vector<Mat> datacube;
	if (datatype == 1)
	{
		for (int i = 0; i < bands; ++i)
		{
			unsigned char *band_i = new unsigned char[lines*samples];
			band_i = readBand1(data1, i, samples, lines, bands);
			Mat image_i(lines, samples, CV_8U, band_i);
			datacube.push_back(image_i);
		}
	}
	else if (datatype == 2)
	{
		for (int i = 0; i < bands; ++i)
		{
			short *band_i = new short[lines*samples];
			band_i = readBand2(data2, i, samples, lines, bands);
			Mat image_i(lines, samples, CV_16S, band_i);
			datacube.push_back(image_i);
		}
	}
	else
	{
		fprintf(stderr, "data type error.\n");
		exit(4);
	}

	/* ----------图像融合部分---------- */
	// 读取全色影像
	Mat img_PAN = imread(img_pan_file, IMREAD_GRAYSCALE);

	// 读取RGB高光谱假彩色影像
	int BGR_num[3] = {7, 51, 229};
	vector<Mat> vHSI;
	for (int i = 0; i < 3; ++i)
	{
		Mat img_BGR;
		double min, max;
		minMaxLoc(datacube[BGR_num[i]], &min, &max, NULL, NULL);
		datacube[BGR_num[i]].convertTo(img_BGR, CV_8U, 255/(max-min), -255*min/(max-min));
		vHSI.push_back(img_BGR);
	}
	Mat img_HSI;
	merge(vHSI, img_HSI);

	if (!img_HSI.data || !img_PAN.data)
	{
		cout << "--(!) Error reading images" << endl; return -1;
	}
	//-- Step 1: 用SIFT算法选取特征点，并计算描述向量
	int minHessian = 400;

	SurfFeatureDetector detector(minHessian);

	std::vector<KeyPoint> keypoints_1, keypoints_2;

	detector.detect(img_HSI, keypoints_1);
	detector.detect(img_PAN, keypoints_2);

	//-- Step 2: Calculate descriptors (feature vectors)
	SurfDescriptorExtractor extractor;

	Mat descriptors_1, descriptors_2;

	extractor.compute(img_HSI, keypoints_1, descriptors_1);
	extractor.compute(img_PAN, keypoints_2, descriptors_2);

	//-- Step 2: 用FLANN方法匹配描述向量
	FlannBasedMatcher matcher;
	std::vector<DMatch> matches;
	matcher.match(descriptors_1, descriptors_2, matches);

	//-- Step 3: 选取较好的匹配点
	double max_dist = 0, min_dist = 100;
	for (int i = 0; i < descriptors_1.rows; ++i)
	{
		double dist = matches[i].distance;
		if (dist < min_dist)
			min_dist = dist;
		if (dist > max_dist)
			max_dist = dist;
	}
	printf("-- Max dist : %f\n", max_dist);
	printf("-- Min dist : %f\n", min_dist);
	std::vector<DMatch> good_matches;
	for (int i = 0; i < descriptors_1.rows; ++i)
	{
		if (matches[i].distance <= max(2 * min_dist, 0.02))
		{
			good_matches.push_back(matches[i]);
		}
	}

	//-- Step 4: 画出匹配点
	Mat img_matches;
	drawMatches(img_HSI, keypoints_1, img_PAN, keypoints_2,
		good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
		vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	imshow("匹配结果", img_matches);
	
	int match_num = good_matches.size();		// 实际匹配点的个数
	Point2f points_1[30];				// img_HSI中的匹配点
	Point2f points_2[30];				// img_RGB中的匹配点
	for (int i = 0; i < 30; ++i)
	{
		points_1[i] = keypoints_1[good_matches[i].queryIdx].pt;
		points_2[i] = keypoints_2[good_matches[i].trainIdx].pt;
	}
	printf("-- 匹配点个数：%d\n", match_num);
	
	//-- Step 5: 仿射变换
	Mat img_HSI_up;
	Mat trans_mat(2, 3, CV_32FC1);
	img_HSI_up = Mat::zeros(img_PAN.rows, img_PAN.cols, img_HSI.type());
	trans_mat = getAffineTransform(points_1, points_2);
	warpAffine(img_HSI, img_HSI_up, trans_mat, img_HSI_up.size());
	imwrite("match.jpg", img_matches);
	imwrite("matched.jpg", img_HSI_up);

	//-- Step 6: HSV融合
	/*
	Mat img_HSV;				// 将RGB转换到HSV的图像
	Mat img_HSV_trans;			// V用PAN代替后的图像
	Mat img_fused;				// 逆变换到RGB空间得到融合后的图像

	cvtColor(img_HSI_up, img_HSV, cv::COLOR_RGB2HSV);
	std::vector<Mat> vHSV;
	split(img_HSV, vHSV);
	vHSV[2] = img_PAN;
	merge(vHSV, img_HSV_trans);
	cvtColor(img_HSV_trans, img_fused, cv::COLOR_HSV2RGB);
	imshow("HSV_fused", img_fused);
	imwrite("HSV_fused.jpg", img_fused);
	*/
	// Step 7: PCA融合
	
	vector<Mat> datacube_HSI;
	split(img_HSI_up, datacube_HSI);

	// 将图像立方体转换为矩阵 (band)*(M*N)，运算部分全部转换为double类型
	Mat datamat = cube2mat(datacube_HSI);

	// 计算数据均值向量
	Mat row_mean;		// 均值列向量
	reduce(datamat, row_mean, 1, CV_REDUCE_AVG, CV_32F);

	// 中心化
	Mat O = Mat::ones(1, img_HSI_up.rows*img_HSI_up.cols, CV_32F);
	Mat mat_mean = row_mean*O;
	datamat = datamat - mat_mean;

	// 计算数据的协方差矩阵
	Mat C = datamat*datamat.t() / img_HSI_up.rows / img_HSI_up.cols;

	// 特征分解
	Mat eigenvalues, eigenvectors;		// eigenvalues数组降序排列，eigenvectors矩阵对应行为特征向量
	eigen(C, eigenvalues, eigenvectors);

	// PCA投影
	Mat mat_PCA = eigenvectors*datamat;

	// 将矩阵转化为图像立方体
	vector<Mat> cube_PCA;
	cube_PCA = mat2cube(mat_PCA, img_HSI_up.rows);

	// 显示主成分
	// Mat PC1;
	// cube_PCA[0].convertTo(PC1, CV_8U);
	// imshow("PC1", PC1);

	// 用全色代替PC1
	cube_PCA[0] = img_PAN;

	// 进行PCA逆变换
	Mat mat_PCA_trans, datamat_trans;
	mat_PCA_trans = cube2mat(cube_PCA);
	datamat_trans = eigenvectors.inv()*mat_PCA_trans;

	// 将变换后的矩阵转化成图像立方体
	vector<Mat> cube_PCA_trans;
	cube_PCA_trans = mat2cube(datamat_trans, img_HSI_up.rows);

	// 合成图像
	Mat img_PCA_fused;
	merge(cube_PCA_trans, img_PCA_fused);

	img_PCA_fused.convertTo(img_PCA_fused, CV_8U);
	imshow("PCA fused", img_PCA_fused);
	imwrite("PCA_fused.jpg", img_PCA_fused);

	waitKey(0);

	return 0;
}
