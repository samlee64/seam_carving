#include "seam.h"
#include "morphing.h"
#include "a6.h"
#include "energy.h"
#include <iomanip>
#include <string>
#include <map>
#include <sys/stat.h>


using namespace std;

enum Routine {
    defaultRoutine,
    growRoutine,
    contentAmplificationRoutine,
    removeObjectRoutine,
};
// Map to associate the strings with the enum values
static std::map<std::string, Routine> mapStringRoutines;

static void Initialize();

void runGrow(const string inputImagePath, const string outputPath, const int addWidth, const int addHeight, const int numSteps, const bool showIntermediate)
{
    const FloatImage input(inputImagePath);

    cout << "Input Width, Height: " << input.width() << ", " << input.height() << endl;
    FloatImage output = grow(input, addWidth, addHeight, numSteps, outputPath); //100 vertical seams, 0 horizontal, in 4 steps
    cout << "Output Width, Height: " << output.width() << ", " << output.height() << endl;
}

int growArgCheck(int argc, char *argv[])
{
    if (argc != 8)
    {
        cout <<  "There is an improper number of arguments. Needed 6, received " << argc << "\n" << endl;
        cout << "Needs arguments <grow> <inputImagePath> <outputPath> <addWidth> <addHeight> <numSteps> <showIntermediate>\n" << endl;
        throw "Improper number of arguments";
    }

    const string inputImagePath(argv[2]);
    const string outputPath(argv[3]);
    const int addWidth = atoi(argv[4]);
    const int addHeight = atoi(argv[5]);
    const int numSteps = atoi(argv[6]);
    const bool showIntermediate = 0 == strcmp(argv[7], "true");

    if (addWidth == 0 && addHeight == 0)
    {
        throw "0 addWidth, 0 addHeight specified. Not doing anything. \n";
    }

    cout << "Running grow with parameters \n" << endl;
    cout << "inputImagePath: " << inputImagePath << endl;
    cout << "outputPath: " << outputPath << endl;
    cout << "addWidth: " << addWidth << endl;
    cout << "addHeight: " << addHeight << endl;
    cout << "numSteps: " << numSteps << endl;
    cout << "showIntermediate: " << showIntermediate << endl;

    runGrow(inputImagePath, outputPath, addWidth, addHeight, numSteps, showIntermediate);

    return 0;
}


void runContentAmplification(const string inputImagePath, const string outputPath, const float factor)
{
    const FloatImage input(inputImagePath);
    const FloatImage output = contentAmplification(input, factor, outputPath);
}

int contentAmplificationArgCheck(int argc, char *argv[])
{
    if (argc != 6)
    {
        cout << "There is an improper number of arguments." << argc << endl;
        cout << "Needs arguments <contentAmplification> <inputImagePath> <outputPath> <factor> <showIntermediate>\n" << endl;

        throw "There is an improper number of arguments";
    }

    const string inputImagePath(argv[2]);
    const string outputPath(argv[3]);
    const float factor = stof(argv[4]);
    const bool showIntermediate = 0 == strcmp(argv[5], "true");

    if (factor == 0)
    {
        throw "0 factor specified. Not doing anything";
    }

    cout << "Running contentAmplification with parameters \n" << endl;
    cout << "inputImagePath: " << inputImagePath << endl;
    cout << "outputPath: " << outputPath << endl;
    cout << "factor: " << factor << endl;
    cout << "showIntermediate: " << showIntermediate << endl;

    runContentAmplification(inputImagePath, outputPath, factor);

    return 0;
}

void runRemoveObject(const string inputImagePath, const string outputPath, const string destroyMaskPath, const string protectMaskPath, const bool lockRatio,  const bool onlyVertical, const bool onlyHorizontal, const bool showIntermediate)
{
   const FloatImage input(inputImagePath);

    FloatImage destroyRegions(destroyMaskPath);
    FloatImage protectRegions(protectMaskPath);

    FloatImage output = removeObject2(input, destroyRegions, protectRegions, lockRatio, onlyVertical, onlyHorizontal, outputPath);
}

int removeObjectArgCheck(int argc, char *argv[])
{
    if (argc != 10)
    {
        cout << "There is an improper number of arguments." << argc << endl;
        cout << "Needs arguments <removeObject> <inputImagePath> <outputPath> <destroyMaskPath> <protectMaskPath> <lockRatio> <onlyHorizontal> <onlyVertial> <showIntermediate>\n" << endl;
        throw "There is an improper number of arugments";
    }

    const string inputImagePath(argv[2]);
    const string outputPath(argv[3]);
    const string destroyMaskPath(argv[4]);
    const string protectMaskPath(argv[5]);
    const bool lockRatio = 0 == strcmp(argv[6], "true");
    const bool onlyHorizontal = 0 == strcmp(argv[7], "true");
    const bool onlyVertical = 0 == strcmp(argv[8], "true");
    const bool showIntermediate = 0 == strcmp(argv[9], "true");

    cout << "Running contentAmplification with parameters \n" << endl;
    cout << "inputImagePath: " << inputImagePath << endl;
    cout << "outputPath: " << outputPath << endl;
    cout << "destroyMaskPath: " << destroyMaskPath << endl;
    cout << "protectMaskPath: " << protectMaskPath << endl;
    cout << "lockRatio: " << lockRatio << endl;
    cout << "onlyVertical: " << onlyVertical << endl;
    cout << "onlyHorizontal: " << onlyHorizontal << endl;
    cout << "showIntermediate: " << showIntermediate << endl;

    runRemoveObject(inputImagePath, outputPath, destroyMaskPath, protectMaskPath, lockRatio, onlyVertical, onlyHorizontal, showIntermediate);
    return 0;
}

int main(int argc, char *argv[])
{
    Initialize();
    const string routine(argv[1]);

    switch (mapStringRoutines[routine]) {
        case growRoutine:
            cout << "Calling grow routine" << endl;
            growArgCheck(argc, argv);
            break;
        case contentAmplificationRoutine:
            cout << "Calling content amplification routine" << endl;
            contentAmplificationArgCheck(argc, argv);
            break;
        case removeObjectRoutine:
            cout << "Calling remove object routine" << endl;
            removeObjectArgCheck(argc, argv);
            break;
        default:
            cout << "default" << endl;
    }

    std::cout << "end" << "\n" ;
}

void Initialize()
{
    mapStringRoutines["grow"] = growRoutine;
    mapStringRoutines["contentAmplification"] = contentAmplificationRoutine;
    mapStringRoutines["removeObject"] = removeObjectRoutine;
}
