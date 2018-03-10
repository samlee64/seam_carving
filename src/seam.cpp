/**************************************************************
 //                      SEAM CARVING  FUNCTIONS              //
 *************************************************************/
#include "morphing.h"
#include "a6.h"
#include "energy.h"
#include "seam.h"

using namespace std;

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


// Returns the X index of the lowest energy value in the bottom row
// @params:
// energyMap: cumulative energymap
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

// Returns the Y index of the lowest energy value in the bottom column
// @params:
// energyMap: cumulative energymap
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

// Returns a min horizontal seam
//@params:
//  energyMap: cumulative energyMap
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
            if (currentY + changeY >= energyMap.height() or currentY + changeY < 0) {continue;} //dont consider items out of bounds
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

//Returns a min vertical seam
// @params:
// energyMap: cumulative energyMap
vector<int> findVerticalSeamMap(FloatImage energyMap)
{
    vector<int> seam;

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

//Returns a min vertical seam
// computes the energy map and then finds the min vertical seam
// should technically be slightly faster because there is 1 less mxn operation for image of size mxn
//@params:
// im: Float image
vector<int> findVerticalSeamImage(FloatImage im)
{
    vector<int> seam;
    FloatImage energyMap(im.width(), im.height(), 1);

    //calculate the cumulative energy
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

        for (int changeX = -1; changeX <= 1; changeX++) {
            if (currentX + changeX >= energyMap.width() or currentX + changeX < 0) {continue;}
            if (energyMap(currentX + changeX, y, 0) < minEnergy) {
                nextX = currentX + changeX;
                minEnergy = energyMap(currentX, y, 0);
            }
        }
        currentX = nextX;
        seam.push_back(nextX);
    }

    return seam;
}

//Returns a min horizontal seam
//@params:
// im: Float image
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


