#include "ueb03.hpp"
#include "util.hpp"
#include "filter.hpp"
#include "ueb01.hpp"
#include "ueb02.hpp"

#define PI (3.14159265358979)

#define DEG_TO_RAD(x) ((x) * ((PI) / 180.0f))

cv::Matx33f ueb03::calcRotMatrix(std::vector<float> values)
{
    //Umrechnung Grad zu Radiant
    float a = DEG_TO_RAD(values.front());
    cv::Matx33f toReturn(std::cos(a), -std::sin(a), 0, std::sin(a), std::cos(a), 0, 0, 0, 1);
    return toReturn;
}

cv::Matx33f ueb03::calcScaleMatrix(std::vector<float> values)
{
    cv::Matx33f toReturn(values.at(0), 0, 0, 0, values.at(1), 0, 0, 0, 1);
    return toReturn;
}

cv::Matx33f ueb03::calcShearMatrix(std::vector<float> values)
{
    cv::Matx33f toReturn(1, values.front(), 0, 0, 1, 0, 0, 0, 1);
    return toReturn;
}

cv::Matx33f ueb03::calcTransformMatrix(std::vector<float> values)
{
    cv::Matx33f toReturn(values.at(0), values.at(1), 0, values.at(2), values.at(3), 0, 0, 0, 1);
    return toReturn;
}

cv::Matx33f ueb03::calcUprightMatrix(std::vector<int> values, cv::Mat img)
{
    //uebergebenen Koordinaten
    int x0 = values.at(2) - img.cols / 2;
    int y0 = values.at(3) - img.rows / 2;
    int x1 = values.at(4) - img.cols / 2;
    int y1 = values.at(5) - img.rows / 2;
    int x2 = values.at(6) - img.cols / 2;
    int y2 = values.at(7) - img.rows / 2;
    //Zielkoordinaten
    int xd0 = -values.at(0) / 2;
    int yd0 = values.at(1) / 2;
    int xd1 = -values.at(0) / 2;
    int yd1 = -values.at(1) / 2;
    int xd2 = values.at(0) / 2;
    int yd2 = -values.at(1) / 2;

    //Matrizen zwangsweise Float => Invertierung moeglich
    cv::Mat temp = (cv::Mat_<float>(6, 6, CV_32F) << x0, y0, 1, 0, 0, 0,
                    0, 0, 0, x0, y0, 1,
                    x1, y1, 1, 0, 0, 0,
                    0, 0, 0, x1, y1, 1,
                    x2, y2, 1, 0, 0, 0,
                    0, 0, 0, x2, y1, 1);
    cv::Mat invTemp = temp.inv();

    cv::Mat dest = (cv::Mat_<float>(6, 1, CV_32F) << xd0, yd0, xd1, yd1, xd2, yd2);
    cv::Mat A;
    cv::solve(temp, dest, A, cv::DECOMP_LU);
    cv::Matx33f toReturn(A.at<float>(0, 0), A.at<float>(0, 1), A.at<float>(0, 2),
                         A.at<float>(0, 3), A.at<float>(0, 4), A.at<float>(0, 5),
                         0, 0, 1);

    return toReturn;
}

std::vector<int> ueb03::calcNewImageSize(cv::Mat img, cv::Matx33f transformMat)
{
    // Fuehrt die Transformation auf die 4 Eckpunkte des Bildes aus
    // => groesste X-Differenz = neue Breite
    // => groesste Y-Differenz = neue Hoehe
    cv::Mat topLeft = (cv::Mat_<float>(3, 1, CV_32F) << 0, 0, 1);
    cv::Mat topRight = (cv::Mat_<float>(3, 1, CV_32F) << img.cols - 1, 0, 1);
    cv::Mat bottomRight = (cv::Mat_<float>(3, 1, CV_32F) << img.cols - 1, img.rows - 1, 1);
    cv::Mat bottomLeft = (cv::Mat_<float>(3, 1, CV_32F) << 0, img.rows - 1, 1);

    cv::Mat newTopLeft = cv::Mat(transformMat * topLeft);
    cv::Mat newTopRight = cv::Mat(transformMat * topRight);
    cv::Mat newBottomRight = cv::Mat(transformMat * bottomRight);
    cv::Mat newBottomLeft = cv::Mat(transformMat * bottomLeft);

    std::vector<float> allY{newTopLeft.at<float>(1, 0), newTopRight.at<float>(1, 0), newBottomRight.at<float>(1, 0), newBottomLeft.at<float>(1, 0)};
    std::vector<float> allX{newTopLeft.at<float>(0, 0), newTopRight.at<float>(0, 0), newBottomRight.at<float>(0, 0), newBottomLeft.at<float>(0, 0)};
    float minX = *std::min_element(allX.begin(), allX.end());
    float maxX = *std::max_element(allX.begin(), allX.end());
    float minY = *std::min_element(allY.begin(), allY.end());
    float maxY = *std::max_element(allY.begin(), allY.end());
    // + 1 => Nullindizierung gegensteuern
    int width = std::floor(maxX - minX + 1 + 0.5f);
    int height = std::floor(maxY - minY + 1 + 0.5f);

    std::vector<int> newSize{width, height};

    return newSize;
}

