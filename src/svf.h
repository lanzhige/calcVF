#pragma once
// number of rings
#define RING_SIZE 32
#define EDGE_LENGTH 512
#define HALF_LENGTH 256
#define OFFSET 0.5
#define RING_RADIUS 8.0

#define _USE_MATH_DEFINES

#include<math.h>
#include<iostream>
#include<vector>
#include<utility>
#include<string>

using std::vector;
using std::pair;
using std::string;

vector<int> calculate_T() {
	vector<int> res(RING_SIZE, 0);
	for (int i = 0; i < EDGE_LENGTH; i++) {
		for (int j = 0; j < EDGE_LENGTH; j++) {
			double d = sqrt(
				(i + OFFSET - HALF_LENGTH)*(i + OFFSET - HALF_LENGTH)
				+ (j + OFFSET - HALF_LENGTH)*(j + OFFSET - HALF_LENGTH)
			);
			double ringIndex = floor(d / RING_RADIUS);
			if (ringIndex < RING_SIZE) res[ringIndex]++;
		}
	}
	return res;
}

vector<double> calculate_SVF_max(const vector<int> &t) {
	vector<double> res(RING_SIZE, 0);
	double n2 = 2 * RING_SIZE;
	for (int i = 0; i < RING_SIZE; i++) {
		res[i] = sin(M_PI*(2 * i + 1) / n2) / t[i];
	}
	return res;
}

/*vector<int> calculate_P(unsigned char *data) {
	vector<int> res(RING_SIZE, 0);

	for (int i = 0; i < EDGE_LENGTH; i++) {
		for (int j = 0; j < EDGE_LENGTH; j++) {
			double d = sqrt(
				(i + OFFSET - HALF_LENGTH)*(i + OFFSET - HALF_LENGTH)
				+ (j + OFFSET - HALF_LENGTH)*(j + OFFSET - HALF_LENGTH)
			);
			double ringIndex = floor(d / RING_RADIUS);
			if (ringIndex < RING_SIZE) {
				int pixel = (j*EDGE_LENGTH + i);
				if (((int)data[pixel])==1) {
					res[(int)ringIndex]++;
				}
			}
		}
	}
	return res;
}

vector<int> calculate_P(unsigned char *data, int k) {
	vector<int> res(RING_SIZE, 0);

	for (int i = 0; i < EDGE_LENGTH; i++) {
		for (int j = 0; j < EDGE_LENGTH; j++) {
			double d = sqrt(
				(i + OFFSET - HALF_LENGTH)*(i + OFFSET - HALF_LENGTH)
				+ (j + OFFSET - HALF_LENGTH)*(j + OFFSET - HALF_LENGTH)
			);
			double ringIndex = floor(d / RING_RADIUS);
			if (ringIndex < RING_SIZE) {
				int pixel = (j*EDGE_LENGTH + i);
				if (((int)data[pixel]) == k) {
					res[(int)ringIndex]++;
				}
			}
		}
	}
	return res;
}*/

vector<vector<int>> calculate_P(unsigned char *data) {
	vector<vector<int>> res(6, vector<int>(RING_SIZE, 0));

	for (int i = 0; i < EDGE_LENGTH; i++) {
		for (int j = 0; j < EDGE_LENGTH; j++) {
			double d = sqrt(
				(i + OFFSET - HALF_LENGTH)*(i + OFFSET - HALF_LENGTH)
				+ (j + OFFSET - HALF_LENGTH)*(j + OFFSET - HALF_LENGTH)
			);
			double ringIndex = floor(d / RING_RADIUS);
			if (ringIndex < RING_SIZE) {
				int pixel = (j*EDGE_LENGTH + i);
				if (((int)data[pixel])>0&&((int)data[pixel])<7) {
					res[(int)data[pixel]-1][(int)ringIndex]++;
				}
			}
		}
	}
	return res;
}

double calculate_SVF(
	const vector<double> &svf_max, const vector<int> &p) {
	double SVF = 0;
	for (int i = 0; i<RING_SIZE; i++) {
		SVF += svf_max[i] * p[i];
	}
	SVF *= M_PI / (2 * RING_SIZE);
	return SVF;
}

vector<int> t;
vector<double> svf_max;
double t_total = 0;

int init() {
	t = calculate_T();
	for (int i = 0; i<t.size(); i++) {
		t_total += t[i];
	}
	svf_max = calculate_SVF_max(t);

	return 0;
}
/*
pair<double, double> calculate(unsigned char *data) {
	vector<int> p = calculate_P(data);
	double SVF1 = calculate_SVF(svf_max, p);
	double p_total = 0;
	for (int i = 0; i<p.size(); i++) {
		p_total += p[i];
	}
	double SVF2 = p_total / t_total;
	return { SVF1, SVF2 };
}
*/

string calculate(unsigned char *data) {
	string res = "";
	vector<vector<int>> p = calculate_P(data);
	for (int i = 0; i < 6; i++) {
		double vf = calculate_SVF(svf_max, p[i]);
		res = res + "," + std::to_string(vf);
	}
	return res;
}