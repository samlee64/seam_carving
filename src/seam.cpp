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

FloatImage createEnergyMap(FloatImage im)
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

FloatImage energyMap(FloatImage im)
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

vector<int> findVerticalSeamMap(FloatImage energyMap)
{
    vector<int> seam;

    //find the root
    int minX = lowestRowIndex(energyMap);
    seam.push_back(minX);

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
        seam.push_back(nextX);
    }
    return seam;
}

vector<int> findVerticalSeamImage(FloatImage im)
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
        nextX = getMinX(energyMap, currentX, y);
        currentX = nextX;
        seam.push_back(nextX);
    }

    return seam;
}
vector<int> findHorizontalSeamImage(FloatImage im)
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


//Adds a seam to the image
//Averages the left and right neighbors
FloatImage addSeam(const FloatImage &im, const vector<int> seam)
{
    //given


}
//I should be adding all the seams at once
FloatImage enlarge(const FloatImage &im, int addWidth, int addHeight, int numSteps)
{
    //lets just do horizontal scaling first
    //we need to find the number of vertical seams
    //ive really gotta keep these lines distinct

//    FloatImage output(im.width() + addWidth, im.height() + addHeight, im.depth());
    FloatImage output(im);
    vector<vector<int>> seams;
    FloatImage energyMap(im.width(), im.height(), 1);

    for(int i = 0; i < addWidth; i++) {

        //so i need to add in whatever seams and give them max values
        // so that these pixels don't get repeated in other seams
        //maybe not max values
        //but super high ones
        //only have to do this for the most resent one actually. Should do this at the end of the function

        int highValue = 100000000000;
        for (int j = 0; j < seams.size(); j++) {
            for (int y = 0; y < seams[j].size(); y++) {
                int x = seams[j][y];
                energyMap(x, y, 0) = highValue;
            }
        }

        //recalculate the energy map
        // skipping over the recalculation of pixels that have super high energies to not overwrite them
        for (int y = 1; y < energyMap.height(); y++) {
            for (int x = 0; x < energyMap.width(); x++) {
                if (energyMap(x, y, 0) >= highValue) {continue;} else {
                    float minEnergy = numeric_limits<float>::max();
                    for (int changeX = -1; changeX <= 1; changeX++) {
                        if (x + changeX >= energyMap.width() or x + changeX < 0) { continue; }
                        minEnergy = min(energyMap(x + changeX, y - 1, 0), minEnergy);
                    }
                    energyMap(x, y, 0) = dualGradientEnergy(im, x, y) + minEnergy;
                }
            }
        }

        char buffer[255];
        sprintf(buffer, DATA_DIR "/output/energyMaps/adding-seams-%d.png", i);
        energyMap.write(buffer);

        //so lets find a new seam
        //optimize this later
        vector<int> seam;
        float minValue = numeric_limits<float>::max();
        int minX;
        for (int x = 0; x < energyMap.width(); x++) {
            //check to make sure that this seam already isn't in the seams
            bool newSeam = true;
            if (energyMap(x, energyMap.height() - 1, 0) < minValue) {
                for (int i = 0; i < seams.size(); i++) {
                    if ( x == seams[i][0]) {
                        newSeam = false;
                        break;
                    }
                }
                //this breaks if we are doubling the size of the image
                // because at some point every seam would have been added
                if (newSeam){
                    minValue = energyMap(x, energyMap.height() - 1, 0);
                    minX = x;
                }
            }
        }

        seam.push_back(minX);
        int currentX = minX;
        int nextX;
        for (int y = energyMap.height() - 2; y >= 0; y--) {
            nextX = getMinX(energyMap, currentX, y);
            currentX = nextX;
            seam.push_back(nextX);
        }

        seams.push_back(seam);
    }

    cout << seams.size() << endl;
    cout << seams[1].size() << endl;

    for (int u = 0; u < seams.size(); u++) {
        for (int o = 0; o < seams[u].size(); o++) {
            int y = energyMap.height() - 1 - o;
            int x = seams[u][o];
            cout << seams[u][o] << " " << y << endl;
            output(x, y, 0) = 1;
            output(x, y, 1) = 0;
            output(x, y, 2) = 0;
        }
        cout << endl;
    }





    return output;


}

//increase the image size by factor
// scale back to orignal size
FloatImage contentAmpfliication(const FloatImage &im, int factor)
{
}

//lets just remove objects vertically
void removeObject(const FloatImage &im, const vector<tuple<int, int>> object)
{
    const float lowValue = numeric_limits<float>::min();
    FloatImage eMap = createEnergyMap(im);
    //so lets iterate through the object and give each value a super low score in the energy map
    for (int i = 0; i < object.size(); i++) {
        int x = get<0>(object[i]);
        int y = get<1>(object[i]);
        eMap(x, y, 0) == lowValue;
    }

    findVerticalSeamMap(eMap);



}

FloatImage drawSeam(const FloatImage &im, const vector<int> seam, bool isHorizontal)
{
    FloatImage output(im);
    if (isHorizontal) {
        for (int i = 0; i < seam.size(); i++) {
            for (int z = 0; z < im.depth(); z++) {
                if (z == 0) {
                    output(im.width() - 1 - i, seam[i], z) = 1;
                } else {
                    output(im.width() - 1 - i, seam[i], z) = 0;
                }
            }
        }
    } else {

        for (int j = 0; j < seam.size(); j++) {
            for (int z = 0; z < im.depth(); z++) {
                if (z == 0) {
                    output(seam[j], im.height() - 1 - j, z) = 1;
                } else {
                    output(seam[j], im.height() - 1 - j, z) = 0;
                }
            }
        }
    }
    return output;
}

