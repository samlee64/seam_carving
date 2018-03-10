
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
    vector<int> seam = findVerticalSeamImage(input);
    FloatImage output = drawSeam(input, seam, false);
    output.write(DATA_DIR "/output/surf-vertical.png");
}

void testFindSeam()
{
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
    mid.write(DATA_DIR "/output/removed-castle-small.png");
}

void testGrow()
{
    const FloatImage input(DATA_DIR "/input/castle-large.png");
    cout << input.width() << " " << input.height() << endl;
    FloatImage output = grow(input, 60, 0, 2);
    cout << input.width() << " " << input.height() << endl;
    cout << output.width() << " " << output.height() << endl;
    output.write(DATA_DIR "/output/grow/castle-large-60-0-2.png");
}

void testRemoveObject()
{
    //vert only
    const FloatImage input(DATA_DIR "/input/castle-medium.png");
    vector<tuple<int, int>> destroyObject = {};
    vector<tuple<int, int>> protectedObject= {};

    FloatImage output = removeObject(input, destroyObject, protectedObject, false, true, false);
    cout << input.width() << " " << input.height() << endl;
    cout << output.width() << " " << output.height() << endl;
    output.write(DATA_DIR "/output/removal/test-castle-removal-minlow.png");
}


void testRemoveandReplaceObject()
{
    const FloatImage input(DATA_DIR "/input/beach.png");
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
    for(int x=145; x<190; x++) {for (int y=221; y <258; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=152; x<195; x++) {for (int y=257; y <286; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=150; x<160; x++) {for (int y=272; y <285; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=148; x<193; x++) {for (int y=284; y <301; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=155; x<191; x++) {for (int y=298; y <313; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=155; x<192; x++) {for (int y=312; y <327; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=158; x<193; x++) {for (int y=326; y <336; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=162; x<180; x++) {for (int y=335; y <383; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=193; x<182; x++) {for (int y=336; y <389; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=174; x<184; x++) {for (int y=332; y <392; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=190; x<180; x++) {for (int y=393; y <387; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=150; x<187; x++) {for (int y=222; y <216; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=153; x<173; x++) {for (int y=217; y <185; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=154; x<149; x++) {for (int y=214; y <189; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=150; x<169; x++) {for (int y=189; y <183; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=156; x<165; x++) {for (int y=186; y <180; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=161; x<172; x++) {for (int y=180; y <187; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=172; x<183; x++) {for (int y=186; y <216; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=184; x<192; x++) {for (int y=216; y <222; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=189; x<195; x++) {for (int y=221; y <259; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=193; x<200; x++) {for (int y=250; y <282; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=144; x<139; x++) {for (int y=221; y <254; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=139; x<152; x++) {for (int y=222; y <256; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=192; x<200; x++) {for (int y=275; y <292; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=196; x<203; x++) {for (int y=285; y <290; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=197; x<204; x++) {for (int y=284; y <287; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=199; x<203; x++) {for (int y=285; y <296; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=196; x<199; x++) {for (int y=286; y <297; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=197; x<205; x++) {for (int y=285; y <294; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=197; x<203; x++) {for (int y=291; y <327; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=203; x<197; x++) {for (int y=291; y <313; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=204; x<207; x++) {for (int y=292; y <322; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=198; x<206; x++) {for (int y=290; y <324; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=193; x<215; x++) {for (int y=312; y <343; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=193; x<208; x++) {for (int y=314; y <309; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=213; x<206; x++) {for (int y=314; y <308; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=204; x<208; x++) {for (int y=306; y <308; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=212; x<216; x++) {for (int y=324; y <342; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=198; x<215; x++) {for (int y=340; y <345; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=145; x<138; x++) {for (int y=249; y <260; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=148; x<138; x++) {for (int y=246; y <254; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=145; x<135; x++) {for (int y=244; y <261; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=142; x<136; x++) {for (int y=254; y <266; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=138; x<133; x++) {for (int y=256; y <265; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=139; x<133; x++) {for (int y=260; y <269; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=135; x<130; x++) {for (int y=260; y <266; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=138; x<132; x++) {for (int y=261; y <271; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=136; x<132; x++) {for (int y=265; y <273; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=134; x<129; x++) {for (int y=265; y <273; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=136; x<127; x++) {for (int y=263; y <276; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=132; x<129; x++) {for (int y=273; y <278; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=134; x<128; x++) {for (int y=274; y <279; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=132; x<125; x++) {for (int y=269; y <280; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=131; x<127; x++) {for (int y=275; y <280; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=129; x<126; x++) {for (int y=279; y <284; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=127; x<125; x++) {for (int y=278; y <282; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=129; x<125; x++) {for (int y=275; y <280; y++) {destroyObject.push_back(make_tuple(x, y));}}


    for(int x=70; x<128; x++) {for (int y=198; y <241; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=84; x<105; x++) {for (int y=172; y <204; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=86; x<108; x++) {for (int y=168; y <174; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=93; x<104; x++) {for (int y=171; y <167; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=94; x<105; x++) {for (int y=166; y <169; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=106; x<111; x++) {for (int y=171; y <184; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=111; x<105; x++) {for (int y=199; y <175; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=106; x<105; x++) {for (int y=184; y <174; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=72; x<121; x++) {for (int y=240; y <287; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=129; x<118; x++) {for (int y=293; y <238; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=125; x<131; x++) {for (int y=233; y <262; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=74; x<122; x++) {for (int y=286; y <314; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=122; x<124; x++) {for (int y=289; y <312; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=129; x<129; x++) {for (int y=282; y <292; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=112; x<85; x++) {for (int y=391; y <311; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=107; x<128; x++) {for (int y=309; y <387; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=126; x<97; x++) {for (int y=392; y <379; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=76; x<65; x++) {for (int y=260; y <209; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=66; x<58; x++) {for (int y=253; y <226; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=125; x<120; x++) {for (int y=300; y <285; y++) {protectedObject.push_back(make_tuple(x, y));}}

    for(int x=145; x<180; x++) {for (int y=181; y <215; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=165; x<194; x++) {for (int y=295; y <389; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=135; x<152; x++) {for (int y=242; y <256; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=134; x<141; x++) {for (int y=256; y <267; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=135; x<146; x++) {for (int y=257; y <264; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=132; x<137; x++) {for (int y=263; y <273; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=128; x<136; x++) {for (int y=266; y <279; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=130; x<143; x++) {for (int y=264; y <266; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=146; x<177; x++) {for (int y=211; y <225; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=162; x<185; x++) {for (int y=209; y <221; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=79; x<104; x++) {for (int y=310; y <388; y++) {protectedObject.push_back(make_tuple(x, y));}}
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
void testRemoveArtifacts()
{

}
void testContentAmplification()
{
    const FloatImage input(DATA_DIR "/input/surf.png");
    cout << input.width() << " " << input.height() << endl;
    FloatImage output = contentAmpilification(input, 2);
    output.write(DATA_DIR "/output/amplification/surf-2.png");
}

void testCastleAll()
{
//    //test energy functions
    const FloatImage input(DATA_DIR "/input/castle-medium.png");
    FloatImage output1 = gradientEnergy(input);
    output1.write(DATA_DIR "/output/full/castle/gradientEnergy.png");

    FloatImage output2 = dualGradientEnergy(input);
    output2.write(DATA_DIR "/output/full/castle/dualGradientEnergy.png");

    //test seam finding
    vector<int> seam1 = findHorizontalSeamImage(input);
    FloatImage output3 = drawSeam(input, seam1, true);
    output3.write(DATA_DIR "/output/full/castle/seam-horizontal.png");

    vector<int> seam2 = findVerticalSeamImage(input);
    FloatImage output4 = drawSeam(input, seam2, false);
    output4.write(DATA_DIR "/output/surf-vertical.png");

    //Remove some vertical seams
    FloatImage mid(input);
    for (int i = 0; i < 50; i++) {
        FloatImage eMap = energyMap(mid);
        const vector<int> seam = findVerticalSeamMap(eMap);

        char buffer[255];
        sprintf(buffer, DATA_DIR "/output/full/castle/seams/%d.png", i);
        drawSeam(mid, seam, false).write(buffer);

        mid = removeSeam(mid, seam, false);
    }
    mid.write(DATA_DIR "/output/full/castle/removed-vertical.png");


    //Grow
    FloatImage output5 = grow(input, 60, 60, 2);
    cout << "Grow results" << endl;
    cout << input.width() << " " << input.height() << endl;
    cout << output5.width() << " " << output5.height() << endl;
    output5.write(DATA_DIR "/output/full/castle/castle-medium-60-60-2.png");

    FloatImage output55 = grow(input, 60, 0, 2);
    cout << "Grow results" << endl;
    cout << input.width() << " " << input.height() << endl;
    cout << output55.width() << " " << output55.height() << endl;
    output55.write(DATA_DIR "/output/full/castle/castle-medium-60-0-2.png");

    FloatImage output6 = grow(input, 60, 60, 1);
    cout << "Grow results" << endl;
    cout << input.width() << " " << input.height() << endl;
    cout << output6.width() << " " << output6.height() << endl;
    output6.write(DATA_DIR "/output/full/castle/castle-medium-60-60-1.png");


    //remove object
    vector<tuple<int, int>> destroyObject = {};
    vector<tuple<int, int>> protectedObject = {};
    for(int x=367; x<423; x++) {for (int y=29; y <359; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=422; x<465; x++) {for (int y=64; y <355; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=462; x<577; x++) {for (int y=81; y <362; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=465; x<487; x++) {for (int y=78; y <85; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=509; x<569; x++) {for (int y=45; y <86; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=510; x<569; x++) {for (int y=37; y <48; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=362; x<582; x++) {for (int y=351; y <367; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=363; x<368; x++) {for (int y=349; y <339; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=367; x<362; x++) {for (int y=352; y <344; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=372; x<428; x++) {for (int y=23; y <33; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=376; x<421; x++) {for (int y=22; y <24; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=383; x<419; x++) {for (int y=19; y <25; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=428; x<418; x++) {for (int y=34; y <65; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=418; x<426; x++) {for (int y=27; y <37; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=512; x<568; x++) {for (int y=39; y <35; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=562; x<523; x++) {for (int y=36; y <30; y++) {destroyObject.push_back(make_tuple(x, y));}}

    FloatImage output7 = removeObject(input, destroyObject, protectedObject, false, true, false);
    output7.write(DATA_DIR "/output/full/castle/removed-castle.png");

    //protect and remove
    destroyObject = {};
    protectedObject = {};
    for(int x=425; x<464; x++) {for (int y=69; y <364; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=426; x<454; x++) {for (int y=64; y <73; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=464; x<574; x++) {for (int y=80; y <366; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=505; x<569; x++) {for (int y=32; y <82; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=519; x<561; x++) {for (int y=28; y <37; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=464; x<508; x++) {for (int y=76; y <82; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=570; x<583; x++) {for (int y=332; y <369; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=522; x<573; x++) {for (int y=369; y <359; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=465; x<436; x++) {for (int y=366; y <359; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=367; x<428; x++) {for (int y=22; y <364; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=381; x<419; x++) {for (int y=19; y <28; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=387; x<411; x++) {for (int y=16; y <21; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=370; x<357; x++) {for (int y=325; y <361; y++) {protectedObject.push_back(make_tuple(x, y));}}
    for(int x=359; x<369; x++) {for (int y=359; y <364; y++) {protectedObject.push_back(make_tuple(x, y));}}

    FloatImage output8 = removeObject(input, destroyObject, protectedObject, false, true, false);
    output8.write(DATA_DIR "/output/full/castle/removed-castle-protect-tower.png");

}

void testBeachAll()
{
    //both surf and beach photos

    //content amplification
    //const FloatImage surf(DATA_DIR "/input/surf.png");
    //FloatImage output1 = contentAmpilification(surf, 2);
    //output1.write(DATA_DIR "/output/full/beach/amplified-surf.png");

    //Object removal
    const FloatImage beach(DATA_DIR "/input/beach.png");
    vector<tuple<int, int>> destroyObject = {};
    vector<tuple<int, int>> protectedObject= {};
    for(int x=120; x<142; x++) {for (int y=46; y <106; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=123; x<134; x++) {for (int y=42; y <49; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=115; x<124; x++) {for (int y=63; y <83; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=104; x<125; x++) {for (int y=78; y <91; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=119; x<147; x++) {for (int y=105; y <113; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=140; x<148; x++) {for (int y=98; y <111; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=126; x<118; x++) {for (int y=110; y <121; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=123; x<116; x++) {for (int y=111; y <118; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=121; x<115; x++) {for (int y=112; y <127; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=121; x<115; x++) {for (int y=121; y <136; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=117; x<111; x++) {for (int y=125; y <138; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=113; x<110; x++) {for (int y=134; y <149; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=98; x<115; x++) {for (int y=144; y <156; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=112; x<124; x++) {for (int y=149; y <119; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=124; x<131; x++) {for (int y=126; y <112; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=140; x<151; x++) {for (int y=105; y <120; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=147; x<161; x++) {for (int y=116; y <123; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=144; x<160; x++) {for (int y=112; y <119; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=151; x<161; x++) {for (int y=117; y <127; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=151; x<165; x++) {for (int y=119; y <132; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=157; x<173; x++) {for (int y=123; y <133; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=168; x<177; x++) {for (int y=130; y <137; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=166; x<181; x++) {for (int y=126; y <141; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=174; x<184; x++) {for (int y=136; y <142; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=174; x<186; x++) {for (int y=138; y <144; y++) {destroyObject.push_back(make_tuple(x, y));}}


    for(int x=98; x<114; x++) {for (int y=145; y <154; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=109; x<118; x++) {for (int y=132; y <146; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=113; x<120; x++) {for (int y=122; y <134; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=116; x<125; x++) {for (int y=110; y <123; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=118; x<132; x++) {for (int y=104; y <116; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=123; x<128; x++) {for (int y=115; y <119; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=122; x<129; x++) {for (int y=119; y <125; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=118; x<122; x++) {for (int y=122; y <126; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=117; x<125; x++) {for (int y=124; y <131; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=117; x<120; x++) {for (int y=131; y <145; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=115; x<116; x++) {for (int y=143; y <148; y++) {destroyObject.push_back(make_tuple(x, y));}}
    FloatImage output2 = removeObject(beach, destroyObject, protectedObject, false, true, false);
    cout << beach.width() << " " << beach.height() << endl;
    cout << output2.width() << " " << output2.height() << endl;

    output2.write(DATA_DIR "/output/full/beach/removal-vert-girl-beach.png");

    int widthDiff = beach.width() - output2.width();
    int heightDiff = beach.height() - output2.height();

    output2 = grow(output2, widthDiff, heightDiff, 2);
    output2.write(DATA_DIR "/output/full/beach/replace-girl-beach.png");
}

void testRemoveBird()
{
    const FloatImage beach(DATA_DIR "/input/beach.png");
    vector<tuple<int, int>> destroyObject = {};
    vector<tuple<int, int>> protectedObject= {};

    for(int x=294; x<314; x++) {for (int y=199; y <235; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=306; x<329; x++) {for (int y=215; y <232; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=304; x<350; x++) {for (int y=227; y <236; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=355; x<345; x++) {for (int y=237; y <229; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=344; x<325; x++) {for (int y=231; y <220; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=300; x<324; x++) {for (int y=232; y <240; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=292; x<305; x++) {for (int y=240; y <245; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=318; x<331; x++) {for (int y=238; y <245; y++) {destroyObject.push_back(make_tuple(x, y));}}

    for(int x=323; x<334; x++) {for (int y=215; y <224; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=344; x<355; x++) {for (int y=228; y <236; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=332; x<343; x++) {for (int y=222; y <231; y++) {destroyObject.push_back(make_tuple(x, y));}}
    FloatImage output2 = removeObject(beach, destroyObject, protectedObject, false, true, false);
    cout << beach.width() << " " << beach.height() << endl;
    cout << output2.width() << " " << output2.height() << endl;

    output2.write(DATA_DIR "/output/full/beach/removal-vert-bird-beach.png");

    int widthDiff = beach.width() - output2.width();
    int heightDiff = beach.height() - output2.height();

    output2 = grow(output2, widthDiff, heightDiff, 2);
    output2.write(DATA_DIR "/output/full/beach/replace-bird-beach.png");
}

void testDolphinStretch()
{





}
void testRemoveGirlBird()
{

    const FloatImage beach(DATA_DIR "/input/beach.png");
    vector<tuple<int, int>> destroyObject = {};
    vector<tuple<int, int>> protectedObject= {};
    for(int x=120; x<142; x++) {for (int y=46; y <106; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=123; x<134; x++) {for (int y=42; y <49; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=115; x<124; x++) {for (int y=63; y <83; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=104; x<125; x++) {for (int y=78; y <91; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=119; x<147; x++) {for (int y=105; y <113; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=140; x<148; x++) {for (int y=98; y <111; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=126; x<118; x++) {for (int y=110; y <121; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=123; x<116; x++) {for (int y=111; y <118; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=121; x<115; x++) {for (int y=112; y <127; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=121; x<115; x++) {for (int y=121; y <136; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=117; x<111; x++) {for (int y=125; y <138; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=113; x<110; x++) {for (int y=134; y <149; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=98; x<115; x++) {for (int y=144; y <156; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=112; x<124; x++) {for (int y=149; y <119; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=124; x<131; x++) {for (int y=126; y <112; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=140; x<151; x++) {for (int y=105; y <120; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=147; x<161; x++) {for (int y=116; y <123; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=144; x<160; x++) {for (int y=112; y <119; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=151; x<161; x++) {for (int y=117; y <127; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=151; x<165; x++) {for (int y=119; y <132; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=157; x<173; x++) {for (int y=123; y <133; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=168; x<177; x++) {for (int y=130; y <137; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=166; x<181; x++) {for (int y=126; y <141; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=174; x<184; x++) {for (int y=136; y <142; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=174; x<186; x++) {for (int y=138; y <144; y++) {destroyObject.push_back(make_tuple(x, y));}}


    for(int x=98; x<114; x++) {for (int y=145; y <154; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=109; x<118; x++) {for (int y=132; y <146; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=113; x<120; x++) {for (int y=122; y <134; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=116; x<125; x++) {for (int y=110; y <123; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=118; x<132; x++) {for (int y=104; y <116; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=123; x<128; x++) {for (int y=115; y <119; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=122; x<129; x++) {for (int y=119; y <125; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=118; x<122; x++) {for (int y=122; y <126; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=117; x<125; x++) {for (int y=124; y <131; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=117; x<120; x++) {for (int y=131; y <145; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=115; x<116; x++) {for (int y=143; y <148; y++) {destroyObject.push_back(make_tuple(x, y));}}

    for(int x=294; x<314; x++) {for (int y=199; y <235; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=306; x<329; x++) {for (int y=215; y <232; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=304; x<350; x++) {for (int y=227; y <236; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=355; x<345; x++) {for (int y=237; y <229; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=344; x<325; x++) {for (int y=231; y <220; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=300; x<324; x++) {for (int y=232; y <240; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=292; x<305; x++) {for (int y=240; y <245; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=318; x<331; x++) {for (int y=238; y <245; y++) {destroyObject.push_back(make_tuple(x, y));}}

    for(int x=323; x<334; x++) {for (int y=215; y <224; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=344; x<355; x++) {for (int y=228; y <236; y++) {destroyObject.push_back(make_tuple(x, y));}}
    for(int x=332; x<343; x++) {for (int y=222; y <231; y++) {destroyObject.push_back(make_tuple(x, y));}}
    FloatImage output2 = removeObject(beach, destroyObject, protectedObject, false, true, false);
    cout << beach.width() << " " << beach.height() << endl;
    cout << output2.width() << " " << output2.height() << endl;

    output2.write(DATA_DIR "/output/full/beach/removal-vert-girl-bird-beach.png");

    int widthDiff = beach.width() - output2.width();
    int heightDiff = beach.height() - output2.height();

    output2 = grow(output2, widthDiff, heightDiff, 2);
    output2.write(DATA_DIR "/output/full/beach/replace-girl-bird-beach.png");


}



int main()
{
    testGrow();
    //testRemoveObject();
    //testRemoveandReplaceObject();
    //testRemoveGirlfriend();
    //testContentAmplification();
    //testCastleAll();
    //testBeachAll();
    //testRemoveGirlBird();
}
