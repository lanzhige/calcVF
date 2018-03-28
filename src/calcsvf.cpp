#include <iostream>

#include "fileio.h"
#include "compression.h"

int main(int argc, char** argv) {
	std::cout << argc << std::endl;
	init();
	if (argc > 2) {
		std::cout << "input directory: " << argv[0] << std::endl;
		string output_string = "";
		recursiveLoad(argv[1], argv[1], output_string);
		std::ofstream myfile;
		string outname(argv[2]);
		if (outname.find(".csv") == string::npos) {
			outname = outname + ".csv";
		}
		myfile.open(&outname[0]);
		myfile << "tileX,tileY,lat,lon,SVF";
		myfile << output_string << "\n";
		myfile.close();
		std::cout << std::endl;
	}
	else if (argc > 1) {
		std::cout << "input directory: "<< argv[0] << std::endl;
		string output_string = "";
		recursiveLoad(argv[1], argv[1], output_string);
		std::ofstream myfile;
		myfile.open("output.csv");
		myfile << "tileX,tileY,lat,lon,SVF";
		myfile << output_string << "\n";
		myfile.close();
		std::cout << std::endl;
	}
	else {
		std::cout << "please enter an input path: " << std::endl;
		std::string s;
		std::cin >> s;
		string output_string = "";
		recursiveLoad(s, s, output_string);
		std::ofstream myfile;
		myfile.open("output.csv");
		myfile << "tileX,tileY,lat,lon,SVF";
		myfile << output_string << "\n";
		myfile.close();
		std::cout << std::endl;
	}
	
	return 0;
}