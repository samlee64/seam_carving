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

// filtering.cpp
// Assignment 4

#include "filtering.h"
#include "a2.h"
#include <math.h>
#include <algorithm>

using namespace std;

/**************************************************************
 //            IMAGE CONVOLUTION AND FILTERING               //
 *************************************************************/

// convolve an image with a box filter of size k by k
FloatImage boxBlur(const FloatImage &im, const int &k, bool clamp)
{
	// create a new empty image
	FloatImage imFilter(im.width(), im.height(), im.channels());

	// convolve the image with the box filter
    for (int y = 0; y < im.height(); y++) {
		for(int x = 0; x < im.width(); x++) {
            for (int z = 0; z < im.depth(); z++) {
				//assume k is odd
				int dis = floor(k / 2);
				float avg = 0;

				for (int y2 = y - dis; y2 <= y + dis; y2++ ) {
					for (int x2 = x - dis; x2 <= x + dis; x2++ ) {
						avg += im.smartAccessor(x2, y2, z, clamp);
					}
				}
				imFilter(x, y, z) = (avg/(k*k));
			}
		}
	}
	return imFilter;
}



// reimeplement the box filter using the filter class.
// check that your results math those in the previous function "boxBlur"
FloatImage boxBlur_filterClass(const FloatImage &im, const int &k, bool clamp)
{
	// use Convolve() to apply convolution
    std:vector<float> kernel = {};
    float value = (float)1/(k*k);
    for (int i = 0; i < (k * k); i++) {
        kernel.push_back(value);
    }

    Filter blur(kernel, k, k);
    FloatImage output = blur.Convolve(im, clamp);


	return output;
}

// uses a Sobel kernel to compute the horizontal and vertical
// components of the gradient of an image and returns the gradient magnitude.
FloatImage gradientMagnitude(const FloatImage &im, bool clamp)
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

    FloatImage output(im.width(), im.height(), 3);
    for (int y = 0; y < im.height(); y++) {
        for (int x = 0; x < im.width(); x++) {
            for (int z = 0; z < im.depth(); z++) {
                output(x, y, z) = pow(pow(xGradient(x, y, z), 2) + pow(yGradient(x, y, z), 2), 0.5) ;
            }
        }
    }

    return output;
}

// create a vector containing the normalized values in a 1D Gaussian filter
vector<float> gauss1DFilterValues(float sigma, float truncate)
{
	// calculate the size of the filter
    int size = 1 + (2 * ceil(sigma*truncate));
    int center = size/2;

	// compute the un-normalized value of the gaussian
    vector<float> distribution;
    float sum = 0.0;
    for (int i = 0; i < size; i++) {
        int r = center - i;

        float power = -(pow(r, 2)/(2*pow(sigma, 2)));
        float value = pow(M_E, power);
        sum += value;
        distribution.push_back(value);
    }

    // normalize
    vector<float> normalized;
    for (int i = 0; i < size; i++) {
        normalized.push_back(distribution[i]/sum);
    }

    return normalized;
}

// blur across the rows of an image
FloatImage gaussianBlur_horizontal(const FloatImage &im, float sigma, float truncate, bool clamp)
{
	// filter in the x direction
   vector<float> filter = gauss1DFilterValues(sigma, truncate);
    Filter guassian(filter, filter.size(), 1);

    return guassian.Convolve(im, clamp);
}

// create a vector containing the normalized values in a 2D Gaussian filter
// needs to be a row major vector
vector<float> gauss2DFilterValues(float sigma, float truncate)
{
	// compute the filter size
    int size = 1 + (2 * ceil(sigma*truncate));
    int center = size/2;

    // compute the unnormalized value of the gaussian and put it in a row-major vector
    vector<float> distribution;
    float sum = 0.0;
    for (int y = 0; y < size; y++) {
        for (int x = 0; x <size; x++) {
            float r = pow(pow((center - x), 2) + pow((center - y), 2), 0.5);
            float power = -(pow(r, 2)/(2*pow(sigma, 2)));
            float value = pow(M_E, power);
            sum += value;
            distribution.push_back(value);
        }
    }

    // normalize
    vector<float> normalized;
    for (int i = 0; i < distribution.size(); i++) {
        normalized.push_back(distribution[i]/sum);
    }

    return normalized;
}

