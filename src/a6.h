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
#pragma once

#include "floatimage.h"

// A6 Functions: Scaling and Rotating an Image
FloatImage scaleNN(const FloatImage &im, float factor);
FloatImage scaleLin(const FloatImage &im, float factor);
float interpolateLin(const FloatImage &im, float x, float y, int z, bool clamp=false);
float interpolateBicubic(const FloatImage &im, float x, float y, int z, bool clamp=false);
FloatImage scaleBicubic(const FloatImage &im, float factor);
FloatImage rotate(const FloatImage &im, float theta);