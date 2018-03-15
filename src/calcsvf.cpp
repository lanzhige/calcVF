#include <iostream>

#include "fileio.h"

#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif

int main(int argc, char** argv) {
	std::cout << argc << std::endl;
	init();
	if (argc > 1) {
		std::cout << "input directory: "<< argv[0] << std::endl;
		string output_string = "";
		recursiveLoad(argv[1], argv[1], output_string);
		std::ofstream myfile;
		myfile.open("output.csv");
		myfile << "lat,lng,tileX,tileY,svf1,svf2";
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
	}
	return 0;
}