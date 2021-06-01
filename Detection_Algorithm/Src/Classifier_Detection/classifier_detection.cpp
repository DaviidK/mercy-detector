// ----------------------------------classifier_detection.cpp--------------------------------------
// Author: David Kang
// Last modified: 05/31/21
// ------------------------------------------------------------------------------------------------
// Purpose: 
// ------------------------------------------------------------------------------------------------
// Assumptions:
//   - 



#include "opencv2/objdetect.hpp"
#include "opencv2/imgproc.hpp"

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

*/
void identifyHero(CascadeClassifier& classifier, Mat image) {
    Mat frame_gray;
    cvtColor(image, frame_gray, COLOR_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);

    // Detect if a hero can be detected in the screenshot 
    std::vector<Rect> staffOccurences;
    classifier.detectMultiScale(frame_gray, staffOccurences);

    for (size_t i = 0; i < staffOccurences.size(); i++) {
        Point center(staffOccurences[i].x + staffOccurences[i].width / 2, staffOccurences[i].y + staffOccurences[i].height / 2);
        ellipse(image, center, Size(staffOccurences[i].width / 2, staffOccurences[i].height / 2), 0, 0, 360, Scalar(0, 255, 0), 6);
    }

    // Display the image to the user, with the drawn ellipse if an object was detected
    imshow("Capture - Face detection", image);
}

const int NUM_HEROES = 2;
const string heroes[NUM_HEROES] = { "Mercy", "Lucio" };
const char* templ_file_prefix = "Detection_Algorithm/Data/Templates/";
const int MATCH_METHOD = 3; // can be any value between 0 and 5.

int identifyHeroEval(Mat& frame, int expected_hero, Mat template_mercy, Mat template_lucio) {
	Mat templ; string filename;
	Mat result; Mat result_templ;
	int counter = 0; int result_hero;
	double tempScore; Point matchLoc;

	// Crop the source image so it only looks at the lower right quadrant. 
	Rect cropRect = Rect(frame.cols / 2, frame.rows / 2, frame.cols / 2, frame.rows / 2);
	Mat cropped = frame(cropRect);

	while (counter < NUM_HEROES) {
		filename = templ_file_prefix + heroes[counter];

		if (counter == 0) {
			templ = template_mercy;
		}
		else {
			templ = template_lucio;
		}
		matchTemplate(cropped, templ, result, MATCH_METHOD);

		double minVal; double maxVal; Point minLoc; Point maxLoc;
		minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
		//normalize(result, result, 0, 1, NORM_L2, -1, Mat());

		if (MATCH_METHOD == TM_SQDIFF || MATCH_METHOD == TM_SQDIFF_NORMED) {
			// If first run or score is less than temp score
			if (counter == 0 || minVal < tempScore) {
				tempScore = minVal;
				matchLoc = minLoc;
				result_hero = counter;
				result_templ = templ;
			}
		}
		else {
			if (counter == 0 || maxVal > tempScore) {
				tempScore = maxVal;
				matchLoc = maxLoc;
				result_hero = counter;
				result_templ = templ;
			}
		}
		counter++;
	}

	Mat display_img;
	frame.copyTo(display_img);
	Point modifiedPt = Point(matchLoc.x + cropped.cols, matchLoc.y + cropped.rows);
	rectangle(display_img, modifiedPt, Point(modifiedPt.x + result_templ.cols, modifiedPt.y + result_templ.rows), Scalar::all(0), 2, 8, 0);
	imshow("result", display_img);
	cout << heroes[result_hero] << endl;
	return expected_hero == result_hero;
}