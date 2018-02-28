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

// morphing.cpp
// Assignment 6


#include "morphing.h"
#include "utils.h"
#include "a2.h"
#include "a6.h"

using namespace std;

/**************************************************************
 //            IMAGE WARPING/MORPHING FUNCTIONS              //
 *************************************************************/

// warp an entire image according to a pair of segments.
FloatImage warpBy1(const FloatImage &im, Segment &segBefore, Segment &segAfter)
{
    FloatImage output(im.width(), im.height(), im.depth());
    for (int y = 0; y < output.height(); y++) {
        for (int x = 0; x < output.width(); x++) {
            float uAfter = segAfter.getU(x, y);
            float vAfter = segAfter.getV(x, y);

            Vec2f oldCoords = segBefore.UVtoX(uAfter, vAfter);
            for (int z = 0; z < output.depth(); z++) {
                output(x, y, z) = interpolateLin(im, oldCoords[0], oldCoords[1], z, true);
            }
        }
    }
	return output;
}

// warp an image according to a vector of before and after segments using segment weighting
FloatImage warp(const FloatImage &im, vector<Segment> &segsBefore, vector<Segment> &segsAfter, float a, float b,
                float p)
{
    FloatImage output(im.width(), im.height(), im.depth());
    for (int y = 0; y < output.height(); y++) {
        for (int x = 0; x < output.width(); x++) {
            float weightSum = 0;
            vector<float> weights = {};

            //calculate all the weights
            for (int i = 0; i < segsAfter.size(); i++) {
                float weight = segsAfter[i].weight(x, y, a, b, p);
                weights.push_back(weight);
                weightSum += weight;
            }

            //normalize the weights
            for (int i = 0; i < weights.size(); i++) {
                weights[i] = weights[i]/weightSum;
            }

            //calculate the pixel in the original image the output pixel maps to
            float newX = 0;
            float newY = 0;
            for (int i = 0; i < segsBefore.size(); i++) {
                float uAfter = segsAfter[i].getU(x, y);
                float vAfter = segsAfter[i].getV(x, y);
                Vec2f oldCoords = segsBefore[i].UVtoX(uAfter, vAfter);
                newX += oldCoords[0] * weights[i];
                newY += oldCoords[1] * weights[i];
            }

            for (int z = 0; z < output.depth(); z++) {
                output(x, y, z) = im.smartAccessor(round(newX), round(newY), z, true);
            }
        }
    }

	return output;
}

// return a vector of N images in addition to the two inputs that morphs going between im1 and im2 for the corresponding segments
vector<FloatImage> morph(const FloatImage &im1, const FloatImage &im2, vector<Segment> &segsBefore,
                         vector<Segment> &segsAfter, int N, float a, float b, float p, bool debug)
{
    N += 1;
    if (im1.height() != im2.height() or im1.width() != im2.width() or im1.depth() != im2.depth()) {
        throw MismatchedDimensionsException();
    }
    vector<FloatImage> output = {};
    vector<vector<Segment>> segsofSegsMiddle= {};

    for(int t = 0; t <= N; t++) {
        vector<Segment> segsMiddle = {};
        float time = (float)t/N;

        //iterate through all the segments
        for (int i = 0; i < segsBefore.size(); i++) {
            //interpolate segments
            float interpolatedPx = (segsBefore[i].P[0] * (1 - time)) + (segsAfter[i].P[0] * time);
            float interpolatedPy = (segsBefore[i].P[1] * (1 - time)) + (segsAfter[i].P[1] * time);
            float interpolatedQx = (segsBefore[i].Q[0] * (1 - time)) + (segsAfter[i].Q[0] * time);
            float interpolatedQy = (segsBefore[i].Q[1] * (1 - time)) + (segsAfter[i].Q[1] * time);

            Segment interpolatedSegment(interpolatedPx, interpolatedPy, interpolatedQx, interpolatedQy);
            segsMiddle.push_back(interpolatedSegment);
        }
        segsofSegsMiddle.push_back(segsMiddle);
    }

    for(int t = 0; t <= N; t++) {
        float time = (float)t/N;

        FloatImage warp1 = warp(im1, segsBefore, segsofSegsMiddle[t], a, b, p);
        FloatImage warp2 = warp(im2, segsAfter, segsofSegsMiddle[t], a, b, p);

        //add in the testing dots
        if (debug) {
            for (int j = 0; j < segsBefore.size(); j++) {
                //before segments
                for (int x = -1; x < 2; x++) {
                    for (int y = -1; y < 2; y++) {
                        //dots for before and after segments
                        warp1(segsBefore[j].P[0] + x, segsBefore[j].P[1] + y, 0) = 1;
                        warp1(segsBefore[j].Q[0] + x, segsBefore[j].Q[1] + y, 0) = 1;
                        warp2(segsAfter[j].P[0] + x, segsAfter[j].P[1] + y, 1) = 1;
                        warp2(segsAfter[j].Q[0] + x, segsAfter[j].Q[1] + y, 1) = 1;

                        //intermediate segments
                        warp2(segsofSegsMiddle[t][j].P[0] + x, segsofSegsMiddle[t][j].P[1] + y, 2) = 1;
                        warp2(segsofSegsMiddle[t][j].Q[0] + x, segsofSegsMiddle[t][j].Q[1] + y, 2) = 1;
                        warp1(segsofSegsMiddle[t][j].P[0] + x, segsofSegsMiddle[t][j].P[1] + y, 2) = 1;
                        warp1(segsofSegsMiddle[t][j].Q[0] + x, segsofSegsMiddle[t][j].Q[1] + y, 2) = 1;
                    }
                }

            }
        }

        //interpolate the two warped images and then push them to the output
        FloatImage interpolatedWarp(warp1.width(), warp1.height(), warp1.depth());
        for (int y = 0; y < warp1.height(); y++) {
            for (int x = 0; x < warp1.width(); x++) {
                for (int z = 0; z < warp1.depth(); z++) {
                    interpolatedWarp(x, y, z) = (warp1(x, y, z) * (1 - time)) + (warp2(x, y, z) * time);
                }
            }
        }
        output.push_back(interpolatedWarp);
    }

    return output;
}