// Blur an image with a full  full 2D rotationally symmetric Gaussian kernel
FloatImage gaussianBlur_2D(const FloatImage &im, float sigma, float truncate, bool clamp)
{
    vector<float> kernel = gauss2DFilterValues(sigma, truncate);

    Filter gaussian(kernel, pow(kernel.size(), 0.5), pow(kernel.size(), 0.5));

    return gaussian.Convolve(im, clamp);
}

// Use principles of seperabiltity to blur an image using 2 1D Gaussian Filters
FloatImage gaussianBlur_seperable(const FloatImage &im, float sigma, float truncate, bool clamp)
{
	// blur using 2, 1D filters in the x and y directions

    vector<float> filter = gauss1DFilterValues(sigma, truncate);
    Filter guassianHorizontal(filter, filter.size(), 1);
    Filter guassianVertical(filter, 1, filter.size());

    return guassianVertical.Convolve(guassianHorizontal.Convolve(im, clamp), clamp);
}

// sharpen an image
FloatImage unsharpMask(const FloatImage &im, float sigma, float truncate, float strength, bool clamp)
{
	// get the low pass image and subtract it from the original image to get the high pass image
    FloatImage high_pass = im - gaussianBlur_2D(im, sigma, truncate, clamp);

    return (strength*high_pass) + im;
}

// Denoise an image using bilateral filtering
FloatImage bilateral(const FloatImage &im, float sigmaRange, float sigmaDomain, float truncateDomain, bool clamp)
{
    FloatImage output(im.width(), im.height(), im.channels());
    vector<float> spatialKernel = gauss2DFilterValues(sigmaDomain, truncateDomain);

    int sizeDomain = pow(spatialKernel.size(), 0.5); //get the size of a single side of the assumed square kernel

    for (int y = 0; y < im.height(); y++) {
        for (int x = 0; x < im.width(); x++) {

            //compute the intensity  kernel
            vector<float> intensityKernel;
            int center_x = floor(sizeDomain/2);
            int center_y = floor(sizeDomain/2);

            for (int j = 0; j < sizeDomain; j++) {
                for (int i = 0; i < sizeDomain; i++) {
                    //iterate through the color channels
                    float total_distance = 0.0;
                    for (int z = 0; z < im.depth(); z++) {
                        int xprime = x - center_x + i;
                        int yprime = y - center_y + j;

                        total_distance += pow(im.smartAccessor(x, y, z, clamp) - im.smartAccessor(xprime, yprime, z, clamp), 2);
                    }

                    float r = pow(total_distance, 0.5);
                    float power = -(pow(r, 2) / (2 * pow(sigmaRange, 2)));

                    float value = pow(M_E, power);
                    intensityKernel.push_back(value);
                }
            }

            //apply the intensity and spatial kernels
            for (int z = 0; z < im.depth(); z++) {
              float total = 0.0;
              float k = 0.0;
              for (int j = 0; j < sizeDomain; j++) { //y
                for (int i = 0; i < sizeDomain; i++) { //x
                    int xprime = x - center_x + i;
                    int yprime = y - center_y + j ;

                    //indexing logic taken from filter class operator()
                    float kernel_value =  intensityKernel[i + j * sizeDomain] * spatialKernel[i + j * sizeDomain];
                    k += kernel_value;
                    total += im.smartAccessor(xprime, yprime, z, clamp) * kernel_value;
                }
              }
              output(x, y, z) = total/k;
            }
        }
    }
	return output;
}

