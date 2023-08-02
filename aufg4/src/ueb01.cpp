#include "ueb01.hpp"
#include "util.hpp"

cv::Mat ueb01::filterBrightness(cv::Mat m, float a)
{
    return ueb01::filterLinear(m, 1, a);
}

cv::Mat ueb01::filterFactor(cv::Mat m, float a)
{
    return ueb01::filterLinear(m, a, 0);
}

cv::Mat ueb01::filterLinear(cv::Mat m, float a, float b)
{
    cv::Mat result;
    if (m.channels() == 3)
    {
        result = cv::Mat::zeros(m.rows, m.cols, CV_8UC3);
        util::for_each_pixel(result, [a, b, m](cv::Mat &result, int y, int x) -> void {
            for (int i = 0; i < result.channels(); i++)
            {
                int temp = m.at<cv::Vec3i>(y, x)[i] * a + (b * 255);
                result.at<cv::Vec3b>(y, x)[i] = CLAMP(temp, 0, 255);
            }
        });
    }
    else
    {
        result = cv::Mat::zeros(m.rows, m.cols, CV_32FC1);
        util::for_each_pixel(result, [a, b, m](cv::Mat &result, int y, int x) -> void {
            float temp = m.at<float>(y, x) * a + b;
            std::cerr << "temp: " << temp << std::endl;
            result.at<float>(y, x) = CLAMP(temp, 0, 1);
        });
    }

    return result;
}
