/**************************************************************
 //                      SEAM CARVING  FUNCTIONS              //
 *************************************************************/
#include "morphing.h"
#include "utils.h"
#include "a2.h"
#include "a6.h"
#include "energy.h"

using namespace std;

FloatImage energyMap(FloatImage im, bool debug)
{
    FloatImage energyMap = gradientEnergy(im);
    //alright so i start at the second row
    for(int y = 1; y < im.height(); y++) {
        //i go through each box in the row
        for (int x = 0; x < im.width(); x++) {
            //for every pixel at this height, i go 1 up, 1 down
            float lowestEnergy = 100; //do i need to change this to be max value?

            //I dont think that this is write. x and y should be flipped
            for (int changeY = -1; changeY <= 1; changeY++) {
                if (x + changeY >= im.width() or x + changeY < 0) {
                    continue;
                }
                lowestEnergy = min(energyMap(x + changeY, y - 1, 0), lowestEnergy);
            }

            energyMap(x, y, 0) = energyMap(x, y, 0) + lowestEnergy;
        }
    }
    if (true) {
        for (int y = 0; y < im.height(); y++) {
            for (int x = 0; x < im.width(); x++) {
                cout << energyMap(x, y, 0) << " ";
            }
            cout << endl;
        }
    }
    return energyMap;
}


float energy(FloatImage im, int x, int y)
{
    return 0.0;

}
//returns a list of x, y, values
//defaults to finding vertical seams

vector<tuple<int, int>> findSeam(FloatImage energyMap, bool horizontal)
{
    vector<tuple<int, int>> seam;
    //so now i have this updated energy map. I now need to iterate through the last row and find the lowest energy value

    //find the root
    float minEnergy = 1000;
    int minX;
    int minY;
    for (int x = 0; x < energyMap.width(); x++) {
        float e = energyMap(x, energyMap.height() - 1, 0);
        if (e < minEnergy) {
            minEnergy = e;
            minX = x;
            minY = energyMap.height() - 1;
        }
    }
    seam.push_back(make_tuple(minX, minY));

    //then propogate up
    int prevX = minX;
    int prevY = minY;
    minEnergy = 1000;

    for (int y = energyMap.height() - 2; y >= 0; y--) {
        int nextX;

        for (int changeX = -1; changeX <= 1; changeX++) {
            if (prevX + changeX >= energyMap.width() or prevX + changeX < 0) {
                continue;
            }
            if (energyMap(prevX + changeX, y, 0) < minEnergy) {
                minEnergy = energyMap(prevX + changeX, y, 0);
                nextX = prevX + changeX;
            }
        }
        seam.push_back(make_tuple(nextX, y));
    }
    // I can then just backtrace
    for (int i = 0; i < seam.size(); i++) {
        cout << get<0>(seam[i]) << " " << get<1>(seam[i]) << endl;

    }

        cout << seam.size() << endl;
    return seam;

}


FloatImage drawSeam(const FloatImage im, const vector<tuple<int, int>> seam)
{
    FloatImage output(im);
    for (int i = 0; i < seam.size(); i++) {
        int x = get<0>(seam[i]);
        int y = get<1>(seam[i]);
        output(x, y, 0) = 1;
        output(x, y, 1) = 0;
        output(x, y, 2) = 0;
    }
    return output;
}