#include "hsicf.h"

#define MAXLINE 20

/* readsamples��������ȡͷ�ļ���samples��ֵ */
int readsamples(FILE *ifp)
{
	int samples = 0;				// ��ʼ������ֵ
	char line[MAXLINE];				// ��ȡ��һ��
	char item[MAXLINE];				// ��ȡ����������
	int value;						// ��ȡ������ֵ

	while ((fgets(line, MAXLINE, ifp)) != 0)
	{
		sscanf(line, "%[^=]", item);				// ��ȡ�Ⱥ�֮ǰ������
		sscanf(line, "%*[^=]=%d", &value);			// ��ȡ�Ⱥ�֮������ݣ�����ע���������Ϊָ����Ҫ��&��
		if ((strcmp(item, "samples")) >= 0)
			break;
	}
	samples = value;
	return samples;
}

/* readlines��������ȡͷ�ļ���samples��ֵ */
int readlines(FILE *ifp)
{
	int lines = 0;					// ��ʼ������ֵ
	char line[MAXLINE];				// ��ȡ��һ��
	char item[MAXLINE];				// ��ȡ����������
	int value;						// ��ȡ������ֵ

	while ((fgets(line, MAXLINE, ifp)) != 0)
	{
		sscanf(line, "%[^=]", item);				// ��ȡ�Ⱥ�֮ǰ������
		sscanf(line, "%*[^=]=%d", &value);			// ��ȡ�Ⱥ�֮������ݣ�����ע���������Ϊָ����Ҫ��&��
		if ((strcmp(item, "lines")) >= 0)
			break;
	}
	lines = value;
	return lines;
}

/* readbands��������ȡͷ�ļ���samples��ֵ */
int readbands(FILE *ifp)
{
	int bands = 0;					// ��ʼ������ֵ
	char line[MAXLINE];				// ��ȡ��һ��
	char item[MAXLINE];				// ��ȡ����������
	int value;						// ��ȡ������ֵ

	while ((fgets(line, MAXLINE, ifp)) != 0)
	{
		sscanf(line, "%[^=]", item);				// ��ȡ�Ⱥ�֮ǰ������
		sscanf(line, "%*[^=]=%d", &value);			// ��ȡ�Ⱥ�֮������ݣ�����ע���������Ϊָ����Ҫ��&��
		if ((strcmp(item, "bands")) >= 0)
			break;
	}
	bands = value;
	return bands;
}

/* readdatatype��������ȡͷ�ļ���datatype��ֵ */
int readdatatype(FILE *ifp)
{
	int type = 0;					// ��ʼ������ֵ
	char line[MAXLINE];				// ��ȡ��һ��
	char item[MAXLINE];				// ��ȡ����������
	int value;						// ��ȡ������ֵ

	while ((fgets(line, MAXLINE, ifp)) != 0)
	{
		sscanf(line, "%[^=]", item);				// ��ȡ�Ⱥ�֮ǰ������
		sscanf(line, "%*[^=]=%d", &value);			// ��ȡ�Ⱥ�֮������ݣ�����ע���������Ϊָ����Ҫ��&��
		if ((strncmp(item, "data type", 9)) == 0)
			break;
	}
	type = value;
	return type;
}