// ----------------------------------basic_object_detector.cpp------------------------------------
// Author: David Kang, with sample code sourced from Ana Huam�n via opencv.org
// Last modified: 05/12/21
// -----------------------------------------------------------------------------------------------
// Purpose: This class modifies the source code found in the OpenCV Object Detection Tutorial
// (https://docs.opencv.org/3.4/db/d28/tutorial_cascade_classifier.html) to use a custom trained 
// classifier. 
// -----------------------------------------------------------------------------------------------
// Assumptions:
//   - A trained cascade classifier will exist, and will be referenced by MERCY_CLASSIFIER_LOCATION
//   - A text file containing relative filepaths to each image to be detected is stored in 
//     TEST_IMAGES_FILE_PATHS 



#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <fstream>
#include <iostream>

using namespace std;
using namespace cv;

static const string MERCY_CLASSIFIER_LOCATION =
      "Tutorials/Object_Detection_Using_Cascade_Classifiers/Input/mercy_staff_classifier.xml";

// TODO: Run a bash/batch command within this program to write all filepaths to
//       this text file, allowing for new files to be added without updating
//       the text file itself.
static const string TEST_IMAGES_FILE_PATHS = 
      "Tutorials/Object_Detection_Using_Cascade_Classifiers/Input/test_image_file_paths.txt";

void detectAndDisplay(CascadeClassifier& mercy_cascade, Mat image);

/**
  * main
    Will initiate the program by loading the classifier and test images to be used. It will then 
    iterate through each image, performing object detection based on the trained classifier. Each 
    image will be presented to the user, and an ellipse will be drawn around all instances of the
    object that are found. A keypress will be required to move through each image.

  * @pre:  The global constant MERCY_CLASSIFIER_LOCATION references an existing path and contains a 
           trained cascade classifier. The constant TEST_IMAGES_FILE_PATHS references a valid 
           text file which contains relative filepaths to eachtest image on separate lines.
  * @post: A new window will be displayed to the user containing the test images, with ellipses 
           drawn around every object instance. Keypresses will move forward through test images.

  * @param: None
  * @return[int]: Exit code for program, with 0 representing success.
*/
int main() {
    CascadeClassifier mercy_cascade;
    String test_images_file_paths;

    // Load the cascade for mercy's staff
    if (!mercy_cascade.load(MERCY_CLASSIFIER_LOCATION)) {
        cout << "--(!)Error loading mercy cascade\n";
        return -1;
    }

    ifstream file(TEST_IMAGES_FILE_PATHS);
    string line;
    
    // Iterate through all lines in the test image file paths text file, loading a new test image
    // for each line in the text file
    while (getline(file, line)) {
        Mat screenshot = imread(line);

        if (screenshot.empty()) {
            cout << "--(!) No captured image -- Break!\n";
            break;
        }

        // Perform object detection using the trained classifier
        detectAndDisplay(mercy_cascade, screenshot);
        waitKey();
    }

    return 0;
}

/**
  * findAndDrawMatches
    Method which will create a brute force matcher and use it to find matches between two sets of
    descriptors. Once found, these matches will be drawn onto an output image.

  * @pre:  Input contains two valid sets of descriptors, sourced from two images.
  * @post: The matches from both descriptors will be drawn onto the passed parameter Mat output.
  
  * @param[mercy_classifier]: Trained cascade classifier for a specific object
  * @param[image]: Single image to be tested, will have object detection performed across the 
                   entire image
  * @return: None
*/
void detectAndDisplay(CascadeClassifier& mercy_classifier,  Mat image) {
    Mat frame_gray;
    cvtColor(image, frame_gray, COLOR_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);

    // Detect if Mery is the hero being played in the screenshot 
    std::vector<Rect> staffOccurences;
    mercy_classifier.detectMultiScale(frame_gray, staffOccurences);

    for (size_t i = 0; i < staffOccurences.size(); i++) {
        Point center(staffOccurences[i].x + staffOccurences[i].width / 2, staffOccurences[i].y + staffOccurences[i].height / 2);
        ellipse(image, center, Size(staffOccurences[i].width / 2, staffOccurences[i].height / 2), 0, 0, 360, Scalar(0, 255, 0), 6);
    }

    // Display the image to the user, with the drawn ellipse if an object was detected
    imshow("Capture - Face detection", image);
}