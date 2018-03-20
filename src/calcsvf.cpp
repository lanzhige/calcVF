#include <iostream>

#include "fileio.h"
#include "compression.h"

int main(int argc, char** argv) {
	/*std::FILE *file_in, *file_out;
	fopen_s(&file_in, "test.bin", "rb");
	fopen_s(&file_out, "output.bin", "w+");
	unsigned char *data = (unsigned char *)malloc(512 * 512 * sizeof(unsigned char));
	decompress("./test.bin", data, 512*512);
	inf(file_in, file_out);

	char *tempBuffer = new char[262144];
	ifstream myFile("./test.bin", std::ios::in | std::ios::binary);
	if (!myFile.read(tempBuffer, 262144)) {
		std::cerr << "Reading file failed: " << std::endl;
		std::cerr << "read file status: " << myFile.gcount();
	}
	for (int i = 0; i < 10000; i++)
		std::cout << " " << (int)tempBuffer[i];
	myFile.close();
	*/
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
		std::ofstream myfile;
		myfile.open("output.csv");
		myfile << "lat,lng,tileX,tileY,svf1,svf2";
		myfile << output_string << "\n";
		myfile.close();
	}
	
	
	return 0;
}