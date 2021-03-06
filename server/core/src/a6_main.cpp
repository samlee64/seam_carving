#include "morphing.h"
#include "a6.h"

using namespace std;

// test the smart accessor function
void testSmartAccessor()
{
	// load an image and create 2 images that will test the smart accessor
	const FloatImage input(DATA_DIR "/input/a6/bear.png");
	input.write(DATA_DIR "/output/bear.png");

	FloatImage clampTrue(input.width(), input.height(), input.channels());
	FloatImage clampFalse(input.width(), input.height(), input.channels());

	for (int z = 0; z < input.channels(); z++)
	{
		for (int x = 0; x < input.width(); x++)
		{
			for (int y = 0; y < input.height(); y++)
			{
				// replace non-valid pixel values with the value of the nearest pixel
				clampTrue(x, y, z) = input.smartAccessor(x - 10, y - 10, z, true);
				// replace non-valid pixel values with black (value=0)
				clampFalse(x, y, z) = input.smartAccessor(x - 10, y - 10, z);
			}
		}
	}

	clampTrue.write(DATA_DIR "/output/smartAccessor_clampTrue.png");
	clampFalse.write(DATA_DIR "/output/smartAccessor_clampFalse.png");
}

// a function to test scaling
void testScaling()
{
	// load in the image and print out original size
	float fact = 2.253;
	const FloatImage bostonim(DATA_DIR "/input/a6/BostonRainbow-crop-400.png");
	printf("Boston image is %dx%dx%d\n", bostonim.width(), bostonim.height(), bostonim.channels());

	// scale using NN interpolation and print the size of the new image
	FloatImage scaledNN = scaleNN(bostonim, fact);
	scaledNN.write(DATA_DIR "/output/BostonRainbow-scaled-NN.png");
	printf("Scaled-NN image is %dx%dx%d\n", scaledNN.width(), scaledNN.height(), scaledNN.channels());

	// scale using bilinear interpolation and print the size of the new image
	FloatImage scaledLin = scaleLin(bostonim, fact);
	scaledLin.write(DATA_DIR "/output/BostonRainbow-scaled-Lin.png");
	printf("Scaled-Lin image is %dx%dx%d\n", scaledLin.width(), scaledLin.height(), scaledLin.channels());
}

// a function to test rotation for those who have done it
void testRotation()
{
	float theta = 3.141 / 4;

	const FloatImage bostonim(DATA_DIR "/input/a6/BostonRainbow-crop-400.png");

	FloatImage rot = rotate(bostonim, theta);
	rot.write(DATA_DIR "/output/BostonRainbow-rotated.png");
	printf("Scaled-Lin image is %dx%dx%d\n", rot.width(), rot.height(), rot.channels());
}

// test warp by 1
void testWarpBy1()
{
	FloatImage bearim(DATA_DIR "/input/a6/bear.png");

	cout << "width" << bearim.width() << endl;
	cout << "height" << bearim.height() << endl;

	// define before and after segments
	Segment segBefore(0, 0, 10, 0);
	Segment segAfter(10, 10, 30, 15);

	Segment segBefore1(128, 85, 108, 85);
	Segment segAfter1(128, 65, 108, 65);

	FloatImage warp1 = warpBy1(bearim, segBefore, segAfter);

	warp1.write(DATA_DIR "/output/warpBy1.png");

	vector<Segment> segsBefore = {segBefore};
	vector<Segment> segsAfter = {segAfter};
	FloatImage warp2 = warp(bearim, segsBefore, segsAfter, 1, 1, 1);
	warp2.write(DATA_DIR "/output/warp.png");
}

// a function to test your morphing function with the fredo and werewolf images
void testMorph()
{
	// load the images
	FloatImage fredo(DATA_DIR "/input/a6/fredo.png");
	FloatImage werewolf(DATA_DIR "/input/a6/werewolf.png");

	// print out the size of the two images
	printf("Fredo image is %dx%dx%d\n", fredo.width(), fredo.height(), fredo.channels());
	printf("Werewolf image is %dx%dx%d\n", werewolf.width(), werewolf.height(), werewolf.channels());

	// define the segments corresponding to fredo (segsBefore) and the werewolf (segsAfter)
	vector<Segment> segsBefore, segsAfter;

	segsBefore.push_back(Segment(87, 128, 109, 127));
	segsBefore.push_back(Segment(143, 127, 162, 131));
	segsBefore.push_back(Segment(96, 197, 129, 190));
	segsBefore.push_back(Segment(118, 221, 132, 200));
	segsBefore.push_back(Segment(140, 238, 165, 170));
	segsBefore.push_back(Segment(71, 242, 44, 196));
	segsBefore.push_back(Segment(9, 46, 34, 14));
	segsBefore.push_back(Segment(175, 66, 154, 27));

	segsAfter.push_back(Segment(83, 114, 107, 109));
	segsAfter.push_back(Segment(139, 103, 157, 101));
	segsAfter.push_back(Segment(100, 170, 132, 151));
	segsAfter.push_back(Segment(125, 198, 145, 171));
	segsAfter.push_back(Segment(142, 196, 158, 139));
	segsAfter.push_back(Segment(96, 211, 75, 180));
	segsAfter.push_back(Segment(11, 41, 33, 7));
	segsAfter.push_back(Segment(175, 56, 155, 13));

	int numFrames = 10;

	// create an image morphing between fredo and werewolf at time t=0.5
	vector<FloatImage> imMorph = morph(fredo, werewolf, segsBefore, segsAfter, numFrames);

	// write out images
	char buffer[255];
	for (int n = 0; n < numFrames + 2; n++)
	{
		FloatImage im = imMorph[n];
		sprintf(buffer, DATA_DIR "/output/fredo_werewolf_morph_%d.png", n);

		im.write(buffer);
	}
}



