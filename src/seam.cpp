/**************************************************************
 //                      SEAM CARVING  FUNCTIONS              //
 *************************************************************/
#include "morphing.h"
#include "utils.h"
#include "a2.h"
#include "a6.h"
#include "energy.h"

using namespace std;

//return the Y coord that has the min value on either neighboring side of the given y coordj
int getMinY(FloatImage energyMap, int x, int y)
{
    int minY;
    float minEnergy = numeric_limits<float>::max();

    for (int changeY = -1; changeY <= 1; changeY++) {
        if (y + changeY >= energyMap.height() or y + changeY < 0) {continue;}
        if (energyMap(x, y + changeY, 0) < minEnergy) {
            minY = y + changeY;
            minEnergy = energyMap(x, minY, 0);
        }
    }
    return minY;
}


//return the X coord that has the min value of either neighboring side of the given x coord
int getMinX(FloatImage energyMap, int x, int y)
{
    int minX;
    float minEnergy = numeric_limits<float>::max();

    for (int changeX = -1; changeX <= 1; changeX++) {
        if (x + changeX >= energyMap.width() or x + changeX < 0) {continue;}
        if (energyMap(x + changeX, y, 0) < minEnergy) {
            minX = x + changeX;
            minEnergy = energyMap(minX, y, 0);
        }
    }
    return minX;
}

void debugPrintMap(FloatImage map)
{
    for (int y = 0; y < map.height(); y++) {
        for (int x = 0; x < map.width(); x++) {
            cout << round(map(x, y, 0)) << " ";
        }
        cout << endl;
    }
    cout << endl;

}

FloatImage energyMap(FloatImage im, bool debug)
{
    FloatImage energyMap = dualGradientEnergy(im);
    for(int y = 1; y < im.height(); y++) {
        for (int x = 0; x < im.width(); x++) {
            //for every pixel at this height, i go 1 up, 1 down
            float lowestEnergy = 10000000; //do i need to change this to be max value?
            for (int change = -1; change <= 1; change++) {
                if (x + change >= im.width() or x + change < 0) {
                    continue;
                }
                lowestEnergy = min(energyMap(x + change, y - 1, 0), lowestEnergy);
            }
            energyMap(x, y, 0) = energyMap(x, y, 0) + lowestEnergy;
        }
    }
    return energyMap;
}


//finds the lowest energy value in the bottom row
// returns the index
int lowestRowIndex(FloatImage energyMap)
{
    int minX;
    float minEnergy = numeric_limits<float>::max();

    for (int x = 0; x < energyMap.width(); x++) {
        float e = energyMap(x, energyMap.height() - 1, 0);
        if (e < minEnergy) {
            minEnergy = e;
            minX = x;
        }
    }
    return minX;
}

//returns the lowest energy value of the last column of the photo
int lowestColumnIndex(FloatImage energyMap)
{
    int minY;
    float minEnergy = numeric_limits<float>::max();

    for (int y = 0; y < energyMap.height(); y++) {
        float e = energyMap(energyMap.width() - 1, y, 0);
        if (e < minEnergy) {
            minEnergy = e;
            minY = y;
        }
    }
    return minY;
}

vector<tuple<int, int>> findVerticalSeam(FloatImage energyMap)
{
    vector<tuple<int, int>> seam;

    //find the root
    int minX = lowestRowIndex(energyMap);
    int minY = energyMap.height() - 1;
    seam.push_back(make_tuple(minX, minY));

    int currentX = minX;
    int nextX;

    for (int y = energyMap.height() - 2; y >= 0; y--) {
        float minEnergy = numeric_limits<float>::max();
        for (int changeX = -1; changeX <= 1; changeX++) {
            if (currentX + changeX >= energyMap.width() or currentX + changeX < 0) {
                continue;
            }
            if (energyMap(currentX + changeX, y, 0) < minEnergy) {
                nextX = currentX + changeX;
                minEnergy = energyMap(nextX, y, 0);
            }
        }
        currentX = nextX;
        seam.push_back(make_tuple(nextX, y));
    }
    return seam;
}

