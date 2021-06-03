/***************************************************************************************************
 * Ground Truth Assessment Tool
 *
 * This tool allows a video to be analyzed frame-by-frame and for the Overwatch Hero and weapon
 * action to be recorded. It outputs a comma seperated file that can be used to identify and
 * extract desired frames, or assess the accuracy of an object detection method by comparing
 * against the ground truth.
 *
 * This program takes a single video path as input. If the PATH_TO_METADATA string is empty, the
 * metadata file will be created from scratch. Otherwise the metadata file will be opened at the
 * provided path. The metadata file will be saved to PATH_TO_METADATA_SAVE. This can be the same
 * as PATH_TO_METADATA to overwrite changes.
 *
 * Once a MetaFile has been created, the user will be able to step through each frame in the video.
 * _________________________________________________________________________________________________
 * PROGRAM CONTROLS
 *
 * SPACE BAR           : Overwrite hero and action, advance one frame
 * RIGHT ARROW         : Advance one frame without overwriting settings
 * NUMBER KEYS         : Set the weapon action based on WEAPON_ACTION1 through WEAPON_ACTION5
 * SHIFT + NUMBER KEYS : Set the hero based on HERO1 through HERO5
 *
 * -------------------------------------------------------------------------------------------------
 *
 * The user's selected hero and weapon action are retained between frames as changes are
 * relatively infrequent. Frames can be quickly advanced or overwritten by holding down the
 * space bar or right arrow key.
 *
 * At this time, frames may only be advanced forward. If a mistake is made, the file must be
 * reopened and cycled to the frame to be changed. It is suggested to do a rough "first-pass" and
 * note the frames where mistake are made, then fix these in a second pass.
 **************************************************************************************************/

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include "Meta_File/meta_file.h"
#include <iostream>
#include <filesystem>

using namespace std;
using namespace cv;

//__________________________________________________________________________________________________
// Configuration Variables

const string PATH_TO_VIDEO = "Detection_Algorithm/Data/Video/Mercy/Glock/idle.mp4";
const string PATH_TO_METADATA = ""; // Leave this blank to create new
const string PATH_TO_METADATA_SAVE = "Detection_Algorithm/Data/Output/";

// These are the weapon actions assigned by the number keys 1-5
// If a hero other than Mercy is being analyzed, these may need to be changed
static const OWConst::WeaponActions WEAPON_ACTION1 = OWConst::Firing;
static const OWConst::WeaponActions WEAPON_ACTION2 = OWConst::Melee;
static const OWConst::WeaponActions WEAPON_ACTION3 = OWConst::Healing;
static const OWConst::WeaponActions WEAPON_ACTION4 = OWConst::Damage_Boosting;
static const OWConst::WeaponActions WEAPON_ACTION5 = OWConst::No_Action;

// These are the heroes assigned the SHIFT-Number keys 1-5
// Change these heroes to whichever you expect to see in the video
static const OWConst::Heroes HERO1 = OWConst::Mercy;
static const OWConst::Heroes HERO2 = OWConst::Lucio;
static const OWConst::Heroes HERO3 = OWConst::Soldier76;
static const OWConst::Heroes HERO4 = OWConst::Orisa;
static const OWConst::Heroes HERO5 = OWConst::No_Hero;

//--------------------------------------------------------------------------------------------------

string removePath(string path);

int getFrameCount(const string& filename);

void checkNumericalInput(int value, int frameIndex,
                         OWConst::Heroes& currentHero, OWConst::WeaponActions& currentAction);

void addTextOutput(Mat& frame, MetaFile metaFile, int frameIndex,
                   OWConst::WeaponActions currentAction, OWConst::Heroes currentHero);

void processVideo(const string& filePath, MetaFile& metaFile);

/***************************************************************************************************
 * Main Function
 *
 * - Searches for metadata files that match the provided video file in the MetaData directory
 *     - If a match is found, the file is opened
 *     - If no match is found:
 *         - The frames in the provided video file are counted
 *         - A new meta file is created
 * - Each frame is displayed to the user, allowing them to set the hero and action
 * - The metadata file is saved to the MetaData directory
 *
 **************************************************************************************************/
int main()
{
    string filePath = PATH_TO_VIDEO;
    string filename = removePath(filePath);
    MetaFile metaFile;

    if(!PATH_TO_METADATA.empty())
    {
        metaFile = MetaFile(PATH_TO_METADATA);

        cout << "Opened " << PATH_TO_METADATA << endl;
    }
    else
    {
        cout << "No corresponding meta file found, creating from scratch" << endl;

        int frameCount = getFrameCount(filePath);

        if(frameCount < 0)
        {
            cout << "Frame count must be greater than 0" << endl;
            return -1;
        }

         metaFile = MetaFile(frameCount);

        cout << "Meta file created" << endl;
    }

    processVideo(filePath, metaFile);

    cout << "Frame loop complete, saving metafile" << endl;

    string metaFileName = PATH_TO_METADATA_SAVE + "/" + filename + ".meta";

    metaFile.save(metaFileName);

    cout << "Meta File Saved at " << metaFileName << endl;

    return 0;
}


/***************************************************************************************************
 * Process Video
 *
 * Displays each video frame to the user and allows them to modify the hero and action corresponding
 * to the frame.
 *
 * - The outer while loop goes through each frame in the video file
 * - The inner while loop waits for user input on each frame
 *
 **************************************************************************************************/