// Bilaterial Filter an image seperately for the Y and UV components of an image
FloatImage bilaYUV(const FloatImage &im, float sigmaRange, float sigmaY, float sigmaUV, float truncateDomain,
                   bool clamp)
{
    FloatImage yuv = rgb2yuv(im);
    FloatImage output(yuv.width(), yuv.height(), yuv.channels());
    vector<float> spatialY = gauss2DFilterValues(sigmaY, truncateDomain);
    vector<float> spatialUV = gauss2DFilterValues(sigmaUV, truncateDomain);

    int sizeDomainY = pow(spatialY.size(), 0.5); //get the size of a single side of the assumed square kernel
    int sizeDomainUV = pow(spatialUV.size(), 0.5);

    for (int y = 0; y < yuv.height(); y++) {
        for (int x = 0; x < yuv.width(); x++) {

            //compute the intensity  kernel for Y
            vector<float> intensityKernelY;
            int center_xY = floor(sizeDomainY/2);
            int center_yY = floor(sizeDomainY/2);

            for (int j = 0; j < sizeDomainY; j++) {
                for (int i = 0; i < sizeDomainY; i++) {
                    //iterate through the color channels
                    float total_distance = 0.0;
                    for (int z = 0; z < yuv.depth(); z++) {
                        int xprime = x - center_xY + i;
                        int yprime = y - center_yY + j;

                        total_distance += pow(yuv.smartAccessor(x, y, z, clamp) - yuv.smartAccessor(xprime, yprime, z, clamp), 2);
                    }

                    float r = pow(total_distance, 0.5);
                    float power = -(pow(r, 2) / (2 * pow(sigmaRange, 2)));

                    float value = pow(M_E, power);
                    intensityKernelY.push_back(value);
                }
            }

            //compute the intensity kernel for UV
            vector<float> intensityKernelUV;
            int center_xUV = floor(sizeDomainUV/2);
            int center_yUV = floor(sizeDomainUV/2);

            for (int j = 0; j < sizeDomainUV; j++) {
                for (int i = 0; i < sizeDomainUV; i++) {
                    //iterate through the color channels
                    float total_distance = 0.0;
                    for (int z = 0; z < yuv.depth(); z++) {
                        int xprime = x - center_xUV + i;
                        int yprime = y - center_yUV + j;

                        total_distance += pow(yuv.smartAccessor(x, y, z, clamp) - yuv.smartAccessor(xprime, yprime, z, clamp), 2);
                    }

                    float r = pow(total_distance, 0.5);
                    float power = -(pow(r, 2) / (2 * pow(sigmaRange, 2)));
                    float value = pow(M_E, power);
                    intensityKernelUV.push_back(value);
                }
            }

            //apply the intensity and spatial kernels
            for (int z = 0; z < yuv.depth(); z++) {
                float total = 0.0;
                float k = 0.0;
                if (z == 0) { //Y channel
                    for (int j = 0; j < sizeDomainY; j++) { //y
                        for (int i = 0; i < sizeDomainY; i++) { //x
                            int xprime = x - center_xY + i;
                            int yprime = y - center_yY + j ;

                            //indexing logic taken from filter class operator()
                            float kernel_value =  intensityKernelY[i + j * sizeDomainY] * spatialY[i + j * sizeDomainY];
                            k += kernel_value;
                            total += yuv.smartAccessor(xprime, yprime, z, clamp) * kernel_value;
                        }
                    }
                } else {  //UV channels
                    for (int j = 0; j < sizeDomainUV; j++) { //y
                        for (int i = 0; i < sizeDomainUV; i++) { //x
                            int xprime = x - center_xUV + i;
                            int yprime = y - center_yUV + j ;

                            //indexing logic taken from filter class operator()
                            float kernel_value =  intensityKernelUV[i + j * sizeDomainUV] * spatialUV[i + j * sizeDomainUV];
                            k += kernel_value;
                            total += yuv.smartAccessor(xprime, yprime, z, clamp) * kernel_value;
                        }
                    }
                }
                output(x, y, z) = total/k;
            }
        }
    }

    FloatImage RGBOutput = yuv2rgb(output);
    return RGBOutput;
}

