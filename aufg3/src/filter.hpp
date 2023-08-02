#include <opencv2/opencv.hpp>
#define value 0.020408f

namespace filter
{
    const cv::Mat boxFilter = (cv::Mat_<float>(3, 3) << 1.0f / 9, 1.0f / 9, 1.0f / 9, 1.0f / 9, 1.0f / 9, 1.0f / 9, 1.0f / 9, 1.0f / 9, 1.0f / 9);
    const cv::Mat gaussianFilter = (cv::Mat_<float>(5, 5) << 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                    0.0f, 1.0f / 16, 2.0f / 16, 1.0f / 16, 0.0f,
                                    0.0f, 2.0f / 16, 4.0f / 16, 2.0f / 16, 0.0f,
                                    0.0f, 1.0f / 16, 2.0f / 16, 1.0f / 16, 0.0f,
                                    0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    const cv::Mat sobelXFilter = (cv::Mat_<float>(3, 3) << -1.0f, 0.0f, 1.0f,
                                  -2.0f, 0.0f, 2.0f,
                                  -1.0f, 0.0f, 1.0f);
    const cv::Mat sobelYFilter = (cv::Mat_<float>(3, 3) << -1.0f, -2.0f, -1.0f,
                                  0.0f, 0.0f, 0.0f,
                                  1.0f, 2.0f, 1.0f);
    const cv::Mat laplaceFilter = (cv::Mat_<float>(3, 3) << 0.0f, 1.0f, 0.0f,
                                   1.0f, -4.0f, 1.0f,
                                   0.0f, 1.0f, 0.0f);

} // namespace filter