//Adds a seam to the mask
// Areas where the seam is are set to 1
//@params
// mask: float image mask that is going to have the seam added to i
// seam: the seam
// isHorizontal: whether the seam is vertical or horizontal
FloatImage addSeamToMask(FloatImage const mask, vector<int> seam, bool isHorizontal)
{
    if (isHorizontal) {
        FloatImage output(mask.width(), mask.height() + 1, mask.depth());
        for (int reverseX = 0; reverseX < output.width(); reverseX++) {
            int badPixel = seam[reverseX]; //this is a y coord
            int offsetPixel =0;

            int x = mask.width() - 1- reverseX;

            for (int y = 0; y < mask.height(); y++) {
                if (y == badPixel) {
                    offsetPixel = -1;
                    output(x, y, 0) = 1;
                } else {
                    for (int z = 0; z < output.depth(); z++) {
                        output(x, y, z) = mask(x, y + offsetPixel, z);
                    }
                }
            }
        }
        return output;
    } else {
        FloatImage output(mask.width() + 1, mask.height(), mask.depth());
        for (int reverseY = 0; reverseY < output.height(); reverseY++) {
            int badPixel = seam[reverseY]; //this is the x coord
            int offsetPixel = 0;

            //adding the seam to the image
            int y = mask.height() - 1 - reverseY;
            for (int x = 0; x < mask.width(); x++) {
                if (x == badPixel) {
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
}

//Adds the a seam to the im
// calculates the value of the seam by averaging its adajacent neighbors
//@params
// im: float image that is going to have a seam added to it
// seam: the seam
// isHorizontal: whether the seam is vertical or horizontal
FloatImage addSeam(const FloatImage im, vector<int> seam, bool isHorizontal)
{
    if (isHorizontal) {
        FloatImage output(im.width(), im.height() + 1, im.depth());
        for (int reverseX = 0; reverseX < output.width(); reverseX++) {
            int badPixel = seam[reverseX]; //this is a y coord
            int offsetPixel =0;

            int x = im.width() - 1- reverseX;

            for (int y = 0; y < im.height(); y++) {
                if (y == badPixel) {
                    offsetPixel = -1;
                    for (int z = 0; z < output.depth(); z++) {
                        float average = (im.smartAccessor(x, y - 1, z, false) + im.smartAccessor(x, y + 1, z, false)) / 2;
                        output(x, y, z) = average;
                    }
                } else {
                    for (int z = 0; z < output.depth(); z++) {
                        output(x, y, z) = im(x, y + offsetPixel, z);
                    }
                }
            }
        }
        return output;
    } else {
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
                        float average = (im.smartAccessor(x - 1, y, z, false) + im.smartAccessor(x + 1, y, z, false)) / 2;
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
}


//Removes a seam from an image
//@params
// im: image to remove the seam from
// seam: seam
// isHorizontal: true -> seam is horizontal
//                 false -> seam is vertical
FloatImage removeSeam(const FloatImage &im, vector<int> seam, bool isHorizontal)
{
    if (isHorizontal) {
        FloatImage output(im.width(), im.height() - 1, im.depth());
        for (int reverseX = 0; reverseX < output.width(); reverseX++) {
            int badPixel = seam[reverseX]; //this is the y coord
            int offsetPixel = 0;

            for (int y = 0; y < output.height(); y++) {
                if ( y == badPixel) {
                    offsetPixel = 1;
                }
                for (int z = 0; z < output.depth(); z++) {
                    int x = im.width() - 1 - reverseX;
                    output(x, y, z) = im(x, y + offsetPixel, z);
                }
            }
        }
        return output;
    } else {
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
}

//Expands the given image by addWith and addHeight
//@params:
// im: image to increase size of
// addWidth: number of pixels to add to the width
// addHeight: number of pixels to add to the height
//
FloatImage grow(const FloatImage &im, int addWidth, int addHeight, int numSteps)
{
    if (numSteps < 1 ) {throw runtime_error("Min number of steps is 1");}
    cout << "Growing" << endl;
    FloatImage mid(im);

    FloatImage mask(im.width(), im.height(), 1);
    float highValue = 1;
    for (int i = 0; i < addWidth; i++) {
        if (i % (addWidth/numSteps) == 0) {
            for (int i =0; i < mask.size(); i++) {
                mask(i) = 0;
            }
        }
        FloatImage eMap = createMaskedEnergyMap(mid, mask, highValue, false);

        char buffer[255];
        sprintf(buffer, DATA_DIR "/output/grow/energyMaps/eMap-%d.png", i);
        eMap.write(buffer);

        char buffer2[255];
        sprintf(buffer2, DATA_DIR "/output/grow/masks/mask-%d.png", i);
        mask.write(buffer2);


        vector<int> seam = findVerticalSeamMap(eMap);

        mask = addSeamToMask(mask, seam, false); //vertical

        char buffer3[255];
        sprintf(buffer3, DATA_DIR "/output/grow/mid/mid-%d.png", i);
        drawSeam(mid, seam, false).write(buffer3);
        mid = addSeam(mid, seam, false);
    }

    for (int i = 0; i < addHeight; i++) {
        if (i %(addHeight/numSteps) == 0 ) {
            for (int i =0; i < mask.size(); i++) {
                mask(i) = 0;
            }
        }
        FloatImage eMap = createMaskedEnergyMap(mid, mask, highValue, true);

        char buffer[255];
        sprintf(buffer, DATA_DIR "/output/grow/energyMaps/eMap-%d.png", i + addWidth);
        eMap.write(buffer);

        char buffer2[255];
        sprintf(buffer2, DATA_DIR "/output/grow/masks/mask-%d.png", i + addWidth);
        mask.write(buffer2);

        vector<int> seam = findHorizontalSeamMap(eMap);

        mask = addSeamToMask(mask, seam, true);
        mid = addSeam(mid, seam, true);
    }

    return mid;
}

//increase the image size by factor
// scale back to orignal size
//@params
// im: float image
// factor: factor to increase the content by. 2 would double it.

FloatImage contentAmpilification(const FloatImage &im, float factor)
{
    if (factor < 1) {
        return im;
    }

    FloatImage scaledImageNN = scaleNN(im, factor);
    FloatImage scaledImageLin = scaleLin(im, factor);

    int reduceWidth = scaledImageLin.width() - im.width();
    int reduceHeight = scaledImageLin.height() - im.height();

    scaledImageLin.write(DATA_DIR "/output/amplification/scaledImage.png");

    for (int i = 0; i < reduceWidth; i++) {
        cout << "Reducing width " <<  i << endl;
        bool isHorizontal = false;
        vector<int> seam = isHorizontal ? findHorizontalSeamImage(scaledImageLin) : findVerticalSeamImage(scaledImageLin);
        scaledImageLin = removeSeam(scaledImageLin, seam, isHorizontal);
        scaledImageLin.write(DATA_DIR "/output/amplification/sca.png");
    }
    for (int i = 0; i < reduceHeight; i++ ) {
        cout << "Reducing height " <<  i << endl;
        bool isHorizontal = true;
        vector<int> seam = isHorizontal ? findHorizontalSeamImage(scaledImageLin) : findVerticalSeamImage(scaledImageLin);
        scaledImageLin = removeSeam(scaledImageLin, seam, isHorizontal);
    }

    return scaledImageLin;
}

//lets just remove objects vertically
// system should be able to automatically calculate the smaller of th evertical or horizontal diameters
//perform vertical or horizontal removals
// destroy object takes precedance over protected object
//@params
// im: float image that has the object to remove
// object: vector of tuples that have x, y coordinates of the object
// lockRatio: true -> locks aspect ratio
// onlyVert: true -> will only remove vertical seams. Overridden by lockRatio
// onlyHorizontal: true -> will only remove horizontal seems. Overriden by lockRatio and onlyVert
FloatImage removeObject(const FloatImage &im, const vector<tuple<int, int>> destroyObject, const vector<tuple<int, int>> protectedObject, bool lockRatio, bool onlyVert, bool onlyHorizontal)
{
    const int MAXTRIES = min(im.width(), im.height()); //Stop before removing more than 50% of the photo
    const float lowValue = -100;

    FloatImage output(im);

    FloatImage badArea(im.width(), im.height(), 1); //area to destroy/protect
    FloatImage copy(im); //copy of the image to

    //Set protected areas to -1
    for (int i = 0; i < protectedObject.size(); i++) {
        int x = get<0>(protectedObject[i]);
        int y = get<1>(protectedObject[i]);
        badArea(x, y, 0) = -1;
        copy(x, y, 1) = 1;
    }

    //Set destroy areas to 1
    for (int i = 0; i < destroyObject.size(); i++) {
        int x = get<0>(destroyObject[i]);
        int y = get<1>(destroyObject[i]);
        badArea(x, y, 0) = 1;
        copy(x, y, 0) = 1;
    }
    copy.write(DATA_DIR "/output/removal/protect-destroy-areas.png");
    badArea.write(DATA_DIR "/output/removal/badareas.png");

    FloatImage eMap;

    //index 0 holds bool for stopping, index 1 holds bool for orientation
    vector<bool> continueAndOrientation = {};

    for (int i = 0; i < MAXTRIES; i++) {
        //calculate the larger difference in area and perform the vertical or horizontal removal accordingly
        continueAndOrientation= seamOrientation(badArea, i, lockRatio, onlyVert, onlyHorizontal);
        bool isHorizontal = continueAndOrientation[1];

        if (isHorizontal) {
            cout << "Removing horizontal: " << i << endl;
        } else {
            cout << "Removing vertical: " << i << endl;
        }

        if (not continueAndOrientation[0]) {
            cout << "Done removing bad area after " << i << " iterations" << endl;
            break;
        }

        //create the EnergyMap
        eMap = createBlockedEnergyMap(output, badArea, lowValue, isHorizontal);

        //junk
        if (i % 3 == 0) {
            char buffer[255];
            sprintf(buffer, DATA_DIR "/output/removal/energy/energyMap-%d.png", i);
            eMap.write(buffer);
        }

        vector<int> seam;
        if (isHorizontal) {
            seam = findHorizontalSeamMap(eMap);
        } else {
            seam = findVerticalSeamMap(eMap);
        }

        FloatImage med = drawSeam(output, seam, isHorizontal);
        if (i % 3 == 0) {
            char buffer5[255];
            sprintf(buffer5, DATA_DIR "/output/removal/seams/seam-%d.png", i);
            med.write(buffer5);
        }

        //no junk
        output = removeSeam(output, seam, isHorizontal);
        //no junk

        if (i % 3 == 0) {
            char buffer2[255];
            sprintf(buffer2, DATA_DIR "/output/removal/midOut/mid-output-%d.png", i);
            output.write(buffer2);
        }

        //no junk
        badArea = removeSeam(badArea, seam, isHorizontal);
        //no junk
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
            if (badArea(x, y, 0) == 1) {
                checkSum += badArea(x, y, 0);
                minX = min(minX, x);
                maxX = max(maxX, x);
                minY = min(minY, y);
                maxY = max(maxY, y);
            }
        }
    }

    if (checkSum <= 0) {
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


//Draws a red seam on the image
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

