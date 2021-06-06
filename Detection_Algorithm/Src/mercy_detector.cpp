/***************************************************************************************************
 * Mercy Detector
 *
 *
 **************************************************************************************************/

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include "Overwatch_Constants/overwatchConstants.h"


using namespace std;
using namespace cv;

using std::chrono::duration_cast;

//--------------------------------------------------------------------------------------------------
// Configuration Variables

static const string VIDEO_PATH = "Detection_Algorithm/Data/Video/Mercy_Eval/Mercy_Eval1.mp4";

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
 * Main Function
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
 **************************************************************************************************/
void processVideo(VideoCapture& capture)
{
    cout << "Opened Video" << endl;
    Mat frame;
    int keyValue;

    long lastTime = getTime();
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

        keyValue = waitKey(1);

        if(keyValue == 27) // Escape quits the outer frame loop
        {
            break;
        }

        elapsedTime = getTime() - lastTime;
        fps = (double) 1000 / (elapsedTime);

        identifiedHero = OWConst::Mercy;
        identifiedAction = OWConst::Holding_Staff;

        addTextOutput(frame, fps, identifiedHero, identifiedAction);

        imshow(displayWindow, frame);

        lastTime = getTime();
    }
}

/***************************************************************************************************
 * Add Text Output
 *
 * Adds text to a provided frame. Displays the current frame number. The current hero and action
 * is the value stored in the file, which will be No_Hero and No_Action by default. The set hero
 * and action are the values that will be overwritten if the user presses the space bar.
 *
 **************************************************************************************************/
void addTextOutput(Mat& frame, double fps, OWConst::Heroes hero, OWConst::WeaponActions action)
{
    string fpsDisplay = "FPS: " + to_string(fps);

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
 **************************************************************************************************/
long getTime()
{
    return duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}