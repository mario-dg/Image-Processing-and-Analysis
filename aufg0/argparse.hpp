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
		ACTION_NONE
	};

	struct Operation {
		Action action;
		std::string filename;
	};

	typedef std::vector<Operation> Operations;

	Operations parse_arguments(int argc, char *argv[]);
}