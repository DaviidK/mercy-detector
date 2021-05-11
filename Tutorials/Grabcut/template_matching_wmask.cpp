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
const char* template_name2 = "Detection_Algorithm/Data/Static_Test_Im/soldier.jpg";
const char* mask_name = "Detection_Algorithm/Data/Static_Test_Im/mask_grabcut_mercy.jpg";

void matchTemplates(Mat src, Mat templ1, Mat templ2, Mat mask, int method);

int main1(int argc, char** argv) {
	Mat srcImg = imread(image_name, IMREAD_COLOR);
	Mat templImg1 = imread(template_name1, IMREAD_COLOR);
	Mat templImg2 = imread(template_name2, IMREAD_COLOR);
	Mat maskImg = imread(mask_name, IMREAD_COLOR);

	if (srcImg.empty() || templImg1.empty() || templImg2.empty() || maskImg.empty()) {
		cout << "Can't read one of the images" << endl;
		return EXIT_FAILURE;
	}

	namedWindow(image_window, WINDOW_AUTOSIZE);
	matchTemplates(srcImg, templImg1, templImg2, maskImg, 0);

}
// 0 and 3
void matchTemplates(Mat src, Mat templ1, Mat templ2, Mat mask, int method) {
	Mat result1; Mat result2;
	Mat displayImg;
	src.copyTo(displayImg);

	result1.create(src.rows, src.cols, COLOR_BGR2GRAY);
	result2.create(src.rows, src.cols, COLOR_BGR2GRAY);

	matchTemplate(src, templ1, result1, method, mask);
	//matchTemplate(src, templ2, result2, method, mask);

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
	/*
	// Do the same but for result2
	minMaxLoc(result2, &minVal1, &maxVal1, &minLoc1, &maxLoc1, Mat());

	if (method == TM_SQDIFF) {
		cout << "Mercy Min value: " << minVal1 << endl;
		matchLoc1 = minLoc1;
	}
	else {
		cout << "Mercy Max value: " << maxVal1 << endl;
		matchLoc1 = maxLoc1;
	}

	rectangle(displayImg, matchLoc1, Point(matchLoc1.x + templ1.cols, matchLoc1.y + templ1.rows), Scalar::all(0), 2, 8, 0);
	imshow(image_window, displayImg);*/
}