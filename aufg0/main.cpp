/** Ein Programm zum Laden/Speichern/Ausgeben von Bildern
  * Ebenso wurde ein Identotaetsfilter implementiert
  * 
  *	@author Leonhard Brandes, Mario da Graca
*/

#include "argparse.hpp"
#include "util.hpp"
#include "ueb00.hpp"

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
			// Laedt das Bild im original Farbformat
			cv::Mat img = cv::imread(op.filename, cv::IMREAD_UNCHANGED);
			stack.push(img);
		}
		break;

		case argp::ACTION_OUTPUT:
		{
			cv::Mat img = stack.top();
			stack.pop();
			if (op.filename == "-")
			{
				util::writeImgStdOut(img, util::determineFileFormat(img));
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

		case argp::ACTION_NONE:
			break;
		}
	}

	return 0;
}