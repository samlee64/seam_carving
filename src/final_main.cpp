
#include "seam.h"
#include "morphing.h"
#include "a6.h"
#include "energy.h"

using namespace std;

void createTestImage()
{
    FloatImage test(10, 10, 1);
    test(1, 0, 0) = 1;
    test(1, 1, 0) = 1;
    test(1, 2, 0) = 1;
    test(1, 3, 0) = 1;
    test(2, 4, 0) = 1;
    test(3, 5, 0) = 1;
    test(4, 6, 0) = 1;
    test(4, 7, 0) = 1;
    test(3, 8, 0) = 1;
    test(4, 9, 0) = 1;

    test.write(DATA_DIR "/input/test.png");
}

void testEnergy()
{
    const FloatImage input(DATA_DIR "/input/test.png");
    FloatImage output = gradientEnergy(input);
    output.write(DATA_DIR "/output/gradientEnergyTest.png");

    const FloatImage input2(DATA_DIR "/input/japan.png");
    FloatImage output2 = gradientEnergy(input2);
    output2.write(DATA_DIR "/output/gradientEnergy.png");
}

void testFindSeam()
{
    //const FloatImage input(DATA_DIR "/input/japan-small.png");
    const FloatImage input(DATA_DIR "/input/test.png");
    FloatImage eMap = energyMap(input);
    const vector<tuple<int, int>> seam = findSeam(eMap);
    FloatImage output = drawSeam(input, seam);
    output.write(DATA_DIR "/output/test-seam.png");
}
int main()
{
    //createTestImage();
    //testEnergy();
    testFindSeam();
}
