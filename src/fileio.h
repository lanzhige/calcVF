#pragma once

#include<experimental/filesystem>
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<unordered_map>
#include<ctime>
#include<utility>

#include<SOIL2/SOIL2.h>

#include "svf.h"

using std::string;
namespace fs = std::experimental::filesystem;
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
		if (s.find("_f.png") != string::npos) {
			if (s.length() < 12) continue;
			string name = s.substr(
				dir.path().parent_path().string().length(),
				s.length() - 6 - dir.path().parent_path().string().length()
			);
			string tile = s.substr(
				filepath.parent_path().string().length(),
				dir.path().parent_path().string().length() - filepath.parent_path().string().length()
			);
/* one output file called CITY.csv next to the program (not the input or output folder).
The csv file should have the format:
lat, lon, tileX, tileY, svf*/
			std::cout << "file name: " << name << " latlng: " << tile << std::endl;
			unsigned char *data;
			int imageWidth, imageHeight;
			data = SOIL_load_image(&s[0], &imageWidth, &imageHeight, 0, SOIL_LOAD_RGBA);
			std::cout << "load image: " << s << " image width: " << imageWidth << " image height: " << imageHeight << std::endl;
			std::pair<double, double> p = calculate(data);
			name[0] = '\n';
			name[name.find('_')] = ',';
			tile[0] = ',';
			tile[tile.find('_')] = ',';
			output = output + name + tile + "," + std::to_string(p.first) + "," + std::to_string(p.second);
			SOIL_free_image_data(data);
			res++;
		}
	}
	return res;
}