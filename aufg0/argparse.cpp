#include "argparse.hpp"

#include <stdexcept>
#include <iostream>

argp::Operations argp::parse_arguments(int argc, char *argv[]) {
	Operations ops;

	std::vector<std::string> arguments(argv + 1, argv + argc);
	unsigned int i = 0;

	auto consume_argument = [&arguments, &i]() {
		if (arguments.size() > i + 1) {
			return arguments[++i];
		}
		else {
			throw std::runtime_error("Nicht genuegend Argumente.");
		}
	};

	for (; i < arguments.size(); i++) {
		std::string& action = arguments[i];
		if (action == "--input") {
			Operation op;
			op.action = argp::ACTION_INPUT;
			op.filename = consume_argument();
			ops.push_back(op);
		} else if (action == "--output") {
			Operation op;
			op.action = argp::ACTION_OUTPUT;
			op.filename = consume_argument();
			ops.push_back(op);
		}
		else if(action == "--identity") {
			Operation op;
			op.action = argp::ACTION_IDENTITY_FILTER;
			ops.push_back(op);
		}
		else if(action == "--show") {
			Operation op;
			op.action = argp::ACTION_SHOW;
			ops.push_back(op);
		}
		else {
			throw std::runtime_error("Unbekannter Parameter.");
		}
	}

	return ops;
}
