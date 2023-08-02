#include "ueb00.hpp"
#include "util.hpp"
#include <iostream>

void ueb00::identity(cv::Mat& img) {
	util::for_each_pixel(img, [](cv::Mat& m, int y, int x) -> void {
		m.at<float>(y, x) = m.at<float>(y, x);
		});
}
