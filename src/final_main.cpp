
#include "seam.h"
#include "morphing.h"
#include "a6.h"
#include "energy.h"
#include <iomanip>

using namespace std;

void createTestImage()
{
    FloatImage test(10, 10, 1);
    test(1, 0, 0) = 1;
    test(1, 1, 0) = 1;
    test(1, 2, 0) = 1;
    test(1, 3, 0) = 1;
    test(1, 4, 0) = 1;
    test(1, 5, 0) = 1;
    test(1, 6, 0) = 1;
    test(1, 7, 0) = 1;
    test(1, 8, 0) = 1;
    test(1, 9, 0) = 1;

    test(3, 0, 0) = 1;
    test(3, 1, 0) = 1;
    test(3, 2, 0) = 1;
    test(3, 3, 0) = 1;
    test(3, 4, 0) = 1;
    test(3, 5, 0) = 1;
    test(3, 6, 0) = 1;
    test(3, 7, 0) = 1;
    test(3, 8, 0) = 1;
    test(3, 9, 0) = 1;
    test.write(DATA_DIR "/input/test3.png");
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
    FloatImage output2 = energyMap(input2, true);
    output2.write(DATA_DIR "/output/surf-energyMap.png");
}

void testFindHorizontalSeam()
{
    const FloatImage input(DATA_DIR "/input/surf.png");
    vector<int> seam = findHorizontalSeam(input);
    FloatImage output = drawSeam(input, seam, true);
    output.write(DATA_DIR "/output/surf-horizontal.png");
}


void testFindSeam()
{
//    const FloatImage input(DATA_DIR "/input/japan-small.png");
    const FloatImage input(DATA_DIR "/input/surf.png");
    FloatImage eMap = energyMap(input);
    eMap.write(DATA_DIR "/output/surf-energyMap.png");
    const vector<tuple<int, int>> seam = findVerticalSeam(eMap);
    //FloatImage output = drawSeam1(eMap, seam);
    //output.write(DATA_DIR "/output/surf-energyMap-seam.png");
}

void testRemoveSeam()
{
    const FloatImage input(DATA_DIR "/input/castle-small.png");
    FloatImage mid(input);

    for (int i = 0; i < 50; i++) {
        FloatImage eMap = energyMap(mid);
        const vector<tuple<int, int>> seam = findVerticalSeam(eMap);
        mid = removeSeam(mid, seam);
    }

    //FloatImage eMap = gradientEnergy(mid);
    //for (int y = 0; y < eMap.height(); y++) {
    //    for (int x = 0; x < eMap.width(); x++) {
    //        cout << setw(3) << round(eMap(x, y, 0)) << " ";
    //    }
    //    cout << endl;
    //}
    cout << input.width() << " " << input.height() << endl;
    cout << mid.width() << " " << mid.height() << endl;
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

int main()
{
   // createTestImage();
  //  testEnergy();
   // testEnergyMap();
    //testFindSeam();
//    testRemoveSeam();
    testFindHorizontalSeam();
}
