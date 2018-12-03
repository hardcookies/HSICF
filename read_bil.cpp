#include "hsicf.h"

// readSpec1����������ָ��λ�ù������ݣ�1-byte unsigned integer��
unsigned char *readSpec1(unsigned char *data, int i, int j, int samples, int lines, int bands)
{
	unsigned char *spectrum = new unsigned char[bands];

	for (int k = 0; k < bands; ++k)
	{
		spectrum[k] = data[bands * samples * i + j + samples * k];
	}
	return spectrum;
}

// readSpec2����������ָ��λ�ù������ݣ�2-byte signed integer��
short *readSpec2(short *data, int i, int j, int samples, int lines, int bands)
{
	short *spectrum = new short[bands];

	for (int k = 0; k < bands; ++k)
	{
		spectrum[k] = data[bands * samples * i + j + samples * k];
	}
	return spectrum;
}

// readBand1����������ָ���������ݣ�1-byte unsigned integer��
unsigned char *readBand1(unsigned char *data, int k, int samples, int lines, int bands)
{
	unsigned char *image = new unsigned char[samples*lines];
	for (int i = 0; i < lines; ++i)
	{
		for (int j = 0; j < samples; ++j)
		{
			image[i * samples + j] = data[samples * bands * i + samples * k + j];
		}
	}
	return image;
}

// readBand2����������ָ���������ݣ�2-byte signed integer��
short *readBand2(short *data, int k, int samples, int lines, int bands)
{
	short *image = new short [samples*lines];
	for (int i = 0; i < lines; ++i)
	{
		for (int j = 0; j < samples; ++j)
		{
			image[i * samples + j] = data[samples * bands * i + samples * k + j];
		}
	}
	return image;
}

