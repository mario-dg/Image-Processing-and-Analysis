/**
 * Ein Modul zum Verarbeiten von Kommandozeilen Parametern
 * 
 * 	@author Leonhard Brandes, Mario da Graca
*/

#pragma once

#include <string>
#include <vector>

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
		ACTION_HISTOGRAM_CHECK
	};

	struct Operation {
		Action action;
		std::string filename;
		float a[2];
		int bin;
	};

	typedef std::vector<Operation> Operations;

	Operations parse_arguments(int argc, char *argv[]);
}