/**************************************************************
 //                 SEGMENT CLASS FUNCTIONS                  //
 *************************************************************/

// Segment constructor takes in 2 points (x1,y1) and (x2,y2) corresponding to the ends of a segment and computes:
// P - 2-element vector to point (x1, y1)
// Q - 2-element vector to pont (x2, y2)
// PQ - 2-element vector from P to Q
// PQ2 - squared distance between P and Q
// PQlength - distance between P and Q
// PQDivByPQlength2 - 2-element vector PQ normalized by PQ2
// perpPQ - 2-element vector perpendicular to PQ
// perpPQDivByPQlength - 2-element vector perpPQ normalized by PQlength
Segment::Segment(float x1, float y1, float x2, float y2)
{
	P.fill(0.f);
	Q.fill(0.f);
	perpPQ.fill(0.f);

    P = {x1, y1};
    Q = {x2, y2};
    //i think that this is wrong
    PQ = subtract(P, Q);
    PQ2 = pow(x2 - x1, 2) + pow(y2 - y1, 2);
    PQlength = pow(PQ2, 0.5);
    PQDivByPQlength2 = scalarMult(PQ, 1/PQlength);

    float theta = 3.14159/2;
    float u1 = (cos(theta) * PQ[0]) - (sin(theta) * PQ[1]);
    float u2 = (sin(theta) * PQ[0]) - (cos(theta) * PQ[1]);
    perpPQ = {u1, u2};
    perpPQDivByPQlength = scalarMult(perpPQ, 1/PQlength);
}

// Implement the computation of the u coordinate of a point (x,y) with respect to a segment
float Segment::getU(float x, float y)
{
    //how do i calculate px
    Vec2f PX = subtract(P, {x, y});
    return dot(PX, PQ)/PQ2;
}

// Implement the computation of the v coordinate of a point (x,y) with respect to a segment
float Segment::getV(float x, float y)
{
    //how do i calculate x and perppq
    Vec2f PX = subtract(P, {x, y});
    return dot(PX, perpPQDivByPQlength);
}

// compute the new (x, y) position of a point given by the (u,v) location relative to another segment.
// return the point (x,y) in a 2-element vector
Vec2f Segment::UVtoX(float u, float v)
{
	// takes the u,v values and returns a coordinate - to be called from target segment
    return add(P, add((scalarMult(PQ, u)), (scalarMult(perpPQDivByPQlength, v)))) ;
}

// Implement distance from a point (x,y) to the segment. Remember the 3 cases from class
float Segment::dist(float x, float y)
{
    float v = getV(x, y);
    float u = getU(x, y);

    float distanceToSegment;
    if (0 <= u and u <= 1) {
        distanceToSegment = abs(v);
    } else {
        if ( u > 1 ) {
            u = u - 1;
        }
        distanceToSegment = pow(pow(u * PQlength, 2) + pow(v, 2), 0.5);
    }
    return distanceToSegment;
}

// compute the weight of a segment to a point (x,y) given the weight parameters a,b, and p
float Segment::weight(float x, float y, float a, float b, float p)
{
    float distance = dist(x, y);
    return pow(pow(PQlength, p)/(a + distance), b);
}

/**************************************************************
 //               DON'T EDIT BELOW THIS LINE                //
 *************************************************************/

// add 2 vectors of the same length.
Vec2f Segment::add(const Vec2f &vec1, const Vec2f &vec2)
{
	return Vec2f{{vec2[0]+vec1[0], vec2[1]+vec1[1]}};
}

// subtracts 2 vectors of the same length.
Vec2f Segment::subtract(const Vec2f &vec1, const Vec2f &vec2)
{
	return Vec2f{{vec2[0]-vec1[0], vec2[1]-vec1[1]}};
}

// takes the dot product between 2 vectors of the same length
float Segment::dot(const Vec2f &vec1, const Vec2f &vec2)
{
	return vec2[0]*vec1[0] + vec2[1]*vec1[1];
}

// mutliplies an entire vector by a scalor value
Vec2f Segment::scalarMult(const Vec2f &vec, float factor)
{
	return Vec2f{{vec[0]*factor, vec[1]*factor}};
}

// destructor
Segment::~Segment() {} // Nothing to clean up