void classMorph(int studentNumber) {
	char buffer1[255];
	char buffer2[255];

	int studentNumberTwo = studentNumber + 1;

	if (studentNumber == 25)  studentNumberTwo = 0;

    //og code
	sprintf(buffer1, DATA_DIR "/input/a6/class-morph/class-morph-%02d.jpg", studentNumber);
	sprintf(buffer2, DATA_DIR "/input/a6/class-morph/class-morph-%02d.jpg", studentNumberTwo);

//	sprintf(buffer1, DATA_DIR "/input/a6/class-morph-%02d.jpg", studentNumber);
//	sprintf(buffer2, DATA_DIR "/input/a6/class-morph-%02d.jpg", studentNumberTwo);

	FloatImage start(buffer1);
	FloatImage end(buffer2);

	vector<Segment> segsBefore, segsAfter;

	segsBefore.push_back(Segment(194, 218, 232, 220));
	segsBefore.push_back(Segment(289, 219, 323, 218));
	segsBefore.push_back(Segment(138, 211, 159, 276));
	segsBefore.push_back(Segment(378, 213, 357, 283));
	segsBefore.push_back(Segment(215, 297, 305, 299));
	segsBefore.push_back(Segment(213, 301, 256, 327));
	segsBefore.push_back(Segment(264, 326, 300, 308));
	segsBefore.push_back(Segment(181, 191, 234, 191));
	segsBefore.push_back(Segment(287, 189, 338, 193));
	segsBefore.push_back(Segment(162, 285, 182, 335));
	segsBefore.push_back(Segment(187, 341, 231, 376));
	segsBefore.push_back(Segment(287, 375, 327, 347));
	segsBefore.push_back(Segment(332, 343, 354, 291));

	segsAfter.push_back(Segment(203, 223, 241, 224));
	segsAfter.push_back(Segment(293, 224, 326, 222));
	segsAfter.push_back(Segment(164, 219, 181, 299));
	segsAfter.push_back(Segment(388, 215, 366, 293));
	segsAfter.push_back(Segment(229, 299, 301, 298));
	segsAfter.push_back(Segment(231, 305, 262, 314));
	segsAfter.push_back(Segment(269, 316, 300, 305));
	segsAfter.push_back(Segment(192, 204, 244, 211));
	segsAfter.push_back(Segment(286, 206, 339, 204));
	segsAfter.push_back(Segment(187, 302, 202, 336));
	segsAfter.push_back(Segment(206, 342, 246, 362));
	segsAfter.push_back(Segment(296, 364, 337, 340));
	segsAfter.push_back(Segment(340, 333, 356, 290));

    int numFrames = 19;

	vector<FloatImage> imMorph = morph(start, end, segsBefore, segsAfter, numFrames);


	// write out images
	char buffer[255];
	for (int n = 0; n < 20; n++)
	{
		FloatImage im = imMorph[n];
		int classIndex = n + 20 * studentNumber;

		sprintf(buffer, DATA_DIR "/output/class-morph%03d.jpg", classIndex);

		im.write(buffer);
	}
}

void testBicubicScaling()
{
	float fact = 2.253;
	const FloatImage bostonim(DATA_DIR "/input/a6/BostonRainbow-crop-400.png");
	printf("Boston image is %dx%dx%d\n", bostonim.width(), bostonim.height(), bostonim.channels());

	FloatImage scaledB = scaleBicubic(bostonim, fact);
	scaledB.write(DATA_DIR "/output/BostonRainbow-scaled-bicubic.png");
	printf("Scaled-Bicubic image is %dx%dx%d\n", scaledB.width(), scaledB.height(), scaledB.channels());

};

void testWeightFunc()
{
	int width = 800;
	int height = 400;
	int a = 1;
	int b = 1;
	int p = 1;
	FloatImage output(width, height, 1);
	Segment topBar((width/2) - 40, height/2, (width/2) + 40, height/2);

	for (int y = 0; y < output.height(); y++) {
		for (int x = 0; x < output.width(); x++) {
			float weight = topBar.weight(x, y, a, b, p);
			output(x, y, 0) = weight;
		}
	}

	for (int x = (width/2)-40; x < (width/2)+40; x++) {
		output(x, height/2, 0) = 0;
	}
	output.write(DATA_DIR "/output/weight.png");
}
// This is a way for you to test your functions.
// We will not grade the contents of the main function
int main()
{
	// uncomment these test functions as you complete the assignment to test your code

    //try { testSmartAccessor();}   catch(...) {cout << "testSmartAccessor Failed!" << endl;}
    //try { testScaling(); }        catch(...) {cout << "testScaling Failed!" << endl;}
    //try { testRotation(); }       catch(...) {cout << "testRotation Failed!" << endl;}
	testBicubicScaling();
    //testWarpBy1();
    //try { testWarpBy1(); }        catch(...) {cout << "testWarpBy1 Failed!" << endl;}

    //testWeightFunc();
	 //try { classMorph(15); }        catch(...) {cout << "classMorph Failed!" << endl;}

    //try { testMorph(); }          catch(...) {cout << "testMorph Failed!" << endl;}
}
