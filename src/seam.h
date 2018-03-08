
#include "a2.h"

using namespace std;

vector<tuple<int, int>> findVerticalSeam(FloatImage energyMap);
vector<int> findHorizontalSeam(FloatImage im);
float energy(FloatImage im, int x, int y);

FloatImage energyMap(FloatImage im, bool debug=false);
FloatImage drawSeam1(const FloatImage im, vector<tuple<int, int>> seam);
FloatImage drawSeam(const FloatImage &im, const vector<int> seam, bool horizontal);
FloatImage removeSeam(const FloatImage im, vector<tuple<int, int>> seam);

