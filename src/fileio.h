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

#include "svf.h"
#include "compression.h"

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
		if (s.find("_0_90_f_c.bin") != string::npos) {
			if (s.length() < 14) continue;
			string name = s.substr(
				dir.path().parent_path().string().length(),
				s.length() - 13 - dir.path().parent_path().string().length()
			);
			string tile = s.substr(
				filepath.parent_path().string().length(),
				dir.path().parent_path().string().length() - filepath.parent_path().string().length()
			);

			unsigned char *data = (unsigned char *)malloc(512 * 512 * sizeof(unsigned char));
			decompress(s, data, 512 * 512);
			std::pair<double, double> p = calculate(data);
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
			output = output + tile + name + "," + std::to_string(p.first);
			delete[] data;
			res++;
		}
	}
	return res;
}

void loadFile(const string &inpath, string &output) {
	fs::path filepath(inpath);
	//record the time and tasks processed
	std::clock_t start;
	double duration;
	start = std::clock();
	int count = 0;

	//don't process redundant
	unordered_map<string, bool> hmap;

	for (auto &dir : fs::recursive_directory_iterator(filepath)) {
		if (!fs::is_directory(dir)) {
			string s = dir.path().string();
			if (s.find("_0_90_f_c.bin") != string::npos) {
				if (s.length() < 14) continue;
				
				string tile_name = s.substr(
					dir.path().parent_path().parent_path().string().length(),
					s.length() - 13 - dir.path().parent_path().parent_path().string().length()
				);
				if (hmap.find(tile_name) != hmap.end()) continue;
				hmap[tile_name] = true;

				unsigned char *data = (unsigned char *)malloc(512 * 512 * sizeof(unsigned char));
				decompress(s, data, 512 * 512);
				std::pair<double, double> p = calculate(data);
				tile_name[0] = '\n';
				if (tile_name.find('_') == string::npos) {
					std::cerr << "can't identify the LatLng of: " << tile_name << std::endl;
				}
				else {
					while (tile_name.find('_') != string::npos) {
						tile_name[tile_name.find('_')] = ',';
					}
				}
				if (tile_name.find('\\') != string::npos) {
					tile_name[tile_name.find('\\')] = ',';
				}
				if (tile_name.find('/') != string::npos) {
					tile_name[tile_name.find('/')] = ',';
				}
				output = output + tile_name + "," + std::to_string(p.first);
				delete[] data;
				count++;
			}
		}
	}

	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	std::cout << "Finished processing files in folder " << inpath
		<< ". Time used: " << duration
		<< ". Files processed: " << count << "." << std::endl;
}