cv::Mat ueb03::transform(cv::Matx33f transMat, std::vector<int> size, cv::Mat img, argp::INTERPOLATION interpol, argp::EDGE edgeOpt, float edgeValue)
{
    //Rueckwaertstransformation
    cv::Matx33f invTransMat = transMat.inv();
    //Zielbild
    cv::Mat toReturn(size.at(1), size.at(0), CV_32F);

    util::for_each_pixel(toReturn, [invTransMat, img, edgeOpt, edgeValue, size, interpol](cv::Mat &toReturn, int y, int x) -> void {
        //Verschiebung in den Ursprung (Index != Position)
        cv::Mat temp = (cv::Mat_<float>(1, 3) << x + 0.5f - size.at(0) / 2, y + 0.5f - size.at(1) / 2, 1);
        //Rueckverschiebung
        float newY = (invTransMat * temp)[1] - 0.5f + img.rows / 2;
        float newX = (invTransMat * temp)[0] - 0.5f + img.cols / 2;

        //Rundung auf den naechsten Nachbarn
        if (interpol == argp::INTERPOLATION_NEAREST)
        {
            //sinnvolleres Runden
            newX = std::floor(newX + 0.5f);
            newY = std::floor(newY + 0.5f);

            //Randbehandlung
            if (newX < 0.0f || newX >= img.cols || newY < 0.0f || newY >= img.rows)
            {
                switch (edgeOpt)
                {
                case argp::EDGE_NULL:
                case argp::EDGE_VALUE:
                    toReturn.at<float>(y, x) = edgeValue;
                    break;
                case argp::EDGE_REPLICATE:
                    toReturn.at<float>(y, x) = img.at<float>(ueb02::clampToEdge(newY, 0, img.rows - 1), ueb02::clampToEdge(newX, 0, img.cols - 1));
                    break;
                case argp::EDGE_REFLECT_11:
                    toReturn.at<float>(y, x) = ueb02::reflect(argp::EDGE_REFLECT_11, img, cv::Mat::ones(1, 1, CV_32F), 0, 0, newY, newX);
                    break;
                case argp::EDGE_REFLECT_101:
                    toReturn.at<float>(y, x) = ueb02::reflect(argp::EDGE_REFLECT_101, img, cv::Mat::ones(1, 1, CV_32F), 0, 0, newY, newX);
                    break;
                }
            }
            else
            {
                toReturn.at<float>(y, x) = img.at<float>(newY, newX);
            }
        }
        //Bilinear Interpolieren
        else if (interpol == argp::INTERPOLATION_BILINEAR)
        {
            //Relative Positionen (Gewichtungsfaktoren)
            float deltaY = newY - (int)newY;
            float deltaX = newX - (int)newX;

            std::vector<int> neighbours{(int)newX, (int)newY, (int)newX + 1, (int)newY, (int)newX, (int)newY + 1, (int)newX + 1, (int)newY + 1};
            std::vector<float> values;

            for (unsigned int i = 0; i < neighbours.size() / 2; i++)
            {
                if (neighbours.at(2 * i) < 0.0f || neighbours.at(2 * i) >= img.cols || neighbours.at(2 * i + 1) < 0.0f || neighbours.at(2 * i + 1) >= img.rows)
                {
                    switch (edgeOpt)
                    {
                    case argp::EDGE_NULL:
                    case argp::EDGE_VALUE:
                        values.push_back(edgeValue);
                        break;
                    case argp::EDGE_REPLICATE:
                        values.push_back(img.at<float>(ueb02::clampToEdge(neighbours.at(2 * i + 1), 0, img.rows - 1), ueb02::clampToEdge(neighbours.at(2 * i), 0, img.cols - 1)));
                        break;
                    case argp::EDGE_REFLECT_11:
                        values.push_back(ueb02::reflect(argp::EDGE_REFLECT_11, img, cv::Mat::ones(1, 1, CV_32F), 0, 0, neighbours.at(2 * i + 1), neighbours.at(2 * i)));
                        break;
                    case argp::EDGE_REFLECT_101:
                        values.push_back(ueb02::reflect(argp::EDGE_REFLECT_101, img, cv::Mat::ones(1, 1, CV_32F), 0, 0, neighbours.at(2 * i + 1), neighbours.at(2 * i)));
                        break;
                    }
                }
                else
                {
                    values.push_back(img.at<float>(neighbours.at(2 * i + 1), neighbours.at(2 * i)));
                }
            }

            //Gewichtung der Intensitäten
            toReturn.at<float>(y, x) = deltaX * deltaY * values.at(3) + (1 - deltaX) * deltaY * values.at(2) + deltaX * (1 - deltaY) * values.at(1) + (1 - deltaX) * (1 - deltaY) * values.at(0);
        }
    });

    return toReturn;
}