void processVideo(const string& filePath, MetaFile& metaFile)
{
    VideoCapture capture(filePath);
    Mat frame;
    Mat displayFrame;

    bool escapeKeyPressed = false;
    int frameIndex = 0;

    // The hero and action that will be saved to the metadata when the space bar is pressed
    OWConst::Heroes currentHero = OWConst::No_Hero;
    OWConst::WeaponActions currentAction = OWConst::No_Action;

    string displayWindow = "VideoDisplay";
    namedWindow(displayWindow);

    while(true)
    {
        capture >> frame;

        if(frame.empty())
            break;

        frame.copyTo(displayFrame);
        addTextOutput(displayFrame, metaFile, frameIndex, currentAction, currentHero);
        imshow(displayWindow, displayFrame);

        // Wait for key input from the user
        while(true)
        {
            int value = waitKey();

            if(value == 32) // Spacebar progresses to the next frame
            {
                // Set to selected values
                metaFile.setHero(frameIndex, currentHero);
                metaFile.setWeaponAction(frameIndex, currentAction);
                break;
            }
            if(value == 3) // Right arrow skips frame with no changes
            {
                break;
            }
            if(value == 27) // Escape quits the outer frame loop
            {
                escapeKeyPressed = true;
                break;
            }

            checkNumericalInput(value, frameIndex, currentHero, currentAction);

            // Keep the text updated while user input is received
            frame.copyTo(displayFrame);
            addTextOutput(displayFrame, metaFile, frameIndex, currentAction, currentHero);
            imshow(displayWindow, displayFrame);
        }

        if(escapeKeyPressed)
        {
            break;
        }

        frameIndex++;
    }
}

/***************************************************************************************************
 * Get Frame Count
 *
 * Counts the frames in a video by looping through it and incrementing a counter.
 *
 **************************************************************************************************/
int getFrameCount(const string& filename)
{
    VideoCapture capture = VideoCapture(filename);

    if(!capture.isOpened())
    {
        cout << "Could not open capture! Either the provided path is invalid, or your build \n" <<
             "of openCV does not support MPEG." << endl;

        return -1;
    }

    Mat frame;
    int frameCount = 0;

    cout << "Starting Frame Count" << endl;

    while(true)
    {
        capture >> frame;

        if(frame.empty())
            break;

        frameCount++;
    }

    cout << "Frame Count is: " << frameCount << endl;

    return frameCount;
}

// Check the number keys and assign weapon action / hero
/***************************************************************************************************
 * Check Numerical Input
 *
 * Checks the number keys and updates currentAction and currentHero accordingly.
 *
 * Number Key : OpenCV Key Value
 * One           : 49
 * Two           : 50
 * Three         : 51
 * Four          : 52
 * Five          : 53
 *
 * Shift + One   : 33
 * Shift + Two   : 64 <- No idea why this happens
 * Shift + Three : 35
 * Shift + Four  : 36
 * Shift + Five  : 37
 *
 **************************************************************************************************/
void checkNumericalInput(int value, int frameIndex, OWConst::Heroes& currentHero, OWConst::WeaponActions& currentAction)
{
    // Assign Weapon Actions with the number keys

    switch(value)
    {
        // Set Actions with Number Keys
        case 49:
            currentAction = WEAPON_ACTION1;
            break;
        case 50:
            currentAction = WEAPON_ACTION2;
            break;
        case 51:
            currentAction = WEAPON_ACTION3;
            break;
        case 52:
            currentAction = WEAPON_ACTION4;
            break;
        case 53:
            currentAction = WEAPON_ACTION5;
            break;

            // Set Heroes with SHIFT-Number Keys
        case 33:
            currentHero = HERO1;
            break;
        case 64: // Shift-2 is different for some reason
            currentHero = HERO2;
            break;
        case 35:
            currentHero = HERO3;
            break;
        case 36:
            currentHero = HERO4;
            break;
        case 37:
            currentHero = HERO5;
            break;
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
void addTextOutput(Mat& frame, MetaFile metaFile, int frameIndex, OWConst::WeaponActions currentAction, OWConst::Heroes currentHero)
{
    string frameCountDisplay = "FRAME COUNT: " + to_string(frameIndex);

    string heroDisplay = "CURRENT/SET HERO: " +
                         OWConst::getHeroString(metaFile.getHero(frameIndex)) +
                         " : " +
                         OWConst::getHeroString(currentHero);

    string actionDisplay = "CURRENT/SET ACTION: " +
                           OWConst::getWeaponActionString(metaFile.getWeaponAction(frameIndex)) +
                           " : " +
                           OWConst::getWeaponActionString(currentAction);

    Scalar textColor(0, 255,0);
    cv::HersheyFonts font = FONT_HERSHEY_COMPLEX;
    float scale = 2.0;
    int thickness = 3;

    putText(frame, frameCountDisplay,Point(100,100), font, scale, textColor, thickness);

    putText(frame, heroDisplay, Point(100,180), font, scale, textColor, thickness);

    putText(frame, actionDisplay,Point(100,260), font, scale, textColor, thickness);
}

/***************************************************************************************************
 * Remove Path
 *
 * Extracts a filename from a given path. Removes both the preceding directories and the file
 * extension.
 *
 * Source: https://stackoverflow.com/questions/8520560/get-a-file-name-from-a-path
 *
 * @pre: path is a valid string
 * @post: The path before the filename is removed as well as the file extension.
 *
 * @return A copy of the provided filename with no path or extension included.
 **************************************************************************************************/
string removePath(string path)
{
    // Remove directory if present.
    // Do this before extension removal in case directory has a period character.
    const size_t last_slash_idx = path.find_last_of("\\/");
    if (string::npos != last_slash_idx)
    {
        path.erase(0, last_slash_idx + 1);
    }

    // Remove extension if present.
    const size_t period_idx = path.rfind('.');
    if (string::npos != period_idx)
    {
        path.erase(period_idx);
    }

    return path;
}


