#pragma once

#include<experimental/filesystem>
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<unordered_map>
#include<ctime>
#include<utility>
#include<unordered_map>

#include "vf.h"
#include "compression.h"

using std::string;
namespace fs = std::experimental::filesystem;

unordered_map<string, bool> hmap;

static std::unordered_map<string, int> bin_postfix = {
	{ "_0_0_f_c.bin", 12 },
	{ "0_90_f_c.bin", 13 },
	{ "_270_f_c.bin", 14 },
	{ "90_0_f_c.bin", 13 },
	{ "80_0_f_c.bin", 14 },
	{ "70_0_f_c.bin",14 }
};

const string postfix[6] = {
	"_0_0_f_c.bin",    //north
	"_0_270_f_c.bin",  //down
	"_0_90_f_c.bin",   //up
	"_180_0_f_c.bin",  //south
	"_270_0_f_c.bin",  //west
	"_90_0_f_c.bin",   //east
};

/* recursively read directories and read image or binary files
to generate fisheye file. Para basepath is the base path of input.
Inpath is the path of the current directory. Output is the output string.
*/
int recursiveLoad(const string &basepath
	, const string& inpath, std::ofstream &myfile) {
	fs::path filepath(inpath);
	string inDir;
	fs::path base(basepath);
	inDir = base.string();

	string curDir = filepath.string();

	int res = 0;
	int total_dir = 0;
	for (auto &dir : fs::directory_iterator(filepath)) {
		if (fs::is_directory(dir)) {
			total_dir++;
		}
	}

	int dir_count = 0;

	for (auto &dir : fs::directory_iterator(filepath)) {
		if (fs::is_directory(dir)) {
			string temp = dir.path().string();
			/*std::cout << "Start processing files in folder: "
				<< temp << std::endl;*/
			std::clock_t start;
			double duration;
			start = std::clock();
			int count = recursiveLoad(basepath, temp, myfile);
			duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
			dir_count++;
			std::cout << "Finished processing files in folder " << temp << std::endl;
			std::cout << ". Time used: " << duration
				<< ". Files processed: " << count << ". " 
				<< "Folder processed: "<<dir_count<<"/"<<total_dir
				<< std::endl;
			continue;
		}

		string s = dir.path().string();
		if (s.find("_0_90_f_c.bin") != string::npos) {
			string file_name_path = s.substr(
				0, s.length() - bin_postfix[s.substr(s.length() - 12, 12)]
			);

			string name = s.substr(
				dir.path().parent_path().string().length(),
				s.length() - 13 - dir.path().parent_path().string().length()
			);

			if (hmap.find(name) != hmap.end()) continue;
			hmap[name] = true;

			string tile = s.substr(
				filepath.parent_path().string().length(),
				dir.path().parent_path().string().length() - filepath.parent_path().string().length()
			);

			unsigned char *data = (unsigned char *)malloc(512 * 512 * sizeof(unsigned char));
			name[0] = ',';
			if (name.find('_') == string::npos) {
				std::cerr << "can't identify the LatLng of: " << name << std::endl;
			}
			else {
				name[name.find('_')] = ',';
			}
			tile[0] = '\n';
			if (tile.find('_') == string::npos) {
				std::cerr << "can't identify tile of: " << tile << std::endl;
			}
			else {
				tile[tile.find('_')] = ',';
			}

			string output = tile + name;

			for (int i = 0; i < 6; i++) {
				string data_path = file_name_path + postfix[i];
				decompress(data_path, data, 512 * 512);
				string vf = calculate(data);

				output = output + vf;
			}
			myfile << output;
			delete[] data;
			res++;
		}
	}
	return res;
}