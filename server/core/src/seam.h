
#include "a2.h"

using namespace std;

int lowestRowIndex(FloatImage energyMap);
int lowestColumnIndex(FloatImage energyMap);

vector<int> findVerticalSeamMap(FloatImage energyMap);
vector<int> findHorizontalSeamMap(FloatImage energyMap);
vector<int> findVerticalSeamImage(FloatImage im);
vector<int> findHorizontalSeamImage(FloatImage im);


FloatImage addSeamToMask(FloatImage const mask, vector<int> seam, bool isHorizontal);
FloatImage addSeam(const FloatImage im, vector<int> seam, bool isHorizontal);

FloatImage removeSeam(const FloatImage &im, vector<int> seam, bool isHorizontal);
FloatImage drawSeam(const FloatImage &im, const vector<int> seam, bool isHorizontal);

FloatImage removeObject(const FloatImage &im, const vector<tuple<int, int>> object, const vector<tuple<int, int>> protectedObject, bool lockRatio=false, bool onlyVert=false, bool onlyHorizontal=false);

vector<bool> seamOrientation(FloatImage badArea, int i, bool lockRatio, bool onlyVert, bool onlyHorizontal);

FloatImage grow(const FloatImage &im, const int addWidth, const int addHeight, const int numSteps);
FloatImage grow(const FloatImage &im, const int addWidth, const int addHeight, const int numSteps, const string outputPath);

FloatImage contentAmpilification(const FloatImage &im, float factor);
FloatImage contentAmplification(const FloatImage &im, const float factor, const string outputPath);

