/** 
  * 
  *	@author Leonhard Brandes, Mario da Graca
*/

#include "argparse.hpp"
#include "util.hpp"
#include "ueb00.hpp"
#include "ueb01.hpp"
#include "ueb02.hpp"
#include "ueb03.hpp"
#include "ueb04.hpp"

#include <functional>
#include <opencv2/opencv.hpp>

#define DEFAULT_FRAMERATE (25.0f)
#define SIZE_TEMPLATE_VECTOR (16)
#define SIZE_MATCHLOC_VECTOR (10)

namespace bba
{
    typedef std::vector<cv::Mat> imagestack;
    typedef std::function<bool(imagestack &)> stack_operation;
} // namespace bba

bool isAffineTransOp(argp::Operation op)
{
    return (op.action == argp::ACTION_ROTATE) || (op.action == argp::ACTION_SCALE) || (op.action == argp::ACTION_SHEAR) || (op.action == argp::ACTION_TRANSFORM);
}

void callBackFunc(int event, int x, int y, int flags, void *param)
{
    (void)flags;
    if (event == cv::EVENT_LBUTTONDOWN)
    {
        {
            cv::Point2i &result = *((cv::Point2i *)param);
            result.x = x;
            result.y = y;
        }
    }
}

/**
* Fuehrt die affinen Transformationen aus
* 
* @param[in] cv::Mat, Bild auf dem die Transformation ausgefuehrt wird 
* @param[in] cv::Matx33f, Transformationsmatrix
* @param[in] argp::EDGE edgeOpt, Randbehandlungsmethode
* @param[in] float, Wert fuer die Randbehandlung
* @param[in] argp::INTERPOLATION interpolationOpt, Interpolationsmethode
*
* @return cv::Mat, transformiertes Bild
*/
cv::Mat executeTransform(cv::Mat img, cv::Matx33f transMat, argp::EDGE edgeOpt, float edgeValue, argp::INTERPOLATION interpolationOpt)
{
    cv::Mat imgCpy;
    img.copyTo(imgCpy);
    std::vector<int> size = ueb03::calcNewImageSize(imgCpy, transMat);
    cv::Mat result = ueb03::transform(transMat, size, imgCpy, interpolationOpt, edgeOpt, edgeValue);
    return result;
};

/**
* Fuehrt das Tracking auf dem Video aus
* 
* @param[in] std::vector<int> i_values, enthaelt Koordinaten und Dimensionen des Templates
* @param[in] std::vector<float> f_values, RGB-Farbwerte des eingeblendeten Textes
* @param[in] std::string trackingStr, Text, der eingeblendet wird
* @param[in] std::vector<cv::Mat> &templVec, Matching-Templates der letzten Frames -> iterative Anpassung
* @param[in] std::vector<cv::Point> &matchLocVec, Matching-Locationns der letzten Frames -> zeitliche Anpassung
* @param[in] cv::Mat img, Bild auf dem das Tracking ausgeführt wird
* @param[in] cv::Mat &templ, Template
* @param[in] bool &createdTempl, Erstausfuehrung der Methode
*
* @return cv::Mat, transformiertes Bild
*/
cv::Mat executeTrack(std::vector<int> i_values, std::vector<float> f_values, std::string trackingStr, std::vector<cv::Mat> &templVec, std::vector<cv::Point> &matchLocVec, cv::Mat img, cv::Mat &templ, bool &createdTempl)
{
    cv::Mat imgCpy;
    img.copyTo(imgCpy);
    if (!createdTempl)
    {
        templ = ueb04::extractTemplate(imgCpy, i_values);
        for (int i = 0; i < SIZE_TEMPLATE_VECTOR; i++)
        {
            templVec.push_back(templ);
        }
        for (int i = 0; i < SIZE_MATCHLOC_VECTOR; i++)
        {
            matchLocVec.push_back(cv::Point(i_values.at(0), i_values.at(1)));
        }
        createdTempl = true;
    }
    ueb04::track(imgCpy, trackingStr, templVec, matchLocVec, f_values);
    return imgCpy;
};

