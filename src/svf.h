// number of rings
#define RING_SIZE 32
#define EDGE_LENGTH 512
#define HALF_LENGTH 256
#define OFFSET 0.5
#define RING_RADIUS 8

#define _USE_MATH_DEFINES

#include<math.h>
#include<iostream>
#include<vector>
#include<utility>

using std::vector;
using std::pair;

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
	vector<double> res(EDGE_LENGTH, 0);
	double n2 = 2 * EDGE_LENGTH;
	for (int i = 0; i < EDGE_LENGTH; i++) {
		res[i] = sin(M_PI*(2 * i + 1) / n2) / t[i];
	}
	return res;
}

vector<int> calculate_P(unsigned char *data) {
	vector<int> res(RING_SIZE, 0);
	for (int i = 0; i < EDGE_LENGTH; i++) {
		for (int j = 0; j < EDGE_LENGTH; j++) {
			double d = sqrt(
				(i + OFFSET - HALF_LENGTH)*(i + OFFSET - HALF_LENGTH)
				+ (j + OFFSET - HALF_LENGTH)*(j + OFFSET - HALF_LENGTH)
			);
			double ringIndex = floor(d / RING_RADIUS);
			if (ringIndex < RING_SIZE) {
				int pixel = (j*EDGE_LENGTH + i) * 4;
				if (data[pixel] > 128) {
					res[ringIndex]++;
				}
			}
		}
	}
	return res;
}

double calculate_SVF(
	const vector<double> &svf_max, const vector<int> &p) {
	double SVF = 0;
	for (int i = 0; i<EDGE_LENGTH; i++) {
		SVF += svf_max[i] * p[i];
	}
	SVF *= M_PI / (2 * EDGE_LENGTH);
	return SVF;
}

vector<int> t = calculate_T();
vector<double> svf_max;
double t_total = 0;

int init() {
	for (int i = 0; i<EDGE_LENGTH; i++) {
		t_total += t[i];
	}
	svf_max = calculate_SVF_max(t);

	return 0;
}

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
