
#include "a2.h"

using namespace std;

int getMinY(FloatImage energyMap, int x, int y);
int getMinX(FloatImage energyMap, int x, int y);

FloatImage energyMap(FloatImage im);
FloatImage createEnergyMap(FloatImage im);

vector<int> findVerticalSeamMap(FloatImage energyMap);
vector<int> findVerticalSeamImage(FloatImage im);
vector<int> findHorizontalSeamImage(FloatImage im);
float energy(FloatImage im, int x, int y);

FloatImage drawSeam(const FloatImage &im, const vector<int> seam, bool isHorizontal);
FloatImage removeSeam(const FloatImage im, vector<int> seam, bool isHorizontal);

FloatImage removeObject(const FloatImage &im, const vector<tuple<int, int>> object);

FloatImage enlarge(const FloatImage &im, int addWidth, int addHeight, int numSteps);
