/***************************************************************************************************
 * Facial Recognition Test
 *
 * This program demonstrates the usage of cascade classifiers to identify the face and eyes from
 * a webcam video feed. This uses two trained cascade classifier models saved as XML files.
 *
 * Depending on how many webcams your computer has connected you may need to change the camera
 * device number in the CommandLineParser arguments.
 *
 * Press the escape key to exit the program.
 *
 **************************************************************************************************/

#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>

using namespace std;
using namespace cv;

void detectAndDisplay(CascadeClassifier& face_cascade, CascadeClassifier& eyes_cascade, Mat frame);

/***************************************************************************************************
 * Main Function
 *
 * Program entry point. Loads the cascade classifiers from the provided file and instantiates the
 * video stream. Begins an infinite while loop that retrieves a frame from the webcam each loop
 * and passes it into detectAndDisplay.
 *
 **************************************************************************************************/
int main(int argc, const char** argv)
{
    CascadeClassifier face_cascade;
    CascadeClassifier eyes_cascade;

    CommandLineParser parser(argc, argv,
    "{help h||}"
    "{face_cascade|Detection_Algorithm/Data/HaarCascades/haarcascade_frontalface_alt2.xml|Path to face cascade.}"
    "{eyes_cascade|Detection_Algorithm/Data/HaarCascades/haarcascade_eye_tree_eyeglasses.xml|Path to eyes cascade.}"
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
    if(!eyes_cascade.load(eyes_cascade_name))
    {
        cout << "Error loading eyes cascade" << endl;

        return -1;
    }

    int camera_device = parser.get<int>("camera");

    VideoCapture capture;

    // Reading from video stream:
    capture.open(camera_device);

    if(!capture.isOpened())
    {
        cout << "Could not start video capture" << endl;
        return -1;
    }

    Mat frame;

    while(capture.read(frame))
    {
        if(frame.empty())
        {
            cout << "No captured frame" << endl;
            break;
        }

        detectAndDisplay(face_cascade, eyes_cascade, frame);

        // Check for escape key
        if(waitKey(10) == 27)
        {
            break;
        }

    }

    return 0;
}

/***************************************************************************************************
 * Detect and Display
 *
 * Uses the face and eyes cascade classifier to first detect faces and eyes in a frame. Eyes are
 * only detected if a face can first be found. Circles are drawn at the detected position of the
 * face and eyes, and the frame is displayed to an output window.
 *
 **************************************************************************************************/
void detectAndDisplay(CascadeClassifier& face_cascade, CascadeClassifier& eyes_cascade, Mat frame)
{
    Mat frame_grey;
    cvtColor(frame, frame_grey, COLOR_BGR2GRAY);
    equalizeHist(frame_grey, frame_grey);

    //Face Detection
    vector<Rect> faces;
    face_cascade.detectMultiScale(frame_grey, faces);

    for ( size_t i = 0; i < faces.size(); i++ )
    {
        Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
        ellipse( frame, center, Size( faces[i].width/2, faces[i].height/2 ), 0, 0, 360, Scalar( 255, 0, 255 ), 4 );
        Mat faceROI = frame_grey( faces[i] );
        //-- In each face, detect eyes
        std::vector<Rect> eyes;
        eyes_cascade.detectMultiScale( faceROI, eyes );
        for ( size_t j = 0; j < eyes.size(); j++ )
        {
            Point eye_center( faces[i].x + eyes[j].x + eyes[j].width/2, faces[i].y + eyes[j].y + eyes[j].height/2 );
            int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
            circle( frame, eye_center, radius, Scalar( 255, 0, 0 ), 4 );
        }
    }


    imshow("Capture - Face Detection", frame);
}
