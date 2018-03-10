
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


void testGrow()
{
    const FloatImage input(DATA_DIR "/input/castle-large.png");
    cout << input.width() << " " << input.height() << endl;
    FloatImage output = grow(input, 60, 60, 2);
    cout << input.width() << " " << input.height() << endl;
    cout << output.width() << " " << output.height() << endl;
    output.write(DATA_DIR "/output/grow/castle-large-60-60.png");
}

void testRemoveObject()
{
    const FloatImage input(DATA_DIR "/input/castle-small.png");
    vector<tuple<int, int>> object = {};
    vector<tuple<int, int>> protectedObject= {};

    FloatImage castle(input);
    //black out the castle
    //large castle
    // x 580, width - 50
    // y 20, 610

    castle.write(DATA_DIR "/output/removal/castle-large-blocked.png");

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


    FloatImage output = removeObject(input, object, protectedObject, false, true, false);
    cout << input.width() << " " << input.height() << endl;
    cout << output.width() << " " << output.height() << endl;
    output.write(DATA_DIR "/output/removal/test-castle-removal-minlow.png");
}


void testRemoveandReplaceObject()
{
    const FloatImage input(DATA_DIR "/input/beach.png");
    //const FloatImage input(DATA_DIR "/input/beach.png");
    vector<tuple<int, int>> destroyObject = {};
    vector<tuple<int, int>> protectedObject= {};

    for(int x=298; x<308; x++) {for (int y=203; y <228; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=308; x<316; x++) {for (int y=213; y <229; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=315; x<325; x++) {for (int y=219; y <229; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=299; x<324; x++) {for (int y=228; y <235; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=325; x<332; x++) {for (int y=223; y <232; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=323; x<335; x++) {for (int y=226; y <235; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=306; x<315; x++) {for (int y=235; y <238; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=335; x<345; x++) {for (int y=227; y <234; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=295; x<302; x++) {for (int y=211; y <222; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=299; x<307; x++) {for (int y=201; y <205; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=308; x<312; x++) {for (int y=209; y <215; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=315; x<319; x++) {for (int y=217; y <220; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=333; x<340; x++) {for (int y=224; y <230; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=344; x<352; x++) {for (int y=231; y <235; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=343; x<349; x++) {for (int y=227; y <232; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=304; x<306; x++) {for (int y=236; y <241; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=308; x<301; x++) {for (int y=240; y <243; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=295; x<302; x++) {for (int y=245; y <245; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=314; x<325; x++) {for (int y=234; y <239; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=322; x<327; x++) {for (int y=239; y <244; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=312; x<332; x++) {for (int y=215; y <226; y++) {destroyObject.push_back(make_tuple(x, y));}}
    FloatImage output = removeObject(input, destroyObject, protectedObject, false, false, false);
    cout << input.width() << " " << input.height() << endl;
    cout << output.width() << " " << output.height() << endl;

    output.write(DATA_DIR "/output/removalAndReplace/removal-auto-pigeon-beach.png");

    int widthDiff = input.width() - output.width();
    int heightDiff = input.height() - output.height();

    output = grow(output, widthDiff, heightDiff, 2);
    output.write(DATA_DIR "/output/removalAndReplace/replace-pigeon-beach.png");


}
void testContentAmplification()
{
    const FloatImage input(DATA_DIR "/input/surf.png");
    cout << input.width() << " " << input.height() << endl;
    FloatImage output = contentAmpilification(input, 1.3);
    output.write(DATA_DIR "/output/amplification/surf-2.png");

}

void testAll()
{



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
    //testGrow();
    //testRemoveObject();
    testRemoveandReplaceObject();
    //testContentAmplification();
}
