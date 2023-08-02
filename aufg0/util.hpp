/** Ein Modul fuer ungebundene Hilfsfunktionen
 * 	@author Leonhard Brandes, Mario da Graca
*/

#pragma once

#include <opencv2/opencv.hpp>
#include <functional>

namespace util
{
	void for_each_pixel(cv::Mat &m, std::function<void(cv::Mat &, int, int)> func);

	void writeImgStdOut(cv::Mat m, std::string fileFormat);

	void showImage(cv::Mat m);

	std::string determineFileFormat(cv::Mat m);
}
