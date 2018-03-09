
#include "seam.h"
#include "morphing.h"
#include "a6.h"
#include "energy.h"
#include <iomanip>

using namespace std;

void createTestImage()
{
    FloatImage test(100, 68, 1);
    for (int x = 0; x <test.width(); x++) {
        for (int y = 0; y < test.height(); y++) {
            test(x, y, 0) = 1;
        }
    }
    for (int x = 0; x <test.width(); x+= 10) {
        for (int y = 0; y < test.height(); y++) {
            test(x, y, 0) = 0;
        }
    }
    test.write(DATA_DIR "/input/test4.png");
}

void testEnergy()
{
    const FloatImage input(DATA_DIR "/input/test2.png");
    FloatImage output = gradientEnergy(input);
    output.write(DATA_DIR "/output/gradientEnergyTest.png");

    const FloatImage input2(DATA_DIR "/input/castle-micro.png");
    FloatImage output2 = gradientEnergy(input2);
    output2.write(DATA_DIR "/output/castle-micro-gradient.png");

    const FloatImage input3(DATA_DIR "/input/surf.png");
    FloatImage output3 = dualGradientEnergy(input3);
    output3.write(DATA_DIR "/output/surf-dualgradient.png");
}

void testEnergyMap()
{
    const FloatImage input2(DATA_DIR "/input/surf.png");
    FloatImage output2 = energyMap(input2);
    output2.write(DATA_DIR "/output/surf-energyMap.png");
}

void testFindHorizontalSeam()
{
    const FloatImage input(DATA_DIR "/input/surf.png");
    vector<int> seam = findHorizontalSeamImage(input);
    FloatImage output = drawSeam(input, seam, true);
    output.write(DATA_DIR "/output/surf-horizontal.png");
}

void testFindVerticalSeam()
{
    const FloatImage input(DATA_DIR "/input/surf.png");
    cout << input.width() << " " << input.height() << endl;
    vector<int> seam = findVerticalSeamImage(input);
    FloatImage output = drawSeam(input, seam, false);
    output.write(DATA_DIR "/output/surf-vertical.png");
}


void testFindSeam()
{
//    const FloatImage input(DATA_DIR "/input/japan-small.png");
    const FloatImage input(DATA_DIR "/input/surf.png");
    FloatImage eMap = energyMap(input);
    eMap.write(DATA_DIR "/output/surf-energyMap.png");
    const vector<int> seam = findVerticalSeamMap(eMap);
    FloatImage output = drawSeam(eMap, seam, false);
    output.write(DATA_DIR "/output/surf-energyMap-seam.png");
}

void testRemoveSeam()
{
    const FloatImage input(DATA_DIR "/input/castle-small.png");
    FloatImage mid(input);

    for (int i = 0; i < 50; i++) {
        FloatImage eMap = energyMap(mid);
        const vector<int> seam = findVerticalSeamMap(eMap);
        mid = removeSeam(mid, seam, false);
    }

    //FloatImage eMap = gradientEnergy(mid);
    //for (int y = 0; y < eMap.height(); y++) {
    //    for (int x = 0; x < eMap.width(); x++) {
    //        cout << setw(3) << round(eMap(x, y, 0)) << " ";
    //    }
    //    cout << endl;
    //}
    mid.write(DATA_DIR "/output/removed-castle-small.png");
}

void testCheck()
{
    const FloatImage input(DATA_DIR "/output/removed-castle-small-gradient.png");

    for (int y = 0; y < input.height(); y++) {
        for (int x = 0; x < input.width(); x++) {
            cout << setw(3) << round(input(x, y, 0)) << " ";
        }
        cout << endl;
    }

}

void testEnlarge()
{
    const FloatImage input(DATA_DIR "/input/castle-micro.png");
    FloatImage output = enlarge(input, 10, 0, 0);
    output.write(DATA_DIR "/output/enlarge/castle-micro.png");
}

void testRemoveObject()
{
    const FloatImage input(DATA_DIR "/input/castle-micro.png");
    vector<tuple<int, int>> object = {};

    FloatImage castle(input);
    //black out the castle
    for (int x = 60; x < input.width() - 5; x++) {
        for (int y = 10; y < 60; y++) {
            tuple<int, int> t = make_tuple(x, y);
            object.push_back(t);
            castle(x, y, 0) = 1;
            castle(x, y, 1) = 0;
            castle(x, y, 2) = 0;
        }
    }

    //black out the person
//    for (int x = 6; x < 11; x++) {
//        for (int y = 50; y < 57; y++) {
//            tuple<int, int> t = make_tuple(x, y);
//            object.push_back(t);
//            castle(x, y, 0) = 1;
//            castle(x, y, 1) = 0;
//            castle(x, y, 2) = 0;
//        }
//    }

    castle.write(DATA_DIR "/output/removal/test.png");

    FloatImage output = removeObject(input, object);
    cout << input.width() << " " << input.height() << endl;
    cout << output.width() << " " << output.height() << endl;
    output.write(DATA_DIR "/output/removal/test-castle-removal.png");
}

int main()
{
    //createTestImage();
  //  testEnergy();
   // testEnergyMap();
    //testFindSeam();
//    testRemoveSeam();
//    testFindHorizontalSeam();
//    testFindVerticalSeam();
    testEnlarge();
    //testRemoveObject();
}
