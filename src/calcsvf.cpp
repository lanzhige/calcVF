#include <iostream>

#include "fileio.h"
#include "compression.h"

int main(int argc, char** argv) {
	std::cout << argc << std::endl;
	init();
	if (argc > 2) {
		std::cout << "input directory: " << argv[1] << std::endl;;
		std::ofstream myfile;
		string outname(argv[2]);
		if (outname.find(".csv") == string::npos) {
			outname = outname + ".csv";
		}
		myfile.open(&outname[0]);
		myfile << "MapTile-x,MapTile-y,Latitude,Longitude,SVF_N,TVF_N,BVF_N,IVF_N,PVF_N,MOVF_N,SVF_Down,TVF_Down,BVF_Down,IVF_Down,PVF_Down,MOVF_Down,SVF_Up,TVF_Up,BVF_Up,IVF_Up,PVF_Up,MOVF_Up,SVF_S,TVF_S,BVF_S,IVF_S,PVF_S,MOVF_S,SVF_W,TVF_W,BVF_W,IVF_W,PVF_W,MOVF_W,SVF_E,TVF_E,BVF_E,IVF_E,PVF_E,MOVF_E";
		recursiveLoad(argv[1], argv[1], myfile);
		myfile << "\n";
		myfile.close();
		std::cout << std::endl;
	}
	else if (argc > 1) {
		std::cout << "input directory: "<< argv[1] << std::endl;
		std::ofstream myfile;
		myfile.open("output.csv");
		myfile << "MapTile-x,MapTile-y,Latitude,Longitude,SVF_N,TVF_N,BVF_N,IVF_N,PVF_N,MOVF_N,SVF_Down,TVF_Down,BVF_Down,IVF_Down,PVF_Down,MOVF_Down,SVF_Up,TVF_Up,BVF_Up,IVF_Up,PVF_Up,MOVF_Up,SVF_S,TVF_S,BVF_S,IVF_S,PVF_S,MOVF_S,SVF_W,TVF_W,BVF_W,IVF_W,PVF_W,MOVF_W,SVF_E,TVF_E,BVF_E,IVF_E,PVF_E,MOVF_E";
		recursiveLoad(argv[1], argv[1], myfile);
		myfile << "\n";
		myfile.close();
		std::cout << std::endl;
	}
	else {
		std::cout << "please enter an input path: " << std::endl;
		std::string s;
		std::cin >> s;
		std::ofstream myfile;
		myfile.open("output.csv");
		myfile << "MapTile-x,MapTile-y,Latitude,Longitude,SVF_N,TVF_N,BVF_N,IVF_N,PVF_N,MOVF_N,SVF_Down,TVF_Down,BVF_Down,IVF_Down,PVF_Down,MOVF_Down,SVF_Up,TVF_Up,BVF_Up,IVF_Up,PVF_Up,MOVF_Up,SVF_S,TVF_S,BVF_S,IVF_S,PVF_S,MOVF_S,SVF_W,TVF_W,BVF_W,IVF_W,PVF_W,MOVF_W,SVF_E,TVF_E,BVF_E,IVF_E,PVF_E,MOVF_E";
		recursiveLoad(s, s, myfile);
		myfile << "\n";
		myfile.close();
		std::cout << std::endl;
	}
	
	return 0;
}