/***************************************************************************************************
 * Object Detection Evaluator
 *
 * @author Matthew Munson, Sana Suse
 * @date 5/25/21
 *
 * This is a tool for determining how well the object detection methods are working. The input is a 
 * video of gameplay which is passed into the detection method. 
 *
 * Configuration / Assumptions:
 *
 * - DETECTION_TYPES acts as a dictionary of which detection methods can be used
 * - DETECTION_METHOD is the method to be used to detect the hero in the frames of the video
 * - PATH_TO_VIDEO is the path to the video file you're breaking down
 * - EXPECTED_HERO is the expected hero that appears in the video file
 * - MATCH_METHOD is a specific parameter for the template matching detection method. It denotes 
 *   which of the 6 template matching methods to be used
 * - The object detection method called in processFrame() should:
 *   * Accept an integer of the expected hero based on the video file
 *   * Return an integer of whether the expected hero was found
 *
 **************************************************************************************************/

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include "../Tutorials/Template_Matching/template_matching.h"
#include "./CSV/csv_wrapper.h"

using namespace std;
using namespace cv;

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;

static const string DETECTION_TYPES[] = { "Template matching", "Cascade classifier", "Edge matching" };
static const int DETECTION_METHOD = 0;

//static const string PATH_TO_VIDEO = "Detection_Algorithm/Data/Video/Lucio/walking1.mp4";
//static const int EXPECTED_HERO = 1; // 0- Mercy, 1- Lucio

// Template matching specific parameters
//static const int MATCH_METHOD = 5;

static const Mat template_mercy = imread("Detection_Algorithm/Data/Templates/Mercy.png");
static const Mat template_lucio = imread("Detection_Algorithm/Data/Templates/Lucio.png");

void displayStats(const int& correct, const int& total);

int processFrame(Mat& frame);

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
int main() {
    vector<vector<string>> videoFiles;
    csv_wrapper::readFromCSV("Detection_Algorithm/Data/Video/video_paths.csv", videoFiles);

    VideoCapture capture;

    vector<vector<string>> output;
    vector<string> row;

    for (int i = 0; i < videoFiles.size(); i++) {
        string videoPath = videoFiles[i][0];
        string expectedHero = videoPath.substr(0, videoPath.find("/", 0));
        cout << expectedHero << endl;

        capture = VideoCapture(videoPath);

        if (!capture.isOpened()) {
            cout << "Could not open capture! Either the provided path is invalid, or your build \n" <<
                "of openCV does not support MPEG." << endl;

            return -1;
        }

        Mat frame;

        int totalFrameCount = 0;
        int correctCount = 0;
        int reps = DETECTION_METHOD == 0 ? 6 : 1;
        
        for (int i = 0; i < reps; i++) {
            row.clear();

            while (true) {

                capture >> frame;

                if (frame.empty()) {
                    break;
                }

                correctCount += processFrame(frame, expectedHero, i);

                waitKey(1);

                //imshow("VideoDisplay", frame);

                totalFrameCount++;
            }

            displayStats(correctCount, totalFrameCount);

            row.push_back(videoPath);
            row.push_back(DETECTION_TYPES[DETECTION_METHOD]);
            row.push_back(to_string(i));
            row.push_back(to_string(correctCount));
            row.push_back(to_string(totalFrameCount));
            output.push_back(row);
        }
    }

    return 0;
}

/***************************************************************************************************
 * Process Frame
 *
 * This is where you should call your image processing code for a single frame.
 * The method should take in an integer of which hero is expected.
 *
 * //TODO: Add your code here
 *
 **************************************************************************************************/
int processFrame(Mat& frame, string expectedHero, int matchMethod) {
    if (DETECTION_METHOD == 0) {
        Mat templ_array[2];
        templ_array[0] = template_mercy;
        templ_array[1] = template_lucio;
        return identifyHero(frame, templ_array, matchMethod, expectedHero);
    }
    return 0;
}

void displayStats(const int& correct, const int& total) {
    double effectiveness = correct / total * 100.0;
    if (DETECTION_METHOD == 0) {
        cout << DETECTION_METHOD << " using method #" << MATCH_METHOD << " had: " << endl;
    }
    else {
        cout << DETECTION_METHOD << "had: " << endl;
    }
   
    cout << "Correct: " << correct << ", out of Total: " << total << endl;
    cout << "Effectiveness: " << effectiveness << "% success rate." << endl;
}