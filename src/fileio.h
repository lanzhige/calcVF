#pragma once

#include<experimental/filesystem>
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<unordered_map>
#include<ctime>
#include<utility>

#include "compression.h"
#include "svf.h"

using std::string;
namespace fs = std::experimental::filesystem;

static std::unordered_map<string, int> bin_postfix = {
	{ "_0_0_f_c.bin", 12 },
	{ "0_90_f_c.bin", 13 },
	{ "_270_f_c.bin", 14 },
	{ "90_0_f_c.bin", 13 },
	{ "80_0_f_c.bin", 14 },
	{ "70_0_f_c.bin",14 }
};


/* recursively read directories and read image or binary files
to generate fisheye file. Para basepath is the base path of input.
Inpath is the path of the current directory. Output is the output string.
*/
int recursiveLoad(const string &basepath
	, const string& inpath, string &output) {
	fs::path filepath(inpath);
	string inDir;
	fs::path base(basepath);
	inDir = base.string();

	string curDir = filepath.string();

	int res = 0;

	for (auto &dir : fs::directory_iterator(filepath)) {
		if (fs::is_directory(dir)) {
			string temp = dir.path().string();
			std::cout << "Start processing files in folder: "
				<< temp << std::endl;
			std::clock_t start;
			double duration;
			start = std::clock();
			int count = recursiveLoad(basepath, temp, output);
			duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
			std::cout << "Finished processing files in folder " << temp
				<< ". Time used: " << duration
				<< ". Files processed: " << count << "." << std::endl;
			continue;
		}

		string s = dir.path().string();
		if (s.find("_f_c.bin") != string::npos) {
			if (s.length() < 12) continue;
			string name = s.substr(
				dir.path().parent_path().string().length(),
				s.length() - bin_postfix[s.substr(s.length() - 12, 12)] - dir.path().parent_path().string().length()
			);
			string latlng = s.substr(
				filepath.parent_path().string().length(),
				dir.path().parent_path().string().length() - filepath.parent_path().string().length()
			);
			/*
			one output file called CITY.csv next to the program (not the input or output folder).
The csv file should have the format:
lat, lon, tileX, tileY, svf
			*/
			std::cout << "file name: " << name << " latlng: " << latlng << std::endl;
			unsigned char *data = (unsigned char *)malloc(512 * 512 * sizeof(unsigned char));
			std::cout << s << std::endl;
			std::cout<<decompress(s, data, 512*512)<<" err"<<std::endl;
			std::pair<double, double> p = calculate(data);
			res++;
		}
	}
	return res;
}