//By channel
// Bilaterial Filter an image seperately for the Y and UV components of an image
FloatImage bilaYUVchannel(const FloatImage &im, float sigmaRange, float sigmaY, float sigmaUV, float truncateDomain,
                   bool clamp)
{
	// convert from RGB to YUV
    FloatImage yuv = rgb2yuv(im);
    FloatImage Y(yuv.width(), yuv.height(), 1);
    FloatImage UV(yuv.width(), yuv.height(), 2);

    //copy y over
    for (int y = 0; y < yuv.height(); y++) {
        for (int x = 0; x < yuv.width(); x++) {
            Y(x, y, 0) = yuv(x, y, 0);
        }
    }

    //copy uv over
    for (int y = 0; y < yuv.height(); y++) {
        for (int x = 0; x < yuv.width(); x++) {
            UV(x, y, 0) = yuv(x, y, 1);
            UV(x, y, 1) = yuv(x, y, 2);
        }
    }

    // denoise Y and UV channels using different domain sigmas
    FloatImage filteredY = bilateral(Y, sigmaRange, sigmaY, truncateDomain, clamp);
    FloatImage filteredUV = bilateral(UV, sigmaRange, sigmaUV, truncateDomain, clamp);

    //Recombine
    FloatImage filteredYUV(yuv.width(), yuv.height(), yuv.depth());
    for (int y = 0; y < yuv.height(); y++) {
        for (int x = 0; x < yuv.width(); x++) {
            filteredYUV(x, y, 0) = filteredY(x, y, 0);
            filteredYUV(x, y, 1) = filteredUV(x, y, 0);
            filteredYUV(x, y, 2) = filteredUV(x, y, 1);
        }
    }
	// convert from YUV back to RGB
    return yuv2rgb(filteredYUV);
}

//EC: Median filter
FloatImage medianFilter(const FloatImage &im, int filterWidth, int filterHeight, bool clamp)
{
    FloatImage output(im.width(), im.height(), im.depth());
   //need to check to make sure that both width and height are odd;
    if (filterHeight%2  == 0 or filterWidth%2 == 0) {
        throw invalid_argument("Height and width need to be odd");
    }

    int center_x = filterWidth/2;
    int center_y = filterHeight/2;
    //consider the area in the filterWidth and filterHeight. Find the median value

    for (int y = 0; y < im.height(); y++) {
        for (int x = 0; x < im.width(); x++) {
            for (int z = 0; z < im.depth(); z++) {
                vector<float> values;

                //add all the nearby pixels
                for(int j = 0; j < filterHeight; j++) {
                    for (int i = 0; i <filterWidth; i++) {
                        int xprime = x - center_x + i;
                        int yprime = y - center_y + j;
                        values.push_back(im.smartAccessor(xprime, yprime, z, clamp));
                    }
                }

                nth_element(values.begin(), values.begin() + values.size()/2, values.end());
                output(x, y, z) = values[values.size()/2];
            }
        }
    }

    return output;
}


//EC: Blur an image using many box blurs
FloatImage gaussianAproxBlur(const FloatImage &im, int numBoxes, float sigma, float truncate, bool clamp)
{
    int boxSize = 1 + (2 * ceil(sigma*truncate));

    FloatImage blurredImage(im);
    // convolve the image with the box filter
    for (int i = 0; i < numBoxes; i++) {
        blurredImage = fasterBoxBlur(blurredImage, boxSize, clamp);
    }

    return blurredImage;
}

