/*
 * This program deals with template matching where only methods that accept masks are
 * being used. These are TM_SQDIFF and TM_CCORR_NORMED. Basically attempts to see if
 * masks make the template matching methodology more accurate or not.
 */

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

const char* image_window = "Source Image";
const char* image_name = "Detection_Algorithm/Data/Static_Test_Im/busy.jpg";
const char* template_name1 = "Detection_Algorithm/Data/Static_Test_Im/template_grabcut_mercy.jpg";
const char* mask_name = "Detection_Algorithm/Data/Static_Test_Im/mask_grabcut_mercy.jpg";
// For cv::MatchTemplate(), only methods 0 and 3 work.
// So for this constant, only include 0 (TM_SQDIFF) or 3 (TM_CCORR_NORMED).
const int MATCH_METHOD = 0;

void matchTemplates(Mat src, Mat templ1, Mat mask, int method);

int main(int argc, char** argv) {
	Mat srcImg = imread(image_name, IMREAD_COLOR);
	Mat templImg1 = imread(template_name1, IMREAD_COLOR);
	Mat maskImg = imread(mask_name, IMREAD_COLOR);

	if (srcImg.empty() || templImg1.empty() || maskImg.empty()) {
		cout << "Can't read one of the images" << endl;
		return EXIT_FAILURE;
	}

	namedWindow(image_window, WINDOW_AUTOSIZE);
	
	matchTemplates(srcImg, templImg1, maskImg, MATCH_METHOD);
}

void matchTemplates(Mat src, Mat templ1, Mat mask, int method) {
	Mat result1;
	Mat displayImg;
	src.copyTo(displayImg);

	result1.create(src.rows, src.cols, COLOR_BGR2GRAY);

	matchTemplate(src, templ1, result1, method, mask);

	double minVal1; double maxVal1; Point minLoc1; Point maxLoc1;
	Point matchLoc1;
	minMaxLoc(result1, &minVal1, &maxVal1, &minLoc1, &maxLoc1, Mat());

	if (method == TM_SQDIFF) {
		cout << "Mercy Min value: " << minVal1 << endl;
		matchLoc1 = minLoc1;
	}
	else {
		cout << "Mercy Max value: " << maxVal1 << endl;
		matchLoc1 = maxLoc1;
	}

	rectangle(displayImg, matchLoc1, Point(matchLoc1.x + templ1.cols, matchLoc1.y + templ1.rows), Scalar::all(0), 2, 8, 0);
	imshow(image_window, displayImg);
	waitKey(0);
}
