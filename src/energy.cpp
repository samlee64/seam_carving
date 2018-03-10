
#include "filtering.h"
#include "a2.h"
#include <math.h>
#include "energy.h"

FloatImage gradientEnergy(const FloatImage &im, bool clamp)
{
    // sobel filtering in x direction
    // create Sobel Filter that extracts horizontal gradients
    Filter sobelX(3, 3);
    sobelX(0,0) = -1.0; sobelX(1,0) = 0.0; sobelX(2,0) = 1.0;
    sobelX(0,1) = -2.0; sobelX(1,1) = 0.0; sobelX(2,1) = 2.0;
    sobelX(0,2) = -1.0; sobelX(1,2) = 0.0; sobelX(2,2) = 1.0;

    FloatImage xGradient = sobelX.Convolve(im, clamp);

    // sobel filtering in y direction
    // create Sobel Filter that extracts vertical gradients
    Filter sobelY(3, 3);
    sobelY(0,0) = -1.0; sobelY(1,0) = -2.0; sobelY(2,0) = -1.0;
    sobelY(0,1) = 0.0; sobelY(1,1) = 0.0; sobelY(2,1) = 0.0;
    sobelY(0,2) = 1.0; sobelY(1,2) = 2.0; sobelY(2,2) = 1.0;

    FloatImage yGradient = sobelY.Convolve(im, clamp);

    // compute squared magnitude
    FloatImage energyMap(im.width(), im.height(), 1);
    for (int y = 0; y < im.height(); y++) {
        for (int x = 0; x < im.width(); x++) {
            float energy = 0.0;
            for (int z = 0; z < im.depth(); z++) {
                energy += abs(xGradient(x, y, z)) + abs(yGradient(x, y, z));

            }
            energyMap(x, y, 0) = energy;
        }
    }

    return energyMap;
    //FloatImage output(im.width(), im.height(), 3);
    //for (int y = 0; y < im.height(); y++) {
    //    for (int x = 0; x < im.width(); x++) {
    //        for (int z = 0; z < im.depth(); z++) {
    //            output(x, y, z) = pow(pow(xGradient(x, y, z), 2) + pow(yGradient(x, y, z), 2), 0.5) ;
    //        }
    //    }
    //}
    //return output;
}

float dualGradientEnergy(const FloatImage &im, int x, int y, bool clamp)
{
    float changeX = 0.0f;
    float changeY = 0.0f;
    for (int z = 0; z < im.depth(); z++) {
        float rx = im.smartAccessor(x - 1, y, z, clamp) - im.smartAccessor(x + 1, y, z, clamp);
        changeX += pow(rx, 2);

        float ry = im.smartAccessor(x, y - 1, z, clamp) - im.smartAccessor(x, y + 1, z, clamp);
        changeY = pow(ry, 2);
    }
    return changeX + changeY;
}

FloatImage dualGradientEnergy(const FloatImage &im, bool clamp) {
    FloatImage energyMap(im.width(), im.height(), 1);
    for (int y = 0; y < im.height(); y++) {
        for (int x = 0; x < im.width(); x++) {
            energyMap(x, y, 0) = dualGradientEnergy(im, x, y, clamp);
        }
    }
    return energyMap;
}

// This funciton adds the mask to the im when calculating the non cumlative energy for each pixel
// this increases the gradient across the seam, reducing the chances that another seam comes across it.
//@params
// im: image to whose energy should be calculated
// mask: mask image. Assumed to have same dimensions as im
// value: value to add to the masked values
// isHorizontal: true -> calculate the energy from left to right
//              false -> calculate the energy from top to bottom
FloatImage createMaskedEnergyMap(FloatImage im, FloatImage mask, float value, bool isHorizontal)
{
    if (im.sizeX() != mask.sizeX() || im.sizeY() != mask.sizeY()) throw runtime_error("Image and mask are different dimensions");
    //apply the mask before the gradient
    FloatImage newIm = im + mask;
    FloatImage energyMap = dualGradientEnergy(newIm);

    //alright so the problem that I am running into right now is that areas are getting blocked off

    //calculate the aggregated energyMap
    if (isHorizontal) {
        for (int x = 1; x < im.width(); x++) {
            for(int y = 0; y < im.height(); y++) {
                float lowestEnergy = numeric_limits<float>::max();
                for (int change = -1; change <= 1; change++) {
                    if (y + change >= im.height() or y + change < 0) {continue;}
                        lowestEnergy = min(energyMap(x - 1, y + change, 0), lowestEnergy);
                }
                energyMap(x, y, 0) = energyMap(x, y, 0) + lowestEnergy;
            }
        }
        return energyMap;
    } else {
        for(int y = 1; y < im.height(); y++) {
            for (int x = 0; x < im.width(); x++) {
                float lowestEnergy = numeric_limits<float>::max();
                for (int change = -1; change <= 1; change++) {
                    if (x + change >= im.width() or x + change < 0) {continue;}
                        lowestEnergy = min(energyMap(x + change, y - 1, 0), lowestEnergy);
                }
                energyMap(x, y, 0) = energyMap(x, y, 0) + lowestEnergy;
            }
        }
        return energyMap;
    }
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

//need to create horizontal for thisj
//this creates an energy map and throws value param into anyplace where the block param has a 1
//@params
// im: float image
// block: float image. Has 1s wherever value should be placed on energyMap. Has -1s wherever -value should be placed on energyMap
// value: value to place into the blocked out areas
// isHorizontal: true -> sum left to right
//              false -> sum top to bottom


FloatImage createBlockedEnergyMap(FloatImage im, FloatImage block, int value, bool isHorizontal)
{
    if (im.sizeX() != block.sizeX() || im.sizeY() != block.sizeY()) throw runtime_error("Image and block are different dimensions");
    FloatImage energyMap = dualGradientEnergy(im);
    if (isHorizontal) {
        for (int x = 1; x < im.width(); x++) {
            for (int y = 0; y < im.height(); y++) {
                float lowestEnergy = 1000;
                for (int change = -1; change <= 1; change++) {
                    if (y + change >= im.height() or y + change < 0) {continue;}
                    lowestEnergy = min(energyMap(x - 1, y + change,0), lowestEnergy);
                }

                if (block(x, y , 0) == 1) {
                    energyMap(x, y, 0) = value;
                } else if (block(x, y, 0) == -1 ){
                    energyMap(x, y, 0) = -value;
                } else {
                    energyMap(x, y, 0) = energyMap(x, y, 0) + lowestEnergy;
                }
            }
        }
        return energyMap;
    } else {
        for (int y = 1; y < im.height(); y++) {
            for (int x = 0; x < im.width(); x++) {
                float lowestEnergy = 1000; //do i need to change this to be max value?
                for (int change = -1; change <= 1; change++) {
                    if (x + change >= im.width() or x + change < 0) {continue;}
                    lowestEnergy = min(energyMap(x + change, y - 1, 0), lowestEnergy);
                }

                if (block(x, y , 0) == 1) {
                    energyMap(x, y, 0) = value;
                } else if (block(x, y, 0) == -1 ){
                    energyMap(x, y, 0) = -value;
                } else {
                    energyMap(x, y, 0) = energyMap(x, y, 0)+ lowestEnergy;
                }
            }
        }
        return energyMap;
    }
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
