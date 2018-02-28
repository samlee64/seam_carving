/*
    CS 89/189 Computational Aspects of Digital Photography C++ basecode.

    Adapted from MIT's 6.815/6.865 basecode, written and designed by:
        Frédo Durand
        Katherine L. Bouman
        Gaurav Chaurasia
        Adrian Vasile Dalca
        Neal Wadhwa

    With additions & modifications by
        Wojciech Jarosz
    for Dartmouth's CS 89/189.
*/

#include "a6.h"
#include <iostream>
#include <math.h>

using namespace std;

// create a new image that is k times bigger than the input by using nearest neighbor interpolation
FloatImage scaleNN(const FloatImage &im, float factor)
{
	// create new FloatImage that is factor times larger than input image
    FloatImage output(floor(im.width() * factor), floor(im.height() * factor), im.depth());

	// loop over new FloatImage pixels and set appropriate values (smartAccessor is probably overkill here)
    for (int y = 0; y < output.height(); y++) {
        for (int x = 0; x < output.width(); x++) {
           for (int z = 0; z < output.depth(); z++) {
               output(x, y, z) = im(round(x/factor), round(y/factor), z);
           }
        }
    }

	// return new image
	return output;
}

// using bilinear interpolation to assign the value of a location from its neighboring pixel values
float interpolateLin(const FloatImage &im, float x, float y, int z, bool clamp)
{
	// Hint: use smartAccessor() to handle coordinates outside the image
    int xsmall = floor(x);
    int xlarge = xsmall + 1;
    int ysmall = floor(y);
    int ylarge = ysmall + 1;

    float xFactor = x - xsmall;
    float topRow = (im.smartAccessor(xsmall, ysmall, z, clamp) * (1- xFactor)) +
            (im.smartAccessor(xlarge, ysmall, z, clamp) * (xFactor) );

    float bottomRow = (im.smartAccessor(xsmall, ylarge, z, clamp) * (1 - xFactor)) +
            (im.smartAccessor(xlarge, ylarge, z, clamp) * (xFactor));

    //now interpolate
    float yFactor = y - ysmall;
    float output = (topRow * (1 - yFactor)) + (bottomRow * (yFactor));
    //float output = (topRow *  yFactor) + (bottomRow * (1 - yFactor));

    return output;
}

float interpolateBicubic(const FloatImage &im, float x, float y, int z, bool clamp)
{
    //there are 16 points to consider
    //floor x, y and consider 1 pixel less than that value, that value, and 2 pixels more than that value
    int floorX = floor(x);
    int floorY = floor(y);

    float xFactor = x - floorX;
    float yFactor = y - floorY;

    //values taken from http://www.paulinternet.nl/?page=bicubic
    //interpolate vertically
    vector<float>  colInterpolation = {};
    for (int i = -1; i < 3; i++) {
        float top = im.smartAccessor(floorX - i, floorY - 1, z, clamp);
        float midTop = im.smartAccessor(floorX - i, floorY, z, clamp);
        float midBottom = im.smartAccessor(floorX - i, floorY + 1, z, clamp);
        float bottom = im.smartAccessor(floorX - i, floorY + 2, z, clamp);

        float A = -(top/2.0f) + ((midTop * 3.0f)/2.0f) - ((midBottom * 3.0)/2.0f) + (bottom/2.0f);
        float B = top - ((midTop * 5.0f)/2.0f) + (midBottom * 2.0f) - (bottom/2.0f);
        float C = (-top/2.0f) + (midBottom/2.0f);
        float D = midTop;

        float value = (pow(xFactor, 3) * A) + (pow(xFactor, 2) * B) + (xFactor * C) + D;
        colInterpolation.push_back(value);
    }

    //interpolate the vertical columns horizontally
    float top = colInterpolation[0];
    float midTop = colInterpolation[1];
    float midBottom = colInterpolation[2];
    float bottom = colInterpolation[3];

    float A = -(top/2.0f) + ((midTop * 3.0f)/2.0f) - ((midBottom * 3.0)/2.0f) + (bottom/2.0f);
    float B = top - ((midTop * 5.0f)/2.0f) + (midBottom * 2.0f) - (bottom/2.0f);
    float C = (-top/2.0f) + (midBottom/2.0f);
    float D = midTop;

    float value = (pow(yFactor, 3) * A) + (pow(yFactor, 2) * B) + (yFactor * C) + D;

    return value;
}

// create a new image that is k times bigger than the input by using bilinear interpolation
FloatImage scaleLin(const FloatImage &im, float factor)
{
	// create new FloatImage that is factor times larger than input image
    FloatImage output(floor(im.width() * factor), floor(im.height() * factor), im.depth());

    // loop over new FloatImage pixels and set appropriate values (use interpolateLin())
    for (int y = 0; y < output.height(); y++) {
        for (int x = 0; x < output.width(); x++) {
            for (int z = 0; z < output.depth(); z++) {
                output(x, y, z) = interpolateLin(im, x/factor, y/factor, z);
            }
        }
    }

    return output;
}

FloatImage scaleBicubic(const FloatImage &im, float factor) {
    FloatImage output(floor(im.width() * factor), floor(im.height() * factor), im.depth());

    for (int y = 0; y < output.height(); y++) {
        for (int x = 0; x < output.width(); x++) {
            for (int z = 0; z < output.depth(); z++) {
                output(x, y, z) = interpolateBicubic(im, x/factor, y/factor, z);
            }
        }
    }
    return output;
}

// rotate an image around its center by theta
FloatImage rotate(const FloatImage &im, float theta)
{
    FloatImage output(im.width(), im.height(), im.depth());

    int centerX = floor(im.width()/2);
    int centerY = floor(im.height()/2);

	//rotate im around its center by theta
    for (int y = 0; y < output.height(); y++) {
        for (int x = 0; x < output.width(); x++) {
            //translate
            int xprime = x - centerX;
            int yprime = y - centerY;

            //apply rotation
            float rotX = (cos(theta) * xprime) - (sin(theta) * yprime);
            float rotY = (sin(theta) * xprime) + (cos(theta) * yprime);

            //translate back
            for (int z = 0; z < output.depth(); z++) {
                output(x, y, z) = interpolateLin(im, rotX + centerX, rotY + centerY, z);
            }
        }
    }

	// return rotated image
    return output;
}