std::vector<bba::stack_operation> bind_operations(const argp::Operations &operations)
{
    std::vector<bba::stack_operation> stack_operations;
    argp::INTERPOLATION interpolationOpt = argp::INTERPOLATION_NEAREST;
    argp::EDGE edgeOpt = argp::EDGE_NULL;
    float edgeValue = 0.0f;

    //Setzt die Framerate des Videos beim schreiben
    float framerate = DEFAULT_FRAMERATE;
    //Transformationsmatrix
    cv::Matx33f transMat(1, 0, 0, 0, 1, 0, 0, 0, 1);
    //Template für das Template-Matching
    cv::Mat templ;
    //Erstellung des Templates nur ein Mal für das gesamte Video
    bool createdTempl = false;
    //Rueckgabe des Mouse-Callbacks -> Track-GUI
    cv::Point2i trackCoords;

    unsigned int i = 0;

    for (const argp::Operation &operation : operations)
    {
        switch (operation.action)
        {
        case argp::ACTION_INPUT:
        {
            const std::string &filename = operation.filename;
            const cv::Mat img = cv::imread(filename);
            stack_operations.push_back(
                [img](bba::imagestack &stack) {
                    stack.push_back(img);
                    return true;
                });
        }
        break;
        case argp::ACTION_VIDEOIN:
        {
            const std::string &filename = operation.filename;
            cv::VideoCapture vc(filename);
            stack_operations.push_back(
                [vc](bba::imagestack &stack) mutable {
                    cv::Mat img;
                    vc >> img;
                    stack.push_back(img);
                    return !img.empty();
                });
        }
        break;
        case argp::ACTION_OUTPUT:
        {
            const std::string &filename = operation.filename;
            stack_operations.push_back(
                [filename](bba::imagestack &stack) {
                    const cv::Mat img = stack.back();
                    stack.pop_back();
                    if (filename == "-")
                    {
                        util::writeImgStdOut(img, ".ppm");
                    }
                    else
                    {

                        cv::imwrite(filename, img);
                    }
                    return false;
                });
        }
        break;
        case argp::ACTION_VIDEOOUT:
        {
            const std::string &filename = operation.filename;
            cv::VideoWriter vw;
            stack_operations.push_back(
                [vw, filename, framerate](bba::imagestack &stack) mutable {
                    const cv::Mat img = stack.back();
                    stack.pop_back();
                    if (!vw.isOpened())
                    {
                        auto fourcc = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');
                        vw.open(filename, fourcc, framerate, img.size());
                    }
                    vw << img;
                    return true;
                });
        }
        break;
        case argp::ACTION_PRINT:
        {
            stack_operations.push_back(
                [](bba::imagestack &stack) {
                    cv::Mat gray;
                    cv::cvtColor(stack.back(), gray, cv::COLOR_BGR2GRAY);
                    std::cerr << gray << std::endl;
                    return true;
                });
        }
        break;
        case argp::ACTION_TRACK:
        {
            std::vector<int> i_values = operation.i_filter_parameter;
            std::vector<float> f_values = operation.f_filter_parameter;
            std::string trackingStr = operation.trackingStr;
            std::vector<cv::Mat> templVec;
            std::vector<cv::Point> matchLocVec;

            stack_operations.push_back(
                [i_values, f_values, trackingStr, templVec, matchLocVec, templ, createdTempl](bba::imagestack &stack) mutable {
                    cv::Mat img = stack.back();
                    stack.pop_back();
                    stack.push_back(executeTrack(i_values, f_values, trackingStr, templVec, matchLocVec, img, templ, createdTempl));
                    return true;
                });
        }
        break;
        case argp::ACTION_TRACK_GUI:
        {
            std::vector<int> i_values = operation.i_filter_parameter;
            std::vector<float> f_values = operation.f_filter_parameter;
            std::string trackingStr = operation.trackingStr;
            std::vector<cv::Mat> templVec;
            std::vector<cv::Point> matchLocVec;

            stack_operations.push_back(
                [i_values, f_values, trackingStr, templVec, matchLocVec, trackCoords, templ, createdTempl](bba::imagestack &stack) mutable {
                    cv::Mat img = stack.back();
                    stack.pop_back();
                    if (!createdTempl)
                    {
                        cv::namedWindow("Choose area to track", 1);

                        cv::setMouseCallback("Choose area to track", callBackFunc, &trackCoords);

                        cv::imshow("Choose area to track", img);

                        cv::waitKey();

                        cv::destroyWindow("Choose area to track");

                        int size = i_values.back();
                        i_values.pop_back();
                        i_values.push_back(trackCoords.x);
                        i_values.push_back(trackCoords.y);
                        i_values.push_back(size);
                    }
                    stack.push_back(executeTrack(i_values, f_values, trackingStr, templVec, matchLocVec, img, templ, createdTempl));
                    return true;
                });
        }
        break;
        case argp::ACTION_FRAMERATE:
        {
            framerate = operation.f_filter_parameter.at(0);
        }
        break;
        case argp::ACTION_IDENTITY_FILTER:
        {
            stack_operations.push_back(
                [](bba::imagestack &stack) {
                    cv::Mat img = stack.back();
                    stack.pop_back();
                    ueb00::identity(img);
                    stack.push_back(img);
                    return false;
                });
        }
        break;

        case argp::ACTION_SHOW:
        {
            stack_operations.push_back(
                [](bba::imagestack &stack) {
                    cv::Mat img = stack.back();
                    util::showImage(img);
                    return false;
                });
        }
        break;

        case argp::ACTION_BRIGTHNESS:
        {
            float value = operation.f_filter_parameter.at(0);
            stack_operations.push_back(
                [value](bba::imagestack &stack) mutable {
                    cv::Mat img = stack.back();
                    stack.pop_back();
                    stack.push_back(ueb01::filterBrightness(img, value));
                    return true;
                });
        }
        break;

        case argp::ACTION_FACTOR:
        {
            float value = operation.f_filter_parameter.at(0);
            stack_operations.push_back(
                [value](bba::imagestack &stack) {
                    cv::Mat img = stack.back();
                    stack.pop_back();
                    stack.push_back(ueb01::filterFactor(img, value));
                    return true;
                });
        }
        break;

        case argp::ACTION_LINEAR:
        {
            std::vector<float> values = operation.f_filter_parameter;
            stack_operations.push_back(
                [values](bba::imagestack &stack) {
                    cv::Mat img = stack.back();
                    stack.pop_back();
                    stack.push_back(ueb01::filterLinear(img, values.at(0), values.at(1)));
                    return true;
                });
        }
        break;

        case argp::ACTION_BOX:
        case argp::ACTION_GAUSSIAN:
        case argp::ACTION_SOBEL_X:
        case argp::ACTION_SOBEL_Y:
        case argp::ACTION_LAPLACE:
        case argp::ACTION_VALUES:
        {
            cv::Mat filter = operation.filter;
            stack_operations.push_back(
                [filter](bba::imagestack &stack) {
                    cv::Mat img = ueb02::values(filter);
                    stack.push_back(img);
                    return true;
                });
        }
        break;

        case argp::ACTION_EDGE:
        {
            edgeOpt = operation.edgeOpt;
            edgeValue = operation.f_filter_parameter.at(0);
        }
        break;

        case argp::ACTION_FOLD:
        {
            stack_operations.push_back(
                [edgeOpt, edgeValue](bba::imagestack &stack) {
                    cv::Mat img1 = stack.back();
                    stack.pop_back();
                    cv::Mat img2 = stack.back();
                    stack.pop_back();
                    cv::Mat result = ueb02::fold(img2, img1, edgeOpt, edgeValue);
                    stack.push_back(result);
                    return true;
                });
        }
        break;

        case argp::ACTION_ROTATE:
        {
            cv::Matx33f rotMat = ueb03::calcRotMatrix(operation.f_filter_parameter);
            transMat = rotMat * transMat;

            if ((i == operations.size() - 1) || !isAffineTransOp(operations.at(i + 1)))
            {
                stack_operations.push_back(
                    [transMat, edgeValue, edgeOpt, interpolationOpt](bba::imagestack &stack) mutable {
                        cv::Mat img = stack.back();
                        stack.pop_back();
                        stack.push_back(executeTransform(img, transMat, edgeOpt, edgeValue, interpolationOpt));
                        return true;
                    });
            }
        }
        break;

        case argp::ACTION_SCALE:
        {
            cv::Matx33f scaleMat = ueb03::calcScaleMatrix(operation.f_filter_parameter);
            transMat = scaleMat * transMat;

            if ((i == operations.size() - 1) || !isAffineTransOp(operations.at(i + 1)))
            {
                stack_operations.push_back(
                    [transMat, interpolationOpt, edgeOpt, edgeValue](bba::imagestack &stack) {
                        cv::Mat img = stack.back();
                        stack.pop_back();
                        stack.push_back(executeTransform(img, transMat, edgeOpt, edgeValue, interpolationOpt));
                        return true;
                    });
            }
        }
        break;

        case argp::ACTION_SHEAR:
        {
            cv::Matx33f shearMat = ueb03::calcShearMatrix(operation.f_filter_parameter);
            transMat = shearMat * transMat;

            if ((i == operations.size() - 1) || !isAffineTransOp(operations.at(i + 1)))
            {
                stack_operations.push_back(
                    [transMat, interpolationOpt, edgeOpt, edgeValue](bba::imagestack &stack) {
                        cv::Mat img = stack.back();
                        stack.pop_back();
                        stack.push_back(executeTransform(img, transMat, edgeOpt, edgeValue, interpolationOpt));
                        return true;
                    });
            }
        }
        break;

        case argp::ACTION_TRANSFORM:
        {
            cv::Matx33f transformMat = ueb03::calcTransformMatrix(operation.f_filter_parameter);
            transMat = transformMat * transMat;

            if ((i == operations.size() - 1) || !isAffineTransOp(operations.at(i + 1)))
            {
                stack_operations.push_back(
                    [transMat, interpolationOpt, edgeOpt, edgeValue](bba::imagestack &stack) {
                        cv::Mat img = stack.back();
                        stack.pop_back();
                        stack.push_back(executeTransform(img, transMat, edgeOpt, edgeValue, interpolationOpt));
                        return true;
                    });
            }
        }
        break;

        case argp::ACTION_UPRIGHT:
        {
            std::vector<int> uprightValues = operation.i_filter_parameter;
            argp::EDGE edgeOpt = operation.edgeOpt;
            float edgeValue = operation.f_filter_parameter.at(0);
            argp::INTERPOLATION interpolOpt = operation.interpolOpt;
            stack_operations.push_back(
                [uprightValues, edgeOpt, edgeValue, interpolOpt](bba::imagestack &stack) {
                    cv::Mat img = stack.back();
                    stack.pop_back();
                    cv::Matx33f transformMat = ueb03::calcUprightMatrix(uprightValues, img);
                    std::vector<int> size{uprightValues.at(0), uprightValues.at(1)};
                    cv::Mat result = ueb03::transform(transformMat, size, img, interpolOpt, edgeOpt, edgeValue);
                    stack.push_back(result);
                    return false;
                });
        }
        break;

        case argp::ACTION_INTERPOLATION:
        {
            interpolationOpt = operation.interpolOpt;
        }
        break;

        default:
            break;
        }

        i++;
    }
    return stack_operations;
}

int main(int argc, char **argv)
{
    argp::Operations operations = argp::parse_arguments(argc, argv);
    std::vector<bba::stack_operation> stack_operations = bind_operations(operations);
    for (bool ok = true; ok;)
    {
        bba::imagestack stack;
        for (const bba::stack_operation &so : stack_operations)
        {
            ok = so(stack);
            if (!ok)
            {
                break;
            }
        }
    }
    return 0;
}