// EC: faster box blur that only depends on image size
FloatImage fasterBoxBlur(const FloatImage &im, const int &k, bool clamp)
{
    // create a new empty image
    FloatImage horizontalBlur(im.width(), im.height(), im.depth());
    FloatImage combinedBlur(im.width(), im.height(), im.depth());

    // convolve the image with the box filter
    for (int y = 0; y < im.height(); y++) {
        for(int x = 0; x < im.width() - 1; x++) {
            for (int z = 0; z < im.depth(); z++) {
                int dis = floor(k / 2);
                //for the first average
                if (x == 0) {
                    float avg = 0;
                    for (int y2 = y - dis; y2 <= y + dis; y2++ ) {
                        for (int x2 = x - dis; x2 <= x + dis; x2++ ) {
                            avg += im.smartAccessor(x2, y2, z, clamp);
                        }
                    }
                    horizontalBlur(x, y, z) = (avg/(k*k));
                }
                horizontalBlur(x + 1, y, z) = (horizontalBlur.smartAccessor(x, y, z) +
                                        (im.smartAccessor(x + k + 1, y, z) - im.smartAccessor(x - k + 1, y, z))/((2*k) +1));
            }
        }
    }

    for(int x = 0; x < im.width(); x++) {
        for (int y = 0; y < im.height() - 1; y++) {
            for (int z = 0; z < im.depth(); z++) {
                int dis = floor(k / 2);
                //for the first average
                if (y == 0) {
                    float avg = 0;
                    for (int y2 = y - dis; y2 <= y + dis; y2++ ) {
                        for (int x2 = x - dis; x2 <= x + dis; x2++ ) {
                            avg += im.smartAccessor(x2, y2, z, clamp);
                        }
                    }
                    combinedBlur(x, y, z) = (avg/(k*k));
                }
                combinedBlur(x, y + 1, z) = (combinedBlur.smartAccessor(x, y, z) +
                                        (horizontalBlur.smartAccessor(x, y + k + 1, z) - horizontalBlur.smartAccessor(x, y - k + 1, z))/((2*k) +1));
            }
        }
    }

    return combinedBlur;
}
/**************************************************************
 //                 FILTER CLASS FUNCTIONS                  //
 *************************************************************/


// write a convolution function for the filter class
FloatImage Filter::Convolve(const FloatImage &im, bool clamp) const
{
	FloatImage imFilter(im.width(), im.height(), im.channels());

    //center x and center y of the kernel
    int center_x = floor(width/2);
    int center_y = floor(height/2);

	//iterate through the image
	for (int y = 0; y < im.height(); y++) {
		for (int x = 0; x < im.width(); x++) {
            for (int z = 0; z < im.depth(); z++) {

				float total = 0;
                //iterate through the kernel
                // starts at the top left corner
				for (int i = 0; i < width; i++) {
					for (int j = 0; j < height; j++) {
						total += im.smartAccessor(x + center_x - i, y + center_y - j, z, clamp) * operator()(i, j);
					}
				}

				imFilter(x, y, z) = total;
			}
		}
	}

	return imFilter;
}


/**************************************************************
 //               DON'T EDIT BELOW THIS LINE                //
 *************************************************************/

// Create an image of 0's with a value of 1 in the middle. This function
// can be used to test that you have properly set the kernel values in your
// Filter object. Make sure to set k to be larger than the size of your kernel
FloatImage impulseImg(const int &k)
{
	// initlize a kxkx1 image of all 0's
	FloatImage impulse(k, k, 1);

	// set the center pixel to have intensity 1
	int center = floor(k / 2);
	impulse(center, center, 0) = 1;

	return impulse;
}

Filter::Filter(const vector<float> &fData, const int &fWidth, const int &fHeight)
{
    if (fWidth%2 == 0 or fHeight%2 == 0) {
        throw invalid_argument("Height and width need to be odd");
    }

    if (fWidth * fHeight != fData.size()) {
        throw invalid_argument("width * height need to be the length of fdata");
    }


	kernel = fData;
	width = fWidth;
	height = fHeight;

}

Filter::Filter(const int &fWidth, const int &fHeight)
{
	width = fWidth;
	height = fHeight;
	kernel = std::vector<float>(width * height, 0);
}

const float &Filter::operator()(int x, int y) const
{
	if (x < 0 || x >= width)
		throw OutOfBoundsException();
	if (y < 0 || y >= height)
		throw OutOfBoundsException();

	return kernel[x + y * width];
}

float &Filter::operator()(int x, int y)
{
	if (x < 0 || x >= width)
		throw OutOfBoundsException();
	if (y < 0 || y >= height)
		throw OutOfBoundsException();

	return kernel[x + y * width];
}
Filter::~Filter() {}
