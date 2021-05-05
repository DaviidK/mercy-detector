//
// Created by matthew on 5/5/21.
//

#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>

using namespace std;
using namespace cv;

void detectAndDisplay(Mat frame);

CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;

int main(int argc, const char** argv)
{
    CommandLineParser parser(argc, argv,
    "{help h||}"
    "{face_cascade|data/haarcascades/haarcascades_frontalface_alt.xml|Path to face cascade.}"
    "{eyes_cascade|data/haarcascades/haarcascade_eye_tree_eyeglasses.xml|Path to eyes cascade.}"
    "{camera|0|Camera device number.}");

    parser.about("\n This program demonstrates how to use the CascadeClassifier to detect the"
                 "face and eyes in a video stream");

    parser.printMessage();

    String face_cascade_name = samples::findFile(parser.get<String>("face_cascade"));
    String eyes_cascade_name = samples::findFile(parser.get<String>("eyes_cascade"));

    if(!face_cascade.load(face_cascade_name))
    {
        cout << "Error loading face cascade" << endl;
        return -1;
    }




}
