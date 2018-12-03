#include "hsicf.h"

#define MAXLINE 20

/* readsamples函数：读取头文件中samples的值 */
int readsamples(FILE *ifp)
{
	int samples = 0;				// 初始化返回值
	char line[MAXLINE];				// 读取的一行
	char item[MAXLINE];				// 读取的属性名称
	int value;						// 读取的属性值

	while ((fgets(line, MAXLINE, ifp)) != 0)
	{
		sscanf(line, "%[^=]", item);				// 读取等号之前的内容
		sscanf(line, "%*[^=]=%d", &value);			// 读取等号之后的内容，这里注意参数类型为指针需要‘&’
		if ((strcmp(item, "samples")) >= 0)
			break;
	}
	samples = value;
	return samples;
}

/* readlines函数：读取头文件中samples的值 */
int readlines(FILE *ifp)
{
	int lines = 0;					// 初始化返回值
	char line[MAXLINE];				// 读取的一行
	char item[MAXLINE];				// 读取的属性名称
	int value;						// 读取的属性值

	while ((fgets(line, MAXLINE, ifp)) != 0)
	{
		sscanf(line, "%[^=]", item);				// 读取等号之前的内容
		sscanf(line, "%*[^=]=%d", &value);			// 读取等号之后的内容，这里注意参数类型为指针需要‘&’
		if ((strcmp(item, "lines")) >= 0)
			break;
	}
	lines = value;
	return lines;
}

/* readbands函数：读取头文件中samples的值 */
int readbands(FILE *ifp)
{
	int bands = 0;					// 初始化返回值
	char line[MAXLINE];				// 读取的一行
	char item[MAXLINE];				// 读取的属性名称
	int value;						// 读取的属性值

	while ((fgets(line, MAXLINE, ifp)) != 0)
	{
		sscanf(line, "%[^=]", item);				// 读取等号之前的内容
		sscanf(line, "%*[^=]=%d", &value);			// 读取等号之后的内容，这里注意参数类型为指针需要‘&’
		if ((strcmp(item, "bands")) >= 0)
			break;
	}
	bands = value;
	return bands;
}

/* readdatatype函数：读取头文件中datatype的值 */
int readdatatype(FILE *ifp)
{
	int type = 0;					// 初始化返回值
	char line[MAXLINE];				// 读取的一行
	char item[MAXLINE];				// 读取的属性名称
	int value;						// 读取的属性值

	while ((fgets(line, MAXLINE, ifp)) != 0)
	{
		sscanf(line, "%[^=]", item);				// 读取等号之前的内容
		sscanf(line, "%*[^=]=%d", &value);			// 读取等号之后的内容，这里注意参数类型为指针需要‘&’
		if ((strncmp(item, "data type", 9)) == 0)
			break;
	}
	type = value;
	return type;
}