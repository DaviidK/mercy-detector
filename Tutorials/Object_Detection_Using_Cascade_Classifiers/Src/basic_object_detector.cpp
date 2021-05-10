// ----------------------------------basic_object_detector.cpp------------------------------------
// Author: David Kang, with code sourced from Ana Huamán via opencv.org
// Last modified: 05/09/21
// -----------------------------------------------------------------------------------------------
// Purpose: This class modifies the source code found in the OpenCV Object Detection Tutorial
// (https://docs.opencv.org/3.4/db/d28/tutorial_cascade_classifier.html) to use a custom trained 
// classifier. 
// -----------------------------------------------------------------------------------------------
// Assumptions:
//     - 


#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>

using namespace std;
using namespace cv;

void detectAndDisplay(CascadeClassifier& mercy_cascade, Mat frame);

int main(int argc, const char** argv) {
    CascadeClassifier mercy_cascade;
    String test_images_file_paths;

    CommandLineParser parser(argc, argv,
        "{mercy_cascade|../Data/Input/mercy_staff_classifier.xml|Path to mercy cascade classifier.}"
        );
    parser.about("\nThis program demonstrates using the cv::CascadeClassifier class to detect"
                 " (Mery's staff) in screenshots of Overatch gameplay.\n"
                 "You can use Haar or LBP features.\n\n");
    parser.printMessage();

    //-- 1. Load the cascade for mercy's staff
    String mercy_cascade_name = samples::findFile(parser.get<String>("face_cascade"));
    if (!mercy_cascade.load(mercy_cascade_name)) {
        cout << "--(!)Error loading mercy cascade\n";
        return -1;
    }

    





    int camera_device = parser.get<int>("camera");
    VideoCapture capture;

    //-- 2. Read the video stream
    capture.open(camera_device);
    if (!capture.isOpened()) {
        cout << "--(!)Error opening video capture\n";
        return -1;
    }

    Mat frame;
    while (capture.read(frame)) {
        if (frame.empty()) {
            cout << "--(!) No captured frame -- Break!\n";
            break;
        }

        //-- 3. Apply the classifier to the frame
        detectAndDisplay(mercy_cascade, frame);
        if (waitKey(10) == 27) {
            break; // escape
        }
    }
    return 0;
}

void detectAndDisplay(CascadeClassifier& mercy_classifier,  Mat frame) {
    Mat frame_gray;
    cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);

    //-- Detect Caduceus Staff
    std::vector<Rect> staffOccurences;
    mercy_classifier.detectMultiScale(frame_gray, staffOccurences);

    for (size_t i = 0; i < staffOccurences.size(); i++) {
        Point center(staffOccurences[i].x + staffOccurences[i].width / 2, staffOccurences[i].y + staffOccurences[i].height / 2);
        ellipse(frame, center, Size(staffOccurences[i].width / 2, staffOccurences[i].height / 2), 0, 0, 360, Scalar(255, 0, 255), 4);
    }

    //-- Show what you got
    imshow("Capture - Face detection", frame);
}