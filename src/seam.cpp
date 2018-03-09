/**************************************************************
 //                      SEAM CARVING  FUNCTIONS              //
 *************************************************************/
#include "morphing.h"
#include "utils.h"
#include "a2.h"
#include "a6.h"
#include "energy.h"
#include "seam.h"

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

vector<int> findHorizontalSeamMap(FloatImage energyMap)
{
    vector<int> seam;

    //find the root
    int minY = lowestColumnIndex(energyMap);
    seam.push_back(minY);

    int currentY = minY;
    int nextY;

    for (int x = energyMap.width() - 2; x >= 0; x--) {
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


//just hardcoding vertical seams right now
FloatImage removeSeam(const FloatImage im, vector<int> seam, bool isHorizontal)
{
    FloatImage output(im.width() - 1, im.height(), im.depth());
    //seam goes from high to low
    for (int reverseY = 0; reverseY < output.height(); reverseY++) {
        int badPixel = seam[reverseY]; //this is the x coord
        int offsetPixel = 0;

        for (int x = 0; x < output.width(); x++) {
            if ( x == badPixel) { //this is correct. I tried x + 1 and it removed from the wrong side of the seam during object removal
                offsetPixel = 1;
            }
            for (int z = 0; z < output.depth(); z++) {
                int y = im.height() - 1 - reverseY;
                output(x, y, z) = im(x + offsetPixel, y, z);
            }
        }
    }
    return output;
}

//modifies the existing mask
FloatImage addSeamToMask(FloatImage const mask, vector<int> seam)
{
    FloatImage output(mask.width() + 1, mask.height(), mask.depth());
    for (int reverseY = 0; reverseY < output.height(); reverseY++) {
        int badPixel = seam[reverseY]; //this is the x coord
        int offsetPixel = 0;

        //adding the seam to the image
        int y = mask.height() - 1 - reverseY;
        for (int x = 0; x < mask.width(); x++) {
            if ( x == badPixel ) {
                offsetPixel = -1;
                output(x, y, 0) = 1;
            } else {
                for (int z = 0; z < output.depth(); z++) {
                    output(x, y, z) = mask(x + offsetPixel, y, z);
                }
            }
        }
    }
    return output;
}
//hard coded vertical
FloatImage addSeam(const FloatImage im, vector<int> seam)
{
    FloatImage output(im.width() + 1, im.height(), im.depth());

    for (int reverseY = 0; reverseY < output.height(); reverseY++) {
        int badPixel = seam[reverseY]; //this is the x coord
        int offsetPixel = 0;

        //adding the seam to the image

        int y = im.height() - 1 - reverseY;
        for (int x = 0; x < im.width(); x++) {
            if ( x == badPixel ) {
                offsetPixel = -1;
                for (int z = 0; z < im.depth(); z++ ) {
                    float average = (im.smartAccessor(x - 1, y, z) + im.smartAccessor(x + 1, y, z)) / 2;
                    output(x, y, z) = average;
                }
            } else {
                for (int z = 0; z < output.depth(); z++) {
                    output(x, y, z) = im(x + offsetPixel, y, z);
                }
            }
        }
    }
    return output;
}


//naive expansion function
// will always pick the same seam
FloatImage grow(const FloatImage &im, int addWidth, int addHeight, int numSteps)
{
    cout << "grow " << endl;
    FloatImage mid(im);
    //so keep a seperate image for the emap?
    // or keep a seperate emap
    //keep a seperate emap that has all the seams in it


    FloatImage mask(im.width(), im.height(), 1);
    float highValue = 100000;
    for (int i = 0; i < addWidth; i++) {
        FloatImage eMap = createMaskedEnergyMap(mid, mask, highValue, false);

        char buffer[255];
        sprintf(buffer, DATA_DIR "/output/grow/energyMaps/eMap-%d.png", i);
        eMap.write(buffer);

        vector<int> seam = findVerticalSeamMap(eMap);
        for (int j = 0; j < seam.size(); j++) {
            int y = eMap.height() - 1 - j;
            //cout << seam[j] << " " << y << endl;
        }

        mid = addSeam(mid, seam);
        mask = addSeamToMask(mask, seam);

    }
    return mid;
}

//increase the image size by factor
// scale back to orignal size
FloatImage contentAmpfliication(const FloatImage &im, int factor)
{
}

//lets just remove objects vertically
// system should be able to automatically calculate the smaller of th evertical or horizontal diameters
//perform vertical or horizontal removals
//@params
// im: float image that has the object to remove
// object: vector of tuples that have x, y coordinates of the object
// lockRatio: true -> locks aspect ratio
// onlyVert: true -> will only remove vertical seams. Overridden by lockRatio
// onlyHorizontal: true -> will only remove horizontal seems. Overriden by lockRatio and onlyVert
FloatImage removeObject(const FloatImage &im, const vector<tuple<int, int>> object, bool lockRatio, bool onlyVert, bool onlyHorizontal)
{
    //alright now lets see if we can alternate between vertical and horizontal rips
    const int MAXTRIES = min(im.width(), im.height());
    FloatImage output(im);
    FloatImage test(DATA_DIR "/input/test4.png");
    for (int x = 6; x < 11; x++) {
        for (int y = 50; y < 57; y++) {
            test(x, y, 0) = 1;
            test(x, y, 1) = 0;
            test(x, y, 2) = 0;
        }
    }

    FloatImage badArea(im.width(), im.height(), 1); //values are inited to 0
    for (int i = 0; i < object.size(); i++) {
        int x = get<0>(object[i]);
        int y = get<1>(object[i]);
        badArea(x, y, 0) = 1;
    }
    badArea.write(DATA_DIR "/output/removal/badarea.png");

    //const float lowValue = numeric_limits<float>::min();
    const float lowValue = -10000000;
    FloatImage eMap;

    bool isHorizontal = false;

    //index 0 holds bool for stopping, index 1 holds bool for orientation
    vector<bool> continueAndOrientation = {};
    //remove the seam from the photo and the energyMap
    //I think that this also means that I need to recaluclate the energy map
    //so I should technically just do this until there is no more bad area

    for (int i = 0; i < MAXTRIES; i++) {
        //calculate the larger difference in area and perform the vertical or horizontal removal accordingly
        continueAndOrientation= seamOrientation(badArea, i, lockRatio, onlyVert, onlyHorizontal);
        isHorizontal = continueAndOrientation[1];
        if (isHorizontal) {
            cout << "removing horizontal at " << i << endl;
        } else {
            cout << "removing vertical at " << i << endl;
        }

        if (not continueAndOrientation[0]) {
            cout << "done removing bad area after " << i << " iterations" << endl;
            break;
        }

        eMap = createMaskedEnergyMap(output, badArea, lowValue, isHorizontal);

        //junk
        char buffer[255];
        sprintf(buffer, DATA_DIR "/output/removal/energy/energyMap-%d.png", i);
        eMap.write(buffer);

        vector<int> seam;
        if (isHorizontal) {
            seam = findHorizontalSeamMap(eMap);
        } else {
            seam = findVerticalSeamMap(eMap);
        }

        FloatImage med = drawSeam(output, seam, isHorizontal);
        char buffer5[255];
        sprintf(buffer5, DATA_DIR "/output/removal/seam-%d.png", i);
        med.write(buffer5);

        //no junk
        output = removeSeam(output, seam, isHorizontal);
        //no junk

        char buffer2[255];
        sprintf(buffer2, DATA_DIR "/output/removal/medoutput-%d.png", i);
        output.write(buffer2);


        test = removeSeam(test, seam, isHorizontal);
        char buffer6[255];
        sprintf(buffer6, DATA_DIR "/output/removal/test-%d.png", i);
        test.write(buffer6);


        //no junk
        badArea = removeSeam(badArea, seam, isHorizontal);
        //no junk

        char buffer3[255];
        sprintf(buffer3, DATA_DIR "/output/removal/medoutput-%d.png", i);
        badArea.write(buffer3);
    }



    return output;
}

//Returns true, true if a horizontal seam should be removed
// returns true, false if a vertical seam should be removed
// returns false, * if no seam should be removed
//
//@params
// badArea: area that should be removed
// i: the iteration that the removal is on
// all bool params false: returns the the direction that has the largest difference in area that needs to be removed
// lockRatio: true -> locks aspect ratio
// onlyVert: true -> will only remove vertical seams. Overridden by lockRatio
// onlyHorizontal: true -> will only remove horizontal seems. Overriden by lockRatio and onlyVert
vector<bool> seamOrientation(FloatImage badArea, int i, bool lockRatio, bool onlyVert, bool onlyHorizontal)
{
    //calculate the larger difference in area and perform the vertical or horizontal removal accordingly
    float checkSum = 0;
    int minY = badArea.height() - 1;
    int maxY = 0;

    int minX = badArea.width() - 1;
    int maxX = 0;

    for (int y = 0; y < badArea.height(); y++ ) {
        for (int x = 0; x < badArea.width(); x++) {
            checkSum += badArea(x, y, 0);
            if (badArea(x, y, 0) == 1) {
                minX = min(minX, x);
                maxX = max(maxX, x);
                minY = min(minY, y);
                maxY = max(maxY, y);
            }
        }
    }
    if (checkSum == 0) {
        return {false, false};
    } else if (lockRatio) {
        if (i % 2 == 1) {
            return {true, false};
        } else {
            return {true, true};
        }
    } else if (onlyVert) {
        return {true, false};
    } else if (onlyHorizontal) {
        return {true, true};
    } else if ((maxX - minX) > (maxY - minY)) {
        return {true, true}; //remove horizontal
    } else {
        return {true, false}; //remove vertical
    }
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

