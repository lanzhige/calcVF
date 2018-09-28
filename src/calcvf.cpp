#include <iostream>

#include "fileio.h"

int main(int argc, char** argv) {
	init();
	if (argc > 1 && argc < 5) {
		if (argv[1][0] == '-') {
			if (argv[1][1] == 'i') {
				Fisheye fe;
				if (argc == 4) {
					std::cout << "input directory: " << argv[2] << std::endl;;
					std::ofstream myfile;
					string outname(argv[3]);
					if (outname.find(".csv") == string::npos) {
						outname = outname + ".csv";
					}
					myfile.open(&outname[0]);
					myfile << "MapTile-x,MapTile-y,Latitude,Longitude,SVF_Up,TVF_Up,BVF_Up,IVF_Up,PVF_Up,MOVF_Up,SVF_Down,TVF_Down,BVF_Down,IVF_Down,PVF_Down,MOVF_Down,SVF_N,TVF_N,BVF_N,IVF_N,PVF_N,MOVF_N,SVF_S,TVF_S,BVF_S,IVF_S,PVF_S,MOVF_S,SVF_E,TVF_E,BVF_E,IVF_E,PVF_E,MOVF_E,SVF_W,TVF_W,BVF_W,IVF_W,PVF_W,MOVF_W";
					myfile.close();
					recursiveLoad(argv[2], argv[2], outname, fe);
					myfile.open(&outname[0], std::ofstream::app | std::ofstream::out);
					myfile << "\n";
					myfile.close();
					std::cout << std::endl;
				}
				else if (argc == 3) {
					std::cout << "input directory: " << argv[2] << std::endl;
					std::ofstream myfile;
					myfile.open("output.csv");
					myfile << "MapTile-x,MapTile-y,Latitude,Longitude,SVF_Up,TVF_Up,BVF_Up,IVF_Up,PVF_Up,MOVF_Up,SVF_Down,TVF_Down,BVF_Down,IVF_Down,PVF_Down,MOVF_Down,SVF_N,TVF_N,BVF_N,IVF_N,PVF_N,MOVF_N,SVF_S,TVF_S,BVF_S,IVF_S,PVF_S,MOVF_S,SVF_E,TVF_E,BVF_E,IVF_E,PVF_E,MOVF_E,SVF_W,TVF_W,BVF_W,IVF_W,PVF_W,MOVF_W";
					myfile.close();
					string outname = "output.csv";
					recursiveLoad(argv[2], argv[2], outname, fe);
					myfile.open("output.csv", std::ofstream::out|std::ofstream::app);
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
					myfile << "MapTile-x,MapTile-y,Latitude,Longitude,SVF_Up,TVF_Up,BVF_Up,IVF_Up,PVF_Up,MOVF_Up,SVF_Down,TVF_Down,BVF_Down,IVF_Down,PVF_Down,MOVF_Down,SVF_N,TVF_N,BVF_N,IVF_N,PVF_N,MOVF_N,SVF_S,TVF_S,BVF_S,IVF_S,PVF_S,MOVF_S,SVF_E,TVF_E,BVF_E,IVF_E,PVF_E,MOVF_E,SVF_W,TVF_W,BVF_W,IVF_W,PVF_W,MOVF_W";
					myfile.close();
					string outname = "output.csv";
					recursiveLoad(s, s, outname, fe);
					myfile.open("output.csv", std::ofstream::out | std::ofstream::app);
					myfile << "\n";
					myfile.close();
					std::cout << std::endl;
				}
			}
			else if (argv[1][1] == 'f') {
				if (argc == 4) {
					std::cout << "input directory: " << argv[2] << std::endl;;
					std::ofstream myfile;
					string outname(argv[3]);
					if (outname.find(".csv") == string::npos) {
						outname = outname + ".csv";
					}
					myfile.open(&outname[0]);
					myfile.close();
					myfile << "MapTile-x,MapTile-y,Latitude,Longitude,SVF_Up,TVF_Up,BVF_Up,IVF_Up,PVF_Up,MOVF_Up,SVF_Down,TVF_Down,BVF_Down,IVF_Down,PVF_Down,MOVF_Down,SVF_N,TVF_N,BVF_N,IVF_N,PVF_N,MOVF_N,SVF_S,TVF_S,BVF_S,IVF_S,PVF_S,MOVF_S,SVF_E,TVF_E,BVF_E,IVF_E,PVF_E,MOVF_E,SVF_W,TVF_W,BVF_W,IVF_W,PVF_W,MOVF_W";
					recursiveLoad(argv[2], argv[2], myfile);
					myfile.open(&outname[0], std::ofstream::app | std::ofstream::out);
					myfile << "\n";
					myfile.close();
					std::cout << std::endl;
				}
				else if (argc == 3) {
					std::cout << "input directory: " << argv[2] << std::endl;
					std::ofstream myfile;
					myfile.open("output.csv");
					myfile << "MapTile-x,MapTile-y,Latitude,Longitude,SVF_Up,TVF_Up,BVF_Up,IVF_Up,PVF_Up,MOVF_Up,SVF_Down,TVF_Down,BVF_Down,IVF_Down,PVF_Down,MOVF_Down,SVF_N,TVF_N,BVF_N,IVF_N,PVF_N,MOVF_N,SVF_S,TVF_S,BVF_S,IVF_S,PVF_S,MOVF_S,SVF_E,TVF_E,BVF_E,IVF_E,PVF_E,MOVF_E,SVF_W,TVF_W,BVF_W,IVF_W,PVF_W,MOVF_W";
					myfile.close();
					recursiveLoad(argv[2], argv[2], myfile);
					myfile.open("output.csv", std::ofstream::app | std::ofstream::out);
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
					myfile << "MapTile-x,MapTile-y,Latitude,Longitude,SVF_Up,TVF_Up,BVF_Up,IVF_Up,PVF_Up,MOVF_Up,SVF_Down,TVF_Down,BVF_Down,IVF_Down,PVF_Down,MOVF_Down,SVF_N,TVF_N,BVF_N,IVF_N,PVF_N,MOVF_N,SVF_S,TVF_S,BVF_S,IVF_S,PVF_S,MOVF_S,SVF_E,TVF_E,BVF_E,IVF_E,PVF_E,MOVF_E,SVF_W,TVF_W,BVF_W,IVF_W,PVF_W,MOVF_W";
					myfile.close();
					recursiveLoad(s, s, myfile);
					myfile.open("output.csv", std::ofstream::app | std::ofstream::out);
					myfile << "\n";
					myfile.close();
					std::cout << std::endl;
				}
			}
			else {
				std::cerr << "the first paramenter should be -f for fisheye input, -i for segmented input!" << std::endl;
				exit(1);
			}
		}
		else {
			std::cerr << "the first paramenter should be -f for fisheye input, -i for segmented input!" << std::endl;
			exit(1);
		}
	}
	else {
		std::cerr << "unresolved number of parameters. the parameters should be in range [1,3]" << std::endl;
		exit(1);
	}


	
	
	return 0;
}