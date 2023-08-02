/**
 * Ein Modul zum Verarbeiten von Kommandozeilen Parametern
 * 
 * 	@author Leonhard Brandes, Mario da Graca
*/

#pragma once

#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

namespace argp {

	enum Action {
		ACTION_INPUT,
		ACTION_OUTPUT,
		ACTION_IDENTITY_FILTER,
		ACTION_SHOW,
		ACTION_NONE,
		ACTION_CLAMP,
		ACTION_BRIGTHNEss,
		ACTION_FACTOR,
		ACTION_LINEAR,
		ACTION_CONTRAST,
		ACTION_GAMMA,
		ACTION_ADD,
		ACTION_SUB,
		ACTION_ADIFF,
		ACTION_NORMALIZE,
		ACTION_HISTOGRAM,
		ACTION_SUM_HISTOGRAM,
		ACTION_HISTOGRAM_CHECK,
		ACTION_VALUES,
		ACTION_EDGE,
		ACTION_FOLD,
		ACTION_BOX,
		ACTION_GAUSSIAN,
		ACTION_SOBEL_X,
		ACTION_SOBEL_Y,
		ACTION_LAPLACE,
		ACTION_MEDIAN,
		ACTION_ERODE,
		ACTION_DILATE,
		ACTION_DISCRIMINATOR,
		ACTION_ROTATE,
		ACTION_SCALE,
		ACTION_SHEAR,
		ACTION_TRANSFORM,
		ACTION_UPRIGHT,
		ACTION_INTERPOLATION,
	};

	enum EDGE {
		EDGE_NULL,
		EDGE_VALUE,
		EDGE_REPLICATE,
		EDGE_REFLECT_11,
		EDGE_REFLECT_101
	};

	enum INTERPOLATION {
		INTERPOLATION_NEAREST,
		INTERPOLATION_BILINEAR
	};

	struct Operation {
		Action action;
		std::string filename;
		// Faktor fuer zB linear
		float a[2];
		// Anzahl an Klassen beim Histogramm
		int bin;
		// Dimensionen einer zu erstellenden Matrix
		int size[2];
		cv::Mat filter;
		// Werte fuer die Randbehandlung
		EDGE edgeOpt;
		float edgeValue;
		//fuer upright
		std::vector<int> uprightValues;
		//fuer verschiedene Transformationen
		std::vector<float> transformValues;
		//fuer Interpolation
		INTERPOLATION interpolOpt;
	};

	typedef std::vector<Operation> Operations;

	Operations parse_arguments(int argc, char *argv[]);
}
