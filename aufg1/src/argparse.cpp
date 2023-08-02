#include "argparse.hpp"

#include <stdexcept>
#include <iostream>

argp::Operations argp::parse_arguments(int argc, char *argv[]) {
	Operations ops;

	std::vector<std::string> arguments(argv + 1, argv + argc);
	unsigned int i = 0;

	auto consume_argument_string = [&arguments, &i]() {
		if (arguments.size() > i + 1) {
			return arguments[++i];
		}
		else {
			throw std::runtime_error("Nicht genuegend Argumente.");
		}
	};

	auto consume_argument_float = [&arguments, &i]() {
		if (arguments.size() > i + 1) {
			return std::stof(arguments[++i]);
		}
		else {
			throw std::runtime_error("Nicht genuegend Argumente.");
		}
	};

	auto consume_argument_int = [&arguments, &i]() {
		if (arguments.size() > i + 1) {
			return std::stoi(arguments[++i]);
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
			op.filename = consume_argument_string();
			ops.push_back(op);
		} else if (action == "--output") {
			Operation op;
			op.action = argp::ACTION_OUTPUT;
			op.filename = consume_argument_string();
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
		else if(action == "--clamp") {
			Operation op;
			op.action = argp::ACTION_CLAMP;
			ops.push_back(op);
		}
		else if(action == "--brightness") {
			Operation op;
			op.action = argp::ACTION_BRIGTHNEss;
			op.a[0] = consume_argument_float();
			ops.push_back(op);
		}
		else if(action == "--factor") {
			Operation op;
			op.action = argp::ACTION_FACTOR;
			op.a[0] = consume_argument_float();
			ops.push_back(op);
		}
		else if(action == "--linear") {
			Operation op;
			op.action = argp::ACTION_LINEAR;
			op.a[0] = consume_argument_float();
			op.a[1] = consume_argument_float();
			ops.push_back(op);
		}
		else if(action == "--contrast") {
			Operation op;
			op.action = argp::ACTION_CONTRAST;
			op.a[0] = consume_argument_float();
			ops.push_back(op);
		}
		else if(action == "--gamma") {
			Operation op;
			op.action = argp::ACTION_GAMMA;
			op.a[0] = consume_argument_float();
			ops.push_back(op);
		}
		else if(action == "--add") {
			Operation op;
			op.action = argp::ACTION_ADD;
			ops.push_back(op);
		}
		else if(action == "--sub") {
			Operation op;
			op.action = argp::ACTION_SUB;
			ops.push_back(op);
		}
		else if(action == "--adiff") {
			Operation op;
			op.action = argp::ACTION_ADIFF;
			ops.push_back(op);
		}
		else if(action == "--normalize") {
			Operation op;
			op.action = argp::ACTION_NORMALIZE;
			ops.push_back(op);
		}
		else if(action == "--histogram") {
			Operation op;
			op.action = argp::ACTION_HISTOGRAM;
			op.bin = consume_argument_int();
			ops.push_back(op);
		}
		else if(action == "--sum-histogram") {
			Operation op;
			op.action = argp::ACTION_SUM_HISTOGRAM;
			op.bin = consume_argument_int();
			ops.push_back(op);
		}
		else if(action == "--histogram-check") {
			Operation op;
			op.action = argp::ACTION_HISTOGRAM_CHECK;
			ops.push_back(op);
		}
		else {
			throw std::runtime_error("Unbekannter Parameter.");
		}
	}

	return ops;
}
