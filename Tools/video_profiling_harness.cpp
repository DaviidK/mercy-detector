/***************************************************************************************************
 * Video Profiling Harness
 *
 * @author Matthew Munson
 * @date 5/12/21
 *
 * This is a tool for measuring how long your processing takes relative to a single frame being
 * played in a video. While the system works for getting a general idea of performance, there's a
 * lot left to do.
 *
 * Structural TODOs:
 * // TODO: Convert this into a C++ Class
 * // TODO: Create file output to save test results
 * // TODO: Make the processing function into a delegate function that can be assigned
 * // TODO: Complete Documentation
 *
 * Video Performance Improvements:
 * // TODO: Research if OpenCV is capping the framerate on its own
 * // TODO: Research if caching video in RAM will speed things up
 *
 * Configuration / Assumptions:
 *
 * - PATH_TO_VIDEO is the path to the video file you're breaking down
 *
 **************************************************************************************************/

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include "../Tutorials/Template_Matching/template_matching.h"

using namespace std;
using namespace cv;

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;

static const string PATH_TO_VIDEO = "Detection_Algorithm/Data/Video/Lucio/walking1.mp4";
static const string DETECTION_TYPES[] = { "Template-Matching", "Cascade-Classifier", "Edge-Matching" };
static const int DETECTION_METHOD = 0; // Change this to change the detection method.

// Template Matching variables 
static Mat TEMPLATES[2];
static const string TEMPL_FILE_PREFIX = "Detection_Algorithm/Data/Templates/";
static const vector<OWConst::Heroes> TM_ACCEPTED_HEROES = { OWConst::Mercy, OWConst::Lucio };
static const int MATCH_METHOD = 0; // Change this to change the temp matching method.
static const bool USE_MASK = false; // Change this to change whether the matching method will take
                                    // a mask. This can only be set to true for 0 and 3 match methods.

void doProcessing(long& start_processing, long& end_processing, Mat& frame);

long getTime();

void displayStats(const long& totalTime, const long& processingTime);

void processFrame(Mat& frame);

void tempMatchingSetup();

/***************************************************************************************************
 * Main Function
 *
 * Opens the provided video file and begins the video profiling loop.
 *
 * Video Profiling Loop:
 * - Record the time at the beginning of the loop
 * - Read in the next frame (file access may take time here)
 * - Does user-defined processing on the frame
 * - Waits for minimum amount of time and displays the frame
 * - Records the time at the end of the loop
 *
 **************************************************************************************************/
int main()
{
    VideoCapture capture = VideoCapture(PATH_TO_VIDEO);

    if(!capture.isOpened())
    {
        cout << "Could not open capture! Either the provided path is invalid, or your build \n" <<
             "of openCV does not support MPEG." << endl;

        return -1;
    }

    Mat frame;
    namedWindow("VideoDisplay");

    int frameCount = 0;
    long averageDiff = 0;

    long start, start_processing, end_processing, end;
    long processingTime, totalTime;

    if (DETECTION_METHOD == 0) {
        tempMatchingSetup();
    }
    
    while(true)
    {
        start = getTime();

        capture >> frame;

        if(frame.empty())
        {
            break;
        }

        doProcessing(start_processing, end_processing, frame);

        waitKey(1);

        //imshow("VideoDisplay", frame);

        end = getTime();

        processingTime = end_processing - start_processing;
        totalTime = end - start;

        if(frameCount % 10 == 0)
        {
            displayStats(totalTime, processingTime);
        }

        averageDiff += (end - start);

        frameCount++;
    }

    cout << "Average diff: " << averageDiff / frameCount << endl;
    cout << "Average FPS: " << (double) 1 / (averageDiff * 1000) << endl;

    return 0;
}

/***************************************************************************************************
 * Process Frame
 *
 * This is where you should call your image processing code for a single frame.
 *
 * //TODO: Add your code here
 *
 **************************************************************************************************/
void processFrame(Mat& frame)
{
    if (DETECTION_METHOD == 0) {
        identifyHero(frame, MATCH_METHOD, USE_MASK);
    }
}

/***************************************************************************************************
 * Do Processing
 *
 * Records the processing start time, calls the processFrame function, and records the processing
 * end time.
 *
 **************************************************************************************************/
void doProcessing(long& start_processing, long& end_processing, Mat& frame)
{
    start_processing = getTime();

    processFrame(frame);

    end_processing = getTime();
}

/***************************************************************************************************
 * Get Time
 *
 * Wrapper for the rather ugly time retrieval call
 *
 * Source: https://www.delftstack.com/howto/cpp/how-to-get-time-in-milliseconds-cpp/
 *
 **************************************************************************************************/
long getTime()
{
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

/***************************************************************************************************
 * Display Stats
 *
 * Displays frame rate information and how much time the processing is taking up. All time values
 * are in milliseconds
 *
 **************************************************************************************************/
void displayStats(const long& totalTime, const long& processingTime)
{
    cout << "Diff: " << totalTime;
    printf(" | FPS: %.1f", (double) 1 / totalTime * 1000);
    cout << " | Processing Diff: " << processingTime;
    printf(" | Percentage: %.1f", ((double) processingTime / totalTime) * 100);
    cout << endl;
}

/***************************************************************************************************
 * Temp Matching Setup
 *
 * This method is a specific set up helper method for the template matching method.
 * It loads in the template images into a global vector of Mats.
 *
 **************************************************************************************************/
void tempMatchingSetup() {
    for (int i = 0; i < TM_ACCEPTED_HEROES.size(); i++) {
        string filename = TEMPL_FILE_PREFIX + OWConst::getHeroString(TM_ACCEPTED_HEROES[i]) + ".png";
        TEMPLATES[i] = imread(filename);
    }
}
