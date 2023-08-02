#include "util.hpp"
#include <iostream>
#include <iomanip>

void util::for_each_pixel(cv::Mat3f &m, std::function<void(cv::Mat3f &, int, int)> func)
{
	for (int y = 0; y < m.rows; y++)
	{
		for (int x = 0; x < m.cols; x++)
		{
			func(m, y, x);
		}
	}
}

void util::for_each_pixel(cv::Mat &m, std::function<void(cv::Mat &, int, int)> func)
{
	for (int y = 0; y < m.rows; y++)
	{
		for (int x = 0; x < m.cols; x++)
		{
			func(m, y, x);
		}
	}
}

void util::writeImgStdOut(cv::Mat m, std::string fileFormat)
{
	// Flag CV_IMWRITE_PXM_BINARY 1(standard) Ausgabe als Binary
	// setzen auf 0 -> nonBinary Ausgabe
	std::vector<int> outputParams;
	outputParams.push_back(CV_IMWRITE_PXM_BINARY);
	outputParams.push_back(0);

	// Buffer mit den Bilddaten
	std::vector<unsigned char> buffer;

	cv::imencode(fileFormat, m, buffer, outputParams);
	for (unsigned int i = 0; i < buffer.size(); i++)
	{
		std::cout << buffer.at(i);
	}
}

void util::showImage(cv::Mat m)
{ 
	cv::imshow("image", m);
	cv::waitKey();
}

void util::printHistogram(cv::Mat histo)
{
	std::cout << '[';
	for (int i = 0; i < histo.rows; i++)
	{
		// Ausgabe des Floats mit 2 Nachkommastellen
		std::cout << std::round(histo.at<float>(i, 0) * 100) / 100;
		if (i < histo.rows - 1)
		{
			std::cout << ", ";
		}
	}
	std::cout << ']' << std::endl;
}

