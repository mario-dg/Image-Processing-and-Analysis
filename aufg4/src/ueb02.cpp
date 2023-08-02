#include "ueb02.hpp"
#include "util.hpp"
#include "filter.hpp"
#include "ueb01.hpp"

#define AMOUNT_BINS_DICRIMINATOR (3)
#define THRESHOLD_DISCRIMINATOR (0.06f)

cv::Mat ueb02::values(cv::Mat img)
{
    return img.clone();
}

int ueb02::clampToEdge(int toClamp, int min, int max)
{
    if (toClamp > max)
    {
        toClamp = max;
    }
    else if (toClamp < min)
    {
        toClamp = min;
    }
    return toClamp;
}

int correctModulo(int a, int b)
{
    return (b + (a % b)) % b;
}

cv::Vec3b ueb02::reflect(argp::EDGE edgeopt, cv::Mat img, cv::Mat filter, int i, int k, int y, int x)
{
    int newY = y - i + filter.rows / 2;
    int newX = x - k + filter.cols / 2;
    if (edgeopt == argp::EDGE_REFLECT_11 || edgeopt == argp::EDGE_REFLECT_101)
    {
        int offset = 0;
        if (edgeopt == argp::EDGE_REFLECT_101)
        {
            offset = 1;
        }

        if (newY < 0)
        {
            if (correctModulo((newY + 1) / (img.rows - offset), 2) == 0)
            {
                newY = img.rows - 1 - (correctModulo(newY, img.rows - offset));
            }
            else
            {
                newY = (correctModulo(newY, img.rows - offset));
            }
        }
        else if (newY >= img.rows)
        {
            if (correctModulo(newY / (img.rows - offset), 2) == 1)
            {
                newY = img.rows - 1 - (correctModulo(newY, img.rows - offset));
            }
            else
            {
                newY = (correctModulo(newY, img.rows - offset));
            }
        }

        if (newX < 0)
        {
            if (correctModulo((newX + 1) / (img.cols - offset), 2) == 0)
            {
                newX = img.cols - 1 - (correctModulo(newX, img.cols - offset));
            }
            else
            {
                newX = (correctModulo(newX, img.cols - offset));
            }
        }
        else if (newX >= img.cols)
        {
            if (correctModulo(newX / (img.cols - offset), 2) == 1)
            {
                newX = img.cols - 1 - (correctModulo(newX, img.cols - offset));
            }
            else
            {
                newX = (correctModulo(newX, img.cols - offset));
            }
        }
    }
    return filter.at<float>(i, k) * img.at<cv::Vec3b>(newY, newX);
}

cv::Mat ueb02::fold(cv::Mat img, cv::Mat filter, argp::EDGE edgeOpt, float edgeValue)
{
    cv::Mat result = cv::Mat::zeros(img.rows, img.cols, CV_8UC3);
    edgeValue = CLAMP(edgeValue, 0, 1) * 255;
    util::for_each_pixel(result, [img, filter, edgeOpt, edgeValue](cv::Mat &result, int y, int x) -> void {
        for (int c = 0; c < img.channels(); c++)
        {

            int i = 0;
            int k = 0;

            for (i = 0; i < filter.rows; i++)
            {
                for (k = 0; k < filter.cols; k++)
                {
                    if (((y - i + filter.rows / 2) < 0) || ((y - i + filter.rows / 2) >= img.rows) || ((x - k + filter.cols / 2) < 0) || ((x - k + filter.cols / 2) >= img.cols))
                    {
                        switch (edgeOpt)
                        {
                        case argp::EDGE_NULL:
                        case argp::EDGE_VALUE:
                            result.at<cv::Vec3b>(y, x)[c] += filter.at<float>(i, k) * edgeValue;
                            break;
                        case argp::EDGE_REPLICATE:
                            result.at<cv::Vec3b>(y, x)[c] += img.at<cv::Vec3b>(ueb02::clampToEdge(y - i + filter.rows / 2, 0, img.rows - 1), ueb02::clampToEdge(x - k + filter.cols / 2, 0, img.cols - 1))[c] * filter.at<float>(i, k);
                            break;
                        case argp::EDGE_REFLECT_11:
                            result.at<cv::Vec3b>(y, x)[c] += ueb02::reflect(argp::EDGE_REFLECT_11, img, filter, i, k, y, x)[c];
                            break;
                        case argp::EDGE_REFLECT_101:
                            result.at<cv::Vec3b>(y, x)[c] += ueb02::reflect(argp::EDGE_REFLECT_101, img, filter, i, k, y, x)[c];
                            break;
                        }
                    }
                    else
                    {
                        result.at<cv::Vec3b>(y, x)[c] += filter.at<float>(i, k) * img.at<cv::Vec3b>(y - i + filter.rows / 2, x - k + filter.cols / 2)[c];
                    }
                }
            }
        }
    });
    return result;
}