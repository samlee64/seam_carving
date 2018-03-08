
#include "filtering.h"
#include "a2.h"
#include <math.h>

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
    float rx = im.smartAccessor(x - 1, y, 0, clamp) - im.smartAccessor(x + 1, y, 0, clamp);
    float gx = im.smartAccessor(x - 1, y, 1, clamp) - im.smartAccessor(x + 1, y, 1, clamp);
    float bx = im.smartAccessor(x - 1, y, 2, clamp) - im.smartAccessor(x + 1, y, 2, clamp);
    float changeX = pow(rx, 2) + pow(gx, 2) + pow(bx, 2);

    float ry = im.smartAccessor(x, y - 1, 0, clamp) - im.smartAccessor(x, y + 1, 0, clamp);
    float gy = im.smartAccessor(x, y - 1, 1, clamp) - im.smartAccessor(x, y + 1, 1, clamp);
    float by = im.smartAccessor(x, y - 1, 2, clamp) - im.smartAccessor(x, y + 1, 2, clamp);

    float changeY = pow(ry, 2) + pow(gy, 2) + pow(by, 2);

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

