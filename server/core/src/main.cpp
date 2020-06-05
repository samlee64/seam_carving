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

void runGrow(const string imageName, const int addWidth, const int addHeight, const int numSteps, const bool showIntermediate)
{
    string inputImagePath = DATA_DIR "/input/" + imageName + ".png";
    const FloatImage input(inputImagePath);

    string outputPath = DATA_DIR "/output/" + imageName + "/";
    cout << outputPath << endl;
    if (mkdir(outputPath.c_str(), 0777) == -1)
       cerr << "Error: " << strerror(errno) << endl;
    else
        cout << "Directory created" << endl;

    //check


    cout << "Input Width, Height: " << input.width() << ", " << input.height() << endl;
    FloatImage output = grow(input, addWidth, addHeight, numSteps, outputPath); //100 vertical seams, 0 horizontal, in 4 steps
    cout << "Output Width, Height: " << output.width() << ", " << output.height() << endl;
}

int growArgCheck(int argc, char *argv[])
{
    if (argc != 7)
    {
        cout <<  "There is an improper number of arguments. Needed 6, received " << argc << "\n" << endl;
        cout << "Needs arguments <routine> <imageName> <addWidth> <addHeight> <numSteps> <showIntermediate>\n" << endl;
        return 1;
    }

    const string imageName(argv[2]);
    const int addWidth = atoi(argv[3]);
    const int addHeight = atoi(argv[4]);
    const int numSteps = atoi(argv[5]);

    const bool showIntermediate = 0 == strcmp(argv[6], "true");

    if (addWidth == 0 && addHeight == 0 )
    {
        cout << "0 addWidth, 0 addHeight specified. Not doing anything. \n" << endl;
        return 0;
    }

    cout << "Running grow with parameters \n" << endl;
    cout << "imageName: " << imageName << endl;
    cout << "addWidth: " << addWidth << endl;
    cout << "addHeight: " << addHeight << endl;
    cout << "numSteps: " << numSteps << endl;
    cout << "showIntermediate: " << showIntermediate << endl;

    runGrow(imageName, addWidth, addHeight, numSteps, showIntermediate);

    return 0;
}


void runContentAmplification(const string imageName, const int factor)
{
    string inputImagePath = DATA_DIR "/input/" + imageName + ".png";

    const FloatImage input(inputImagePath);

    string outputPath = DATA_DIR "/output/" + imageName + "/";
    cout << outputPath << endl;

    const FloatImage output = contentAmplification(input, factor, outputPath);
}

int contentAmplificationArgCheck(int argc, char *argv[])
{
    int i;
    for (i=0; i<argc-1; i++)
    {
        cout << " " << argv[i];
    }

    cout << "\n" << endl;
    if (argc != 5)
    {
        cout << "There is an improper number of arguments." << argc << endl;
        cout << "Needs arguments <routine> <imageName> <factor> <showIntermediate>\n" << endl;
        cout << "Provided: ";
        int i;
        for (i=0; i<argc-1; i++)
        {
            cout << " " << argv[i];
        }
        cout << "\n" << endl;


        return 1;
    }

    const string imageName(argv[2]);
    const int factor = atoi(argv[3]);

    const bool showIntermediate = 0 == strcmp(argv[4], "true");
    if (factor == 0)
    {
        cout << "0 factor specified. Not doing anything" << endl;
        return 0;
    }

    cout << "Running grow with parameters \n" << endl;
    cout << "imageName: " << imageName << endl;
    cout << "factor: " << factor << endl;
    cout << "showIntermediate: " << showIntermediate << endl;

    runContentAmplification(imageName, factor);

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
            cout << "Calling content amplification" << endl;
            contentAmplificationArgCheck(argc, argv);
            break;

        case removeObjectRoutine:
            cout << "Calling removeObjectRoutine" << endl;
            break;
        default:
            cout << "default" << endl;
    }


    //grow imageName width height debug
    //some arg validation then runGrow
    //void runGrow(const string imageName, const int addWidth, const int addHeight, const bool debug)


//    testGrow();


    std::cout << "end" << "\n" ;
}

void Initialize()
{
    mapStringRoutines["grow"] = growRoutine;
    mapStringRoutines["contentAmplification"] = contentAmplificationRoutine;
    mapStringRoutines["removeObject"] = removeObjectRoutine;
}
