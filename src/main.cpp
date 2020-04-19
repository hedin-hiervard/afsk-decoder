#include <iostream>

#include "argparse.hpp"

#include "data_types.h"
#include "AfskDecoder.h"

using namespace std;
using namespace argparse;

auto main(int argc, char** argv) -> int {
	ArgumentParser program("afsk decoder");

	program.add_argument("-f", "--file")
		.help("input wav file to decode")
		.required();

	program.add_argument("--min-zero")
		.help("min width of zero rectangle in microseconds")
		.action([](const std::string& value) { return std::stod(value); })
		.default_value(600.0);

	program.add_argument("--max-zero")
		.help("max width of zero rectangle in microseconds")
		.action([](const std::string& value) { return std::stod(value); })
		.default_value(680.0);

	program.add_argument("--min-one")
		.help("min width of one rectangle in microseconds")
		.action([](const std::string& value) { return std::stod(value); })
		.default_value(300.0);

	program.add_argument("--max-one")
		.help("max width of one rectangle in microseconds")
		.action([](const std::string& value) { return std::stod(value); })
		.default_value(360.0);

	program.add_argument("--zc-resolution")
		.help("resolution of zero-crossing detection in number of samples")
		.action([](const std::string& value) { return std::stoi(value); })
		.default_value(512);

	program.add_argument("--raw")
		.help("print raw bits and bytes")
		.default_value(false)
		.implicit_value(true);

	program.add_argument("--info")
		.help("print info messages about decoding")
		.default_value(false)
		.implicit_value(true);

	program.add_argument("--errors")
		.help("print decoding errors")
		.default_value(false)
		.implicit_value(true);

	try {
	    program.parse_args(argc, argv);
	 }
	 catch (const std::runtime_error& err) {
	 	cout << "bad argument" << endl;
		cout << program.help().str() << endl;
		return 1;
	 }
	 catch (const std::invalid_argument& err) {
	 	cout << "bad argument" << endl;
		cout << program.help().str() << endl;
		return 1;
	 }

	std::string filename;

	double zeroMinWidth;
	double zeroMaxWidth;

	double oneMinWidth;
	double oneMaxWidth;

	Samples::size_type zcDetectResolution;

	bool printRaw;
	bool printErrors;
	bool printInfo;

	try {
		filename = program.get<string>("--file");
		zeroMinWidth = program.get<double>("--min-zero");
		zeroMaxWidth = program.get<double>("--max-zero");
		oneMinWidth = program.get<double>("--min-one");
		oneMaxWidth = program.get<double>("--max-one");
		zcDetectResolution = program.get<int>("--zc-resolution");
		printRaw = program.get<bool>("--raw");
		printInfo = program.get<bool>("--info");
		printErrors = program.get<bool>("--errors");
	} catch(const std::runtime_error) {
		cout << "bad argument" << endl;
		cout << program.help().str() << endl;
		return 1;
	}

	AfskDecoder decoder;

	auto coutWriter = [](std::string msg) { cout << msg; };
	decoder.decode(
		move(filename),
		{
			.zeroBitRangeInMicroseconds = { zeroMinWidth, zeroMaxWidth },
			.oneBitRangeInMicroseconds = { oneMinWidth, oneMaxWidth },
		},
		zcDetectResolution,
		printInfo ? coutWriter : nullptr,
		printErrors ? coutWriter : nullptr,
		coutWriter,
		printRaw ? coutWriter : nullptr
	);
	return 0;
}