/** Ein Programm zum Laden/Speichern/Ausgeben von Bildern
  * Ebenso wurde ein Identotaetsfilter implementiert
  * 
  *	@author Leonhard Brandes, Mario da Graca
*/

#include "argparse.hpp"
#include "util.hpp"
#include "ueb00.hpp"
#include "ueb01.hpp"

#include <stack>
#include <opencv2/opencv.hpp>

int main(int argc, char **argv)
{

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
			result = ueb01::filterSub(img1, img2);
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

		case argp::ACTION_NONE:
			break;
		}
	}

	return 0;
}
