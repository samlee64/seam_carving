
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

void testRemoveGirlfriend()
{
    const FloatImage input(DATA_DIR "/input/couple.png");
    vector<tuple<int, int>> destroyObject = {};
    vector<tuple<int, int>> protectedObject= {};

    for(int x=152; x<180; x++) {for (int y=187; y <211; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=154; x<190; x++) {for (int y=213; y <261; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=155; x<190; x++) {for (int y=261; y <276; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=155; x<188; x++) {for (int y=277; y <328; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=153; x<191; x++) {for (int y=272; y <281; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=152; x<160; x++) {for (int y=281; y <298; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=193; x<181; x++) {for (int y=282; y <329; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=147; x<161; x++) {for (int y=222; y <230; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=150; x<156; x++) {for (int y=220; y <223; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=151; x<154; x++) {for (int y=219; y <219; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=144; x<148; x++) {for (int y=224; y <233; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=142; x<155; x++) {for (int y=233; y <224; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=140; x<149; x++) {for (int y=230; y <244; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=149; x<157; x++) {for (int y=242; y <233; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=192; x<195; x++) {for (int y=274; y <257; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=189; x<195; x++) {for (int y=226; y <259; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=186; x<195; x++) {for (int y=255; y <274; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=141; x<147; x++) {for (int y=244; y <252; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=139; x<148; x++) {for (int y=246; y <254; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=137; x<146; x++) {for (int y=255; y <260; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=137; x<144; x++) {for (int y=251; y <256; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=134; x<140; x++) {for (int y=258; y <263; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=131; x<138; x++) {for (int y=264; y <269; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=128; x<136; x++) {for (int y=270; y <272; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=151; x<137; x++) {for (int y=243; y <258; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=137; x<136; x++) {for (int y=254; y <264; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=145; x<131; x++) {for (int y=260; y <266; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=138; x<128; x++) {for (int y=267; y <272; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=135; x<125; x++) {for (int y=273; y <278; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=135; x<127; x++) {for (int y=271; y <276; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=131; x<124; x++) {for (int y=279; y <280; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=132; x<122; x++) {for (int y=277; y <283; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=196; x<198; x++) {for (int y=252; y <280; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=193; x<196; x++) {for (int y=255; y <275; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=194; x<203; x++) {for (int y=276; y <292; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=198; x<205; x++) {for (int y=289; y <313; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=193; x<214; x++) {for (int y=310; y <327; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=192; x<212; x++) {for (int y=305; y <312; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=215; x<194; x++) {for (int y=344; y <327; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=163; x<180; x++) {for (int y=327; y <384; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=192; x<184; x++) {for (int y=330; y <391; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=191; x<178; x++) {for (int y=323; y <391; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=154; x<178; x++) {for (int y=206; y <215; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=176; x<155; x++) {for (int y=187; y <183; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=152; x<150; x++) {for (int y=187; y <208; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=177; x<191; x++) {for (int y=326; y <391; y++) {destroyObject.push_back(make_tuple(x, y));}}


    for(int x=72; x<107; x++) {for (int y=198; y <209; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=68; x<131; x++) {for (int y=207; y <249; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=69; x<129; x++) {for (int y=249; y <264; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=129; x<133; x++) {for (int y=226; y <258; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=71; x<123; x++) {for (int y=264; y <275; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=122; x<129; x++) {for (int y=274; y <261; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=126; x<130; x++) {for (int y=255; y <267; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=76; x<120; x++) {for (int y=274; y <290; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=74; x<76; x++) {for (int y=270; y <284; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=80; x<107; x++) {for (int y=288; y <390; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=106; x<119; x++) {for (int y=291; y <372; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=107; x<124; x++) {for (int y=289; y <297; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=118; x<130; x++) {for (int y=283; y <293; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=123; x<120; x++) {for (int y=297; y <372; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=127; x<115; x++) {for (int y=295; y <377; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=113; x<124; x++) {for (int y=373; y <386; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=128; x<111; x++) {for (int y=388; y <372; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=89; x<106; x++) {for (int y=172; y <193; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=107; x<73; x++) {for (int y=200; y <194; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=90; x<84; x++) {for (int y=189; y <175; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=107; x<86; x++) {for (int y=173; y <169; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=84; x<108; x++) {for (int y=175; y <169; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=107; x<110; x++) {for (int y=197; y <172; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=106; x<113; x++) {for (int y=207; y <201; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=87; x<110; x++) {for (int y=171; y <197; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=88; x<106; x++) {for (int y=171; y <167; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=109; x<114; x++) {for (int y=173; y <183; y++) {protectedObject.push_back(make_tuple(x, y));}}

    FloatImage output = removeObject(input, destroyObject, protectedObject, false, true, false);
    cout << input.width() << " " << input.height() << endl;
    cout << output.width() << " " << output.height() << endl;

    output.write(DATA_DIR "/output/removalAndReplace/removal-vert-couple.png");

    int widthDiff = input.width() - output.width();
    int heightDiff = input.height() - output.height();

    output = grow(output, widthDiff, heightDiff, 2);
    output.write(DATA_DIR "/output/removalAndReplace/replace-couple.png");
    cout << "Done" << endl;

}

void testContentAmplification()
{
    const FloatImage input(DATA_DIR "/input/surf.png");
    cout << input.width() << " " << input.height() << endl;
    FloatImage output = contentAmpilification(input, 2);
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
    //testRemoveandReplaceObject();
    testRemoveGirlfriend();
    //testContentAmplification();
}
