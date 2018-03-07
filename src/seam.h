
#include "a2.h"

using namespace std;

vector<tuple<int, int>> findSeam(FloatImage energyMap, bool horizontal=true);
float energy(FloatImage im, int x, int y);

FloatImage energyMap(FloatImage im, bool debug=false);
FloatImage drawSeam(const FloatImage im, vector<tuple<int, int>> seam);
