#include "util.hpp"

/**
 * Durchlaeuft jeden Pixel eines Bildes und wendet die uebergebene Funktion an
 * 
 * @param[in] cv::Mat, Bild
 * @param[in] std::function, Funktion
 */
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

/**
 * Schreibt die Daten eines Bildes auf stdout
 * 
 * @param[in] cv::Mat, Bild
 * @param[in] std::string, Dateiformat
 */
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

/**
 * Zeigt ein Bild solange an, bis eine Taste gedrueckt wird
 * 
 * @param[in] cv::Mat, Bild
 */
void util::showImage(cv::Mat m)
{
	cv::imshow("image", m);
	cv::waitKey();
}

/**
 * Liefert eine Dateiendung, zugehoerig zum eingelesenen PNM Bild
 * 
 * @param[in] cv::Mat, Bild
 * @return std::string, Dateiendung
 */ 
std::string util::determineFileFormat(cv::Mat m)
{
	int amountChannels = m.channels();
	std::string fileFormat;

	switch (amountChannels)
	{
	case 0:
		fileFormat = ".pbm";
		break;
	case 1:
		fileFormat = ".pgm";
		break;
	default:
		fileFormat = ".ppm";
	}
	return fileFormat;
}
