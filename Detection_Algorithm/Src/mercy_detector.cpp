/***************************************************************************************************
 * Mercy Detector
 *
 * @authors Matthew Munson,
 * @date June 6th 2021
 *
 * This program is our project's final deliverable. It takes in a video of Overwatch gameplay and
 * displays it to the user while outputting the detected hero and weapon action to the screen. A
 * frames per second reading is also provided to gauge the performance of the object detection
 * algorithms.
 *
 * _________________________________________________________________________________________________
 * Assumptions:
 *
 * • VIDEO_PATH is a valid path to a .mp4 video
 * _________________________________________________________________________________________________
 * Implementation Details:
 *
 **************************************************************************************************/

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include "Overwatch_Constants/overwatchConstants.h"

using namespace std;
using namespace cv;

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::system_clock;

//--------------------------------------------------------------------------------------------------
// Configuration Variables

static const string VIDEO_PATH = "Detection_Algorithm/Data/Evaluation/Gameplay/gameplay1.mp4";

static const bool USE_TEMPLATE_MATCHING = false;
static const bool USE_CASCADE_CLASSIFIER = false;

static const bool VERBOSE_CONSOLE_LOG = false;

//--------------------------------------------------------------------------------------------------

/***************************************************************************************************
 * Process Video
 **************************************************************************************************/
void processVideo(VideoCapture& capture);

/***************************************************************************************************
 * Add Text Output
 **************************************************************************************************/
void addTextOutput(Mat& frame, double fps, OWConst::Heroes hero, OWConst::WeaponActions action);

/***************************************************************************************************
 * Get Time
 **************************************************************************************************/
long getTime();

/***************************************************************************************************
 * Detect Hero
 **************************************************************************************************/
OWConst::Heroes detectHero(Mat& frame);

/***************************************************************************************************
 * Detect Action
 **************************************************************************************************/
OWConst::WeaponActions detectAction(Mat& frame);

/***************************************************************************************************
 * Main Function
 *
 * @pre
 * @post
 **************************************************************************************************/
int main()
{
    VideoCapture capture(VIDEO_PATH);

    if(!capture.isOpened())
    {
        cout << "Could not open capture! Either the provided path is invalid, or your build \n" <<
             "of openCV does not support MPEG." << endl;

        return -1;
    }

    cout << "Successfully opened " << VIDEO_PATH << endl;

    processVideo(capture);
}

/***************************************************************************************************
 * Process Video
 *
 * @param capture
 *
 * @pre
 * @post
 **************************************************************************************************/
void processVideo(VideoCapture& capture)
{
    Mat frame;
    int keyCode; // The value of the key pressed during waitKey()

    long lastTime = getTime(); // The time measured at the end of the last frame
    long elapsedTime;
    double fps;

    OWConst::Heroes identifiedHero;
    OWConst::WeaponActions identifiedAction;

    string displayWindow = "VideoDisplay";
    namedWindow(displayWindow);

    cout << "Starting video loop" << endl;

    while(true)
    {
        capture >> frame;

        if(frame.empty())
        {
            break;
        }

        keyCode = waitKey(1);

        if(keyCode == 27) // Escape key stops the loop
        {
            break;
        }

        elapsedTime = getTime() - lastTime;
        fps = (double) 1000 / ((double) elapsedTime);

        identifiedHero = detectHero(frame);
        identifiedAction = detectAction(frame);

        addTextOutput(frame, fps, identifiedHero, identifiedAction);

        imshow(displayWindow, frame);

        lastTime = getTime();
    }

    cout << "Video Loop Stopped" << endl;
}

/***************************************************************************************************
 * Detect Hero
 *
 * @param frame
 *
 * @pre
 * @post
 *
 * @return
 **************************************************************************************************/
OWConst::Heroes detectHero(Mat& frame)
{
    OWConst::Heroes detectedHero = OWConst::No_Hero;

    if(USE_TEMPLATE_MATCHING)
    {
        detectedHero = OWConst::No_Hero; //TODO: Put Template Matching Hero Detect Here

        if(VERBOSE_CONSOLE_LOG)
        {
            cout << "Template Matching Detected Hero: "
            << OWConst::getHeroString(detectedHero) << endl;
        }
    }
    if(USE_CASCADE_CLASSIFIER)
    {
        detectedHero = OWConst::No_Hero; //TODO: Put Cascade Classifier Hero Detect Here

        if(VERBOSE_CONSOLE_LOG)
        {
            cout << "Cascade Classifier Detected Hero: "
            << OWConst::getHeroString(detectedHero) << endl;
        }
    }

    return detectedHero;
}

/***************************************************************************************************
 * Detect Action
 *
 * @param frame
 *
 * @pre
 * @post
 *
 * @return
 **************************************************************************************************/
OWConst::WeaponActions detectAction(Mat& frame)
{
    OWConst::WeaponActions detectedAction = OWConst::No_Action;

    if(USE_TEMPLATE_MATCHING)
    {
        detectedAction = OWConst::No_Action; //TODO: Put Template Matching Action Detect Here

        if(VERBOSE_CONSOLE_LOG)
        {
            cout << "Template Matching Detected Action: "
                 << OWConst::getWeaponActionString(detectedAction) << endl;
        }
    }
    if(USE_CASCADE_CLASSIFIER)
    {
        detectedAction = OWConst::No_Action; //TODO: Put Cascade Classifier Action Detect Here

        if(VERBOSE_CONSOLE_LOG)
        {
            cout << "Cascade Classifier Detected Action: "
                 << OWConst::getWeaponActionString(detectedAction) << endl;
        }
    }

    return detectedAction;
}

/***************************************************************************************************
 * Add Text Output
 *
 * @param frame
 * @param fps
 * @param hero
 * @param action
 *
 * @pre
 * @post
 *
 **************************************************************************************************/
void addTextOutput(Mat& frame, double fps, OWConst::Heroes hero, OWConst::WeaponActions action)
{
    string fpsDisplay = "FPS: " + to_string((int) fps);

    string heroDisplay = "IDENTIFIED HERO: " + OWConst::getHeroString(hero);

    string actionDisplay = "IDENTIFIED ACTION: " + OWConst::getWeaponActionString(action);

    Scalar textColor(0, 255,0);
    cv::HersheyFonts font = FONT_HERSHEY_COMPLEX;
    float scale = 2.0;
    int thickness = 3;

    putText(frame, fpsDisplay, Point(100, 100), font, scale, textColor, thickness);

    putText(frame, heroDisplay, Point(100,180), font, scale, textColor, thickness);

    putText(frame, actionDisplay,Point(100,260), font, scale, textColor, thickness);
}

/***************************************************************************************************
 * Get Time
 *
 * Wrapper for the rather ugly time retrieval call
 *
 * Source: https://www.delftstack.com/howto/cpp/how-to-get-time-in-milliseconds-cpp/
 *
 * @pre
 * @post
 *
 * @return
 **************************************************************************************************/
long getTime()
{
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}