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
	if (argc > 1) {
		std::cout << "input directory: "<< argv[0] << std::endl;
		string output_string = "";
		recursiveLoad(argv[1], argv[1], output_string);

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