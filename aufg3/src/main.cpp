/** Ein Programm zum Laden/Speichern/Ausgeben von Bildern
  * Ebenso wurde ein Identotaetsfilter implementiert
  * 
  *	@author Leonhard Brandes, Mario da Graca
*/

#include "argparse.hpp"
#include "util.hpp"
#include "ueb00.hpp"
#include "ueb01.hpp"
#include "ueb02.hpp"
#include "ueb03.hpp"

#include <stack>
#include <opencv2/opencv.hpp>

int main(int argc, char **argv)
{
	argp::INTERPOLATION interpolationOpt = argp::INTERPOLATION_NEAREST;
	argp::EDGE edgeOpt = argp::EDGE_NULL;
	float edgeValue = 0.0f;

	argp::Operations ops = argp::parse_arguments(argc, argv);
	std::stack<cv::Mat> stack;

	// Flag CV_IMWRITE_PXM_BINARY 1(standard) Ausgabe als Binary
	// setzen auf 0 -> nonBinary Ausgabe
	std::vector<int> outputParams;
	outputParams.push_back(CV_IMWRITE_PXM_BINARY);
	outputParams.push_back(0);

	for (const argp::Operation &op : ops)
	{
		switch (op.action)
		{
		case argp::ACTION_INPUT:
		{
			cv::Mat img = cv::imread(op.filename, CV_LOAD_IMAGE_GRAYSCALE);
			img.cv::Mat::convertTo(img, CV_32F, 1.0f / 255.0f);
			stack.push(img);
		}
		break;

		case argp::ACTION_OUTPUT:
		{
			cv::Mat img = stack.top();
			stack.pop();
			img.cv::Mat::convertTo(img, CV_8U, 255);
			if (op.filename == "-")
			{
				util::writeImgStdOut(img, ".pgm");
			}
			else
			{
				cv::imwrite(op.filename, img, outputParams);
			}
		}
		break;

		case argp::ACTION_IDENTITY_FILTER:
		{
			cv::Mat img = stack.top();
			stack.pop();
			ueb00::identity(img);
			stack.push(img);
		}
		break;

		case argp::ACTION_SHOW:
		{
			cv::Mat img = stack.top();
			util::showImage(img);
		}
		break;

		case argp::ACTION_CLAMP:
		{
			cv::Mat img = stack.top();
			stack.pop();
			ueb01::filterClamp(img);
			stack.push(img);
		}
		break;

		case argp::ACTION_BRIGTHNEss:
		{
			cv::Mat img = stack.top();
			stack.pop();
			ueb01::filterBrightness(img, op.a[0]);
			stack.push(img);
		}
		break;

		case argp::ACTION_FACTOR:
		{
			cv::Mat img = stack.top();
			stack.pop();
			ueb01::filterFactor(img, op.a[0]);
			stack.push(img);
		}
		break;

		case argp::ACTION_LINEAR:
		{
			cv::Mat img = stack.top();
			stack.pop();
			ueb01::filterLinear(img, op.a[0], op.a[1]);
			stack.push(img);
		}
		break;

		case argp::ACTION_CONTRAST:
		{
			cv::Mat img = stack.top();
			stack.pop();
			ueb01::filterContrast(img, op.a[0]);
			stack.push(img);
		}
		break;

		case argp::ACTION_GAMMA:
		{
			cv::Mat img = stack.top();
			stack.pop();
			ueb01::filterGamma(img, op.a[0]);
			stack.push(img);
		}
		break;

		case argp::ACTION_ADD:
		{
			cv::Mat img1 = stack.top();
			stack.pop();
			cv::Mat img2 = stack.top();
			stack.pop();
			cv::Mat result;
			result = ueb01::filterAdd(img1, img2);
			stack.push(result);
		}
		break;

		case argp::ACTION_SUB:
		{
			cv::Mat img1 = stack.top();
			stack.pop();
			cv::Mat img2 = stack.top();
			stack.pop();
			cv::Mat result;
			result = ueb01::filterSub(img2, img1);
			stack.push(result);
		}
		break;

		case argp::ACTION_ADIFF:
		{
			cv::Mat img1 = stack.top();
			stack.pop();
			cv::Mat img2 = stack.top();
			stack.pop();
			cv::Mat result;
			result = ueb01::filterADiff(img1, img2);
			stack.push(result);
		}
		break;

		case argp::ACTION_NORMALIZE:
		{
			cv::Mat img = stack.top();
			stack.pop();
			ueb01::filterNormalize(img);
			stack.push(img);
		}
		break;

		case argp::ACTION_HISTOGRAM:
		{
			cv::Mat img = stack.top();
			cv::Mat histo(op.bin, 1, CV_32S);
			ueb01::histogram(img, histo, op.bin);
			util::printHistogram(histo);
		}
		break;

		case argp::ACTION_SUM_HISTOGRAM:
		{
			cv::Mat img = stack.top();
			cv::Mat histo(op.bin, 1, CV_32S);
			ueb01::sumHistogram(img, histo, op.bin);
			util::printHistogram(histo);
		}
		break;

		case argp::ACTION_HISTOGRAM_CHECK:
		{
			cv::Mat img = stack.top();
			ueb01::histogramEvaluation result = ueb01::histogramCheck(img);
			util::printHistogramCheck(result);
		}
		break;

		case argp::ACTION_BOX:
		case argp::ACTION_GAUSSIAN:
		case argp::ACTION_SOBEL_X:
		case argp::ACTION_SOBEL_Y:
		case argp::ACTION_LAPLACE:
		case argp::ACTION_VALUES:
		{
			cv::Mat img = ueb02::values(op.filter);
			stack.push(img);
		}
		break;

		case argp::ACTION_EDGE:
		{
			edgeOpt = op.edgeOpt;
			edgeValue = op.edgeValue;
		}
		break;

		case argp::ACTION_FOLD:
		{
			cv::Mat filter = stack.top();
			stack.pop();
			cv::Mat img = stack.top();
			stack.pop();
			cv::Mat result = ueb02::fold(img, filter, edgeOpt, edgeValue);
			stack.push(result);
		}
		break;

		case argp::ACTION_MEDIAN:
		{
			cv::Mat img = stack.top();
			stack.pop();
			cv::Mat result = ueb02::filterMedian(img);
			stack.push(result);
		}
		break;

		case argp::ACTION_ERODE:
		{
			cv::Mat img = stack.top();
			stack.pop();
			cv::Mat result = ueb02::filterErode(img);
			stack.push(result);
		}
		break;

		case argp::ACTION_DILATE:
		{
			cv::Mat img = stack.top();
			stack.pop();
			cv::Mat result = ueb02::filterDilate(img);
			stack.push(result);
		}
		break;

		case argp::ACTION_DISCRIMINATOR:
		{
			cv::Mat img = stack.top();
			ueb02::discriminatorEvaluation result = ueb02::discriminator(img);
			util::printDiscriminator(result);
			result = result;
		}
		break;

		/*-----------------------------------------*/
		/*           Ab hier Uebung 03             */
		/*-----------------------------------------*/
		case argp::ACTION_ROTATE:
		{
			cv::Mat img = stack.top();
			stack.pop();
			cv::Matx33f transformMat = ueb03::calcRotMatrix(op.transformValues);
			std::vector<int> size = ueb03::calcNewImageSize(img, transformMat);
			cv::Mat result = ueb03::transform(transformMat, size, img, interpolationOpt, edgeOpt, edgeValue);
			stack.push(result);
		}
		break;

		case argp::ACTION_SCALE:
		{
			cv::Mat img = stack.top();
			stack.pop();
			cv::Matx33f transformMat = ueb03::calcScaleMatrix(op.transformValues);
			std::vector<int> size = ueb03::calcNewImageSize(img, transformMat);
			cv::Mat result = ueb03::transform(transformMat, size, img, interpolationOpt, edgeOpt, edgeValue);
			stack.push(result);
		}
		break;

		case argp::ACTION_SHEAR:
		{
			cv::Mat img = stack.top();
			stack.pop();
			cv::Matx33f transformMat = ueb03::calcShearMatrix(op.transformValues);
			std::vector<int> size = ueb03::calcNewImageSize(img, transformMat);
			cv::Mat result = ueb03::transform(transformMat, size, img, interpolationOpt, edgeOpt, edgeValue);
			stack.push(result);
		}
		break;

		case argp::ACTION_TRANSFORM:
		{
			cv::Mat img = stack.top();
			stack.pop();
			cv::Matx33f transformMat = ueb03::calcTransformMatrix(op.transformValues);
			std::vector<int> size = ueb03::calcNewImageSize(img, transformMat);
			cv::Mat result = ueb03::transform(transformMat, size, img, interpolationOpt, edgeOpt, edgeValue);
			stack.push(result);
		}
		break;

		case argp::ACTION_UPRIGHT:
		{
			cv::Mat img = stack.top();
			stack.pop();
			cv::Matx33f transformMat = ueb03::calcUprightMatrix(op.uprightValues, img);
			std::vector<int> size{op.uprightValues.at(0), op.uprightValues.at(1)};
			cv::Mat result = ueb03::transform(transformMat, size, img, interpolationOpt, edgeOpt, edgeValue);
			stack.push(result);
		}
		break;

		case argp::ACTION_INTERPOLATION:
		{
			interpolationOpt = op.interpolOpt;
		}

		case argp::ACTION_NONE:
			break;
		}
	}

	return 0;
}
