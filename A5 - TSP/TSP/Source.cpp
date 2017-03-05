/*
 * This uses only 2^31 bytes of ram
 */

#include <iostream>
#include <bitset>
#include <unordered_map>
#include <string>

constexpr char VERTICES = 25;
constexpr int pow2_27 = 134217728;

using namespace std;

struct TspParams {
	bitset<VERTICES> b;
	char verticesUsed;
	char target;

	void print() const {
		cout << b << " vertices used: " << (int)verticesUsed << " target: " << (int)target << endl;
	}

	int toPos()const {
		return static_cast<size_t>(b.to_ullong())>>1 | target << (VERTICES-1);
	}
};

vector<pair<double, double>> points;
double ** solved;


double dist(int a, int b) {
	double dx = points[a].first - points[b].first;
	double dy = points[a].second - points[b].second;
	return sqrt(dx*dx + dy*dy);
}

void put(TspParams params, double value) {
	int pos = params.toPos();
	int i = 0;
	while(pos - pow2_27 >= 0) {
		pos -= pow2_27;
		i++;
	}
	solved[i][pos] = value;

}

double get(TspParams params) {
	int pos = params.toPos();
	int i = 0;
	while (pos - pow2_27 >= 0) {
		pos -= pow2_27;
		i++;
	}
	return solved[i][pos];
}



double valueForParams(TspParams params) {
	if (get(params) != -1)
		return get(params);
	
	double minVal = DBL_MAX;
	for (char i = params.verticesUsed == 2 ? 0:1;i<VERTICES;i++) {
		if (i != params.target && params.b[i]) {
			TspParams newParams = { params.b, params.verticesUsed - 1, i };
			newParams.b[params.target] = false;
			double val = valueForParams(newParams);
			put(newParams, val);
			double totalDist = val + dist(i, params.target);
			if (val != DBL_MAX && totalDist < minVal)
				minVal = totalDist;
		}
	}

	return minVal;
}

void initBaseCase() {
	TspParams params;
	params.verticesUsed = 1;
	params.target = 0;
	for (char i = 0; i<VERTICES - 1; i++) {
		TspParams paramsCopy = params;
		paramsCopy.b[i] = true;
		if (i == 0)
			put(paramsCopy, 0);
		else
			put(paramsCopy, DBL_MAX);
	}
}

double solveTSP() {
	initBaseCase();
	bitset<VERTICES> verticesToUse(string((int)VERTICES, '1'));

	double minDist = DBL_MAX;
	for (char i=1;i<VERTICES;i++) {
		TspParams params { verticesToUse, VERTICES, i };
		double val = valueForParams(params);
		if (val != DBL_MAX) {
			double totalDist = val + dist(0, i);
			minDist = totalDist < minDist ? totalDist : minDist;
		}

	}
	
	return minDist;
	
}

int main() {
	solved = new double*[4];
	for (int i = 0; i < 4; i++)
		solved[i] = new double[pow2_27];
	for (int j=0;j<4;j++)
		for (int i = 0; i < pow2_27; i++)
			solved[j][i] = -1;



	int verts;
	cin >> verts;
	if (verts != VERTICES) {
		cout << "Expected " << VERTICES << " and not " << verts << endl;
		exit(1);
	}

	
	for (int i=0;i<verts;i++) {
		double x, y;
		cin >> x >> y;
		points.push_back({ x,y });
	}


	cout << solveTSP() << endl;;
	//TspParams tp{ bitset<VERTICES>("1111111111111111111111111"), 2 };
	//hash<TspParams> h;
	////cout << h(tp) << endl;
	//cout << bitset< 32 >(h(tp)) << endl;
	

}