vector<int> findVerticalSeam1(FloatImage im)
{
    vector<int> seam;
    FloatImage energyMap(im.width(), im.height(), 1);

    for (int y = 1; y < energyMap.height(); y++) {
        for (int x = 0; x < energyMap.width(); x++) {
            float minEnergy = numeric_limits<float>::max();
            for (int changeX = -1; changeX <= 1; changeX++) {
                if (x + changeX >= energyMap.width() or x + changeX < 0) {continue;}
                minEnergy = min(energyMap(x + changeX, y - 1, 0), minEnergy);
            }
            energyMap(x, y, 0) = dualGradientEnergy(im, x, y) + minEnergy;
        }
    }

    int minX = lowestRowIndex(energyMap);
    seam.push_back(minX);

    int currentX = minX;
    int nextX;

    for (int y = energyMap.height() - 2; y >= 0; y--) {
        float minEnergy = numeric_limits<float>::max();
        nextX = getMinX(energyMap, currentX, y);
        currentX = nextX;
        seam.push_back(nextX);
    }

    return seam;
}
vector<int> findHorizontalSeam(FloatImage im)
{
    vector<int> seam;
    FloatImage energyMap(im.width(), im.height(), 1);

    //generate the aggregate energy map
    for (int x = 1; x < energyMap.width(); x++) {
        for (int y = 0; y < energyMap.height(); y++) {
            float minEnergy = numeric_limits<float>::max();

            for (int changeY = -1; changeY <= 1; changeY++) {
                if (y + changeY >= energyMap.height() or y + changeY < 0) {continue;}
                minEnergy = min(energyMap(x - 1, y + changeY, 0), minEnergy);
            }
            energyMap(x, y, 0) = dualGradientEnergy(im, x, y) + minEnergy;
        }
    }

    energyMap.write(DATA_DIR "/output/new-surf-horizontal-energy.png");

    int minY = lowestColumnIndex(energyMap);
    seam.push_back(minY);

    int currentY = minY;
    int nextY;

    for (int x = energyMap.width() - 2; x >= 0; x-- ) {
        float minEnergy = numeric_limits<float>::max();
        for (int changeY = -1; changeY <= 1; changeY++) {
            if (currentY + changeY >= energyMap.height() or currentY + changeY < 0) {continue;}
            if (energyMap(x, currentY + changeY, 0) < minEnergy) {
                nextY = currentY + changeY;
                minEnergy = energyMap(x, nextY, 0);
            }
        }
        currentY = nextY;
        seam.push_back(nextY);
    }
    return seam;
}



FloatImage removeSeam(const FloatImage im, vector<tuple<int, int>> seam)
{
    FloatImage output(im.width() - 1, im.height(), im.depth());
    //seam goes from high to low
    for (int y = output.height() - 1; y >= 0; y--) {
        int badPixel = get<0>(seam[y]); //this is the x coord
        //prety sure i need to index badPixel the other way around
        int offsetPixel = 0;
        for (int x = 0; x < output.width(); x++) {
            if ( x + 1== badPixel ) {
                offsetPixel = 1;
            }
            for (int z = 0; z < output.depth(); z++) {
                output(x, y, z) = im(x + offsetPixel, y, z);
            }
        }
    }
    return output;
}

FloatImage drawSeam(const FloatImage &im, const vector<int> seam, bool horizontal)
{
    FloatImage output(im);
    if (horizontal) {
        for (int i = 0; i < seam.size(); i++) {
            for (int z = 0; z < im.depth(); z++) {
                if (z == 0) {
                    output(i, seam[i], z) = 1;
                } else {
                    output(i, seam[i], z) = 0;
                }
            }
        }
    } else {
        for (int j = 0; j < seam.size(); j++) {
            for (int z = 0; z < im.depth(); z++) {
                if (z == 0) {
                    output(seam[j], j, 0) = 1;
                } else {
                    output(seam[j], j, 0) = 0;
                }
            }
        }
    }
    return output;
}

FloatImage drawSeam1(const FloatImage &im, const vector<tuple<int, int>> seam)
{
    FloatImage output(im);
    for (int i = 0; i < seam.size(); i++) {
        int x = get<0>(seam[i]);
        int y = get<1>(seam[i]);
        output(x, y, 0) = 1;
    }
    return output;
}