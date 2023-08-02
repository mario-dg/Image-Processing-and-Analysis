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

int clampToEdge(int toClamp, int min, int max)
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

void reflect(argp::EDGE edgeopt, cv::Mat img, cv::Mat filter, int i, int k, cv::Mat &result, int y, int x)
{
    if (edgeopt == argp::EDGE_REFLECT_11 || edgeopt == argp::EDGE_REFLECT_101)
    {

        int newY = y - i + filter.rows / 2;
        int newX = x - k + filter.cols / 2;
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
        result.at<float>(y, x) += filter.at<float>(i, k) * img.at<float>(newY, newX);
    }
}

cv::Mat ueb02::fold(cv::Mat img, cv::Mat filter, argp::EDGE edgeOpt, float edgeValue)
{
    cv::Mat result = cv::Mat::zeros(img.rows, img.cols, CV_32F);
    util::for_each_pixel(result, [img, filter, edgeOpt, edgeValue](cv::Mat &result, int y, int x) -> void {
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
                        result.at<float>(y, x) += filter.at<float>(i, k) * edgeValue;
                        break;
                    case argp::EDGE_REPLICATE:
                        result.at<float>(y, x) += img.at<float>(clampToEdge(y - i + filter.rows / 2, 0, img.rows - 1), clampToEdge(x - k + filter.cols / 2, 0, img.cols - 1)) * filter.at<float>(i, k);
                        break;
                    case argp::EDGE_REFLECT_11:
                        reflect(argp::EDGE_REFLECT_11, img, filter, i, k, result, y, x);
                        break;
                    case argp::EDGE_REFLECT_101:
                        reflect(argp::EDGE_REFLECT_101, img, filter, i, k, result, y, x);
                        break;
                    }
                }
                else
                {
                    result.at<float>(y, x) += filter.at<float>(i, k) * img.at<float>(y - i + filter.rows / 2, x - k + filter.cols / 2);
                }
            }
        }
    });
    return result;
}

std::vector<float> getNeighbours33(cv::Mat img, int y, int x)
{
    std::vector<float> neighbours;
    //Mitte
    neighbours.push_back(img.at<float>(y, x));
    if (y >= 1)
    {
        //Oben
        neighbours.push_back(img.at<float>(y - 1, x));
        if (x >= 1)
        {
            //Oben links
            neighbours.push_back(img.at<float>(y - 1, x - 1));
        }
        if (x < img.cols - 1)
        {
            //Unten links
            neighbours.push_back(img.at<float>(y - 1, x + 1));
        }
    }
    if (y < img.rows - 1)
    {
        //Unten
        neighbours.push_back(img.at<float>(y + 1, x));
        if (x >= 1)
        {
            //Unten links
            neighbours.push_back(img.at<float>(y + 1, x - 1));
        }
        if (x < img.cols - 1)
        {
            //Unten rechts
            neighbours.push_back(img.at<float>(y + 1, x + 1));
        }
    }
    if (x >= 1)
    {
        //Links
        neighbours.push_back(img.at<float>(y, x - 1));
    }
    if (x < img.cols - 1)
    {
        //Rechts
        neighbours.push_back(img.at<float>(y, x + 1));
    }
    return neighbours;
}

cv::Mat ueb02::filterMedian(cv::Mat img)
{
    cv::Mat result = cv::Mat::zeros(img.rows, img.cols, CV_32F);
    util::for_each_pixel(result, [img](cv::Mat &result, int y, int x) -> void {
        std::vector<float> neighbours = getNeighbours33(img, y, x);
        std::sort(neighbours.begin(), neighbours.end());
        result.at<float>(y, x) = neighbours.at((neighbours.size() - 1) / 2);
    });
    return result;
}

cv::Mat ueb02::filterErode(cv::Mat img)
{
    cv::Mat result = cv::Mat::zeros(img.rows, img.cols, CV_32F);
    util::for_each_pixel(result, [img](cv::Mat &result, int y, int x) -> void {
        std::vector<float> neighbours = getNeighbours33(img, y, x);
        result.at<float>(y, x) = *std::min_element(neighbours.begin(), neighbours.end());
    });
    return result;
}

cv::Mat ueb02::filterDilate(cv::Mat img)
{
    cv::Mat result = cv::Mat::zeros(img.rows, img.cols, CV_32F);
    util::for_each_pixel(result, [img](cv::Mat &result, int y, int x) -> void {
        std::vector<float> neighbours = getNeighbours33(img, y, x);
        result.at<float>(y, x) = *std::max_element(neighbours.begin(), neighbours.end());
    });
    return result;
}

ueb02::discriminatorEvaluation ueb02::discriminator(cv::Mat img)
{
    ueb02::discriminatorEvaluation result = ueb02::EVAL_NONE;
    cv::Mat sobelImg = ueb02::fold(img, filter::sobelXFilter, argp::EDGE_REPLICATE, 0.0f);
    cv::Mat histo(AMOUNT_BINS_DICRIMINATOR, 1, CV_32S);
    ueb01::filterClamp(sobelImg);
    ueb01::histogram(sobelImg, histo, AMOUNT_BINS_DICRIMINATOR);
    if (histo.at<float>(AMOUNT_BINS_DICRIMINATOR - 1, 0) > THRESHOLD_DISCRIMINATOR)
    {
        result = ueb02::EVAL_FOREST;
    }
    else
    {
        result = ueb02::EVAL_STAIRS;
    }

    return result;
}