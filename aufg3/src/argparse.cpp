#include "argparse.hpp"

#include <stdexcept>
#include <iostream>
#include "filter.hpp"

argp::Operations argp::parse_arguments(int argc, char *argv[])
{
	Operations ops;

	std::vector<std::string> arguments(argv + 1, argv + argc);
	unsigned int i = 0;

	auto consume_argument_string = [&arguments, &i]() {
		if (arguments.size() > i + 1)
		{
			return arguments[++i];
		}
		else
		{
			throw std::runtime_error("Nicht genuegend Argumente.");
		}
	};

	auto consume_argument_float = [&arguments, &i]() {
		if (arguments.size() > i + 1)
		{
			return std::stof(arguments[++i]);
		}
		else
		{
			throw std::runtime_error("Nicht genuegend Argumente.");
		}
	};

	auto consume_argument_int = [&arguments, &i]() {
		if (arguments.size() > i + 1)
		{
			return std::stoi(arguments[++i]);
		}
		else
		{
			throw std::runtime_error("Nicht genuegend Argumente.");
		}
	};

	for (; i < arguments.size(); i++)
	{
		std::string &action = arguments[i];
		if (action == "--input")
		{
			Operation op;
			op.action = argp::ACTION_INPUT;
			op.filename = consume_argument_string();
			ops.push_back(op);
		}
		else if (action == "--output")
		{
			Operation op;
			op.action = argp::ACTION_OUTPUT;
			op.filename = consume_argument_string();
			ops.push_back(op);
		}
		else if (action == "--identity")
		{
			Operation op;
			op.action = argp::ACTION_IDENTITY_FILTER;
			ops.push_back(op);
		}
		else if (action == "--show")
		{
			Operation op;
			op.action = argp::ACTION_SHOW;
			ops.push_back(op);
		}
		else if (action == "--clamp")
		{
			Operation op;
			op.action = argp::ACTION_CLAMP;
			ops.push_back(op);
		}
		else if (action == "--brightness")
		{
			Operation op;
			op.action = argp::ACTION_BRIGTHNEss;
			op.a[0] = consume_argument_float();
			ops.push_back(op);
		}
		else if (action == "--factor")
		{
			Operation op;
			op.action = argp::ACTION_FACTOR;
			op.a[0] = consume_argument_float();
			ops.push_back(op);
		}
		else if (action == "--linear")
		{
			Operation op;
			op.action = argp::ACTION_LINEAR;
			op.a[0] = consume_argument_float();
			op.a[1] = consume_argument_float();
			ops.push_back(op);
		}
		else if (action == "--contrast")
		{
			Operation op;
			op.action = argp::ACTION_CONTRAST;
			op.a[0] = consume_argument_float();
			ops.push_back(op);
		}
		else if (action == "--gamma")
		{
			Operation op;
			op.action = argp::ACTION_GAMMA;
			op.a[0] = consume_argument_float();
			ops.push_back(op);
		}
		else if (action == "--add")
		{
			Operation op;
			op.action = argp::ACTION_ADD;
			ops.push_back(op);
		}
		else if (action == "--sub")
		{
			Operation op;
			op.action = argp::ACTION_SUB;
			ops.push_back(op);
		}
		else if (action == "--adiff")
		{
			Operation op;
			op.action = argp::ACTION_ADIFF;
			ops.push_back(op);
		}
		else if (action == "--normalize")
		{
			Operation op;
			op.action = argp::ACTION_NORMALIZE;
			ops.push_back(op);
		}
		else if (action == "--histogram")
		{
			Operation op;
			op.action = argp::ACTION_HISTOGRAM;
			op.bin = consume_argument_int();
			ops.push_back(op);
		}
		else if (action == "--sum-histogram")
		{
			Operation op;
			op.action = argp::ACTION_SUM_HISTOGRAM;
			op.bin = consume_argument_int();
			ops.push_back(op);
		}
		else if (action == "--histogram-check")
		{
			Operation op;
			op.action = argp::ACTION_HISTOGRAM_CHECK;
			ops.push_back(op);
		}
		else if (action == "--values")
		{
			Operation op;
			op.action = argp::ACTION_VALUES;
			op.size[0] = consume_argument_int();
			op.size[1] = consume_argument_int();
			op.filter = cv::Mat::zeros(op.size[1], op.size[0], CV_32F);
			int i = 0;
			int k = 0;
			for (i = 0; i < op.size[1]; i++)
			{
				for (k = 0; k < op.size[0]; k++)
				{
					op.filter.at<float>(i, k) = consume_argument_float();
				}
				
			}
			ops.push_back(op);
		}
		else if (action == "--edge")
		{
			Operation op;
			op.action = argp::ACTION_EDGE;
			std::string temp = consume_argument_string();
			if (temp == "null")
			{
				op.edgeOpt = EDGE_NULL;
				op.edgeValue = 0.0f;
			}
			else if (temp == "replicate")
			{
				op.edgeOpt = EDGE_REPLICATE;
				op.edgeValue = -1.0f;
			}
			else if (temp == "reflect11")
			{
				op.edgeOpt = EDGE_REFLECT_11;
				op.edgeValue = -1.0f;
			}
			else if (temp == "reflect101")
			{
				op.edgeOpt = EDGE_REFLECT_101;
				op.edgeValue = -1.0f;
			}
			else
			{
				op.edgeOpt = EDGE_VALUE;
				op.edgeValue = std::stof(temp);
			}
			ops.push_back(op);
		}
		else if (action == "--fold")
		{
			Operation op;
			op.action = argp::ACTION_FOLD;
			ops.push_back(op);
		}
		else if (action == "--box")
		{
			Operation op;
			op.action = argp::ACTION_BOX;
			op.size[0] = 3;
			op.size[1] = op.size[0];
			op.filter = filter::boxFilter;
			ops.push_back(op);
		}
		else if (action == "--gaussian")
		{
			Operation op;
			op.action = argp::ACTION_BOX;
			op.size[0] = 5;
			op.size[1] = op.size[0];
			op.filter = filter::gaussianFilter;

			ops.push_back(op);
		}
		else if (action == "--sobel-x")
		{
			Operation op;
			op.action = argp::ACTION_BOX;
			op.size[0] = 3;
			op.size[1] = op.size[0];
			op.filter = filter::sobelXFilter;

			ops.push_back(op);
		}
		else if (action == "--sobel-y")
		{
			Operation op;
			op.action = argp::ACTION_BOX;
			op.size[0] = 3;
			op.size[1] = op.size[0];
			op.filter = filter::sobelYFilter;

			ops.push_back(op);
		}
		else if (action == "--laplace")
		{
			Operation op;
			op.action = argp::ACTION_BOX;
			op.size[0] = 3;
			op.size[1] = op.size[0];
			op.filter = filter::laplaceFilter;

			ops.push_back(op);
		}
		else if (action == "--median")
		{
			Operation op;
			op.action = argp::ACTION_MEDIAN;
			ops.push_back(op);
		}
		else if (action == "--erode")
		{
			Operation op;
			op.action = argp::ACTION_ERODE;
			ops.push_back(op);
		}
		else if (action == "--dilate")
		{
			Operation op;
			op.action = argp::ACTION_DILATE;
			ops.push_back(op);
		}
		else if (action == "--discriminator")
		{
			Operation op;
			op.action = argp::ACTION_DISCRIMINATOR;
			ops.push_back(op);
		}	
		/*------------------------------------------------------------*/
		/*                 Ab hier Uebung 03                          */
		/*------------------------------------------------------------*/
		
		
		else if (action == "--rotate")
		{	
			Operation op;
			op.action = argp::ACTION_ROTATE;
			//der Wert ist die Rotation in Grad
			op.transformValues.push_back(consume_argument_float());
			ops.push_back(op);
		}
			else if (action == "--scale")
		{	
			Operation op;
			op.action = argp::ACTION_SCALE;
			//Skalierung in x- und in y-Richtung
			op.transformValues.push_back(consume_argument_float());
			op.transformValues.push_back(consume_argument_float());
			ops.push_back(op);
		}
			else if (action == "--shear")
		{	
			Operation op;
			op.action = argp::ACTION_SHEAR;
			//der Wert ist der Scherfaktor
			op.transformValues.push_back(consume_argument_float());
			ops.push_back(op);
		}
		else if (action == "--transform")
		{
			
			Operation op;
			op.action = argp::ACTION_TRANSFORM;
			int m = 0;
			//4, da 2x2 Matrix fuer Transformationen, Schleife lohnt eigentlich nicht wirklich
			for(; m < 4 ;m++)
			{
				op.transformValues.push_back(consume_argument_float());
			}
			
			
			ops.push_back(op);
		}
		//erwartet 8 Werte: 2 fuer die Zielbildgroesse und 3 Koordinaten
		else if (action == "--upright")
		{
			int i = 0;
			Operation op;
			op.action = argp::ACTION_UPRIGHT;
			for(; i < 8;i++){
				int val = consume_argument_int();
				op.uprightValues.push_back(val);
			}
			ops.push_back(op);
		}
		else if (action == "--interpolation")
		{
			Operation op;
			op.action = argp::ACTION_INTERPOLATION;
			std::string temp = consume_argument_string();
			if (temp == "nearest")
			{
				op.interpolOpt = INTERPOLATION_NEAREST;
			} else if (temp == "bilinear")
			{
				op.interpolOpt = INTERPOLATION_BILINEAR;
			} else 
			{//unbekannte Interpolationsmethode
				throw std::runtime_error("Unbekannter Parameter.");	
			}
			ops.push_back(op);
		}
		else
		{
			throw std::runtime_error("Unbekannter Parameter.");
		}
	}

	return ops;
}
