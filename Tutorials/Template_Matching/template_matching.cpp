#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

const int NUM_HEROES = 2;
const string heroes[NUM_HEROES] = { "Mercy", "Lucio" };
const char* templ_file_prefix = "Detection_Algorithm/Data/Templates/";
const char* sample_mercy_frame = "Detection_Algorithm/Data/Extracted_Frames/Mercy/Wand/Idle/frame_1.png";
const char* sample_lucio_frame = "Detection_Algorithm/Data/Extracted_Frames/Lucio/Idle/frame_311.png";
const int MATCH_METHOD = 0; // can be any value between 0 and 6.

void identifyHero(Mat& frame) {
	Mat templ; string filename;
	Mat result; Mat result_templ;
	int counter = 0; int result_hero; 
	double tempScore; Point matchLoc;

	while (counter < NUM_HEROES) {
		filename = templ_file_prefix + heroes[counter] ;
		templ = imread(filename + ".png", IMREAD_COLOR);
		matchTemplate(frame, templ, result, MATCH_METHOD);

		double minVal; double maxVal; Point minLoc; Point maxLoc;
		minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

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
	rectangle(display_img, matchLoc, Point(matchLoc.x + result_templ.cols, matchLoc.y + result_templ.rows), Scalar::all(0), 2, 8, 0);
	imshow("result", display_img);
	cout << heroes[result_hero];
}

int main(int argc, char** argv) {
	Mat frame = imread(sample_mercy_frame, IMREAD_COLOR);
	identifyHero(frame);
	waitKey(0);
}