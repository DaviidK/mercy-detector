/*
 * This program aims to simply print 
 */

#include "template_matching.h"

const int NUM_HEROES = 2;
const string heroes[NUM_HEROES] = { "Mercy", "Lucio" };
const char* templ_file_prefix = "Detection_Algorithm/Data/Templates/";

int identifyHero(Mat& frame, Mat* template_array, int match_method, string expected_hero) {
	if (match_method < 0 || match_method > 5) {
		cout << "The match method was invalid." << endl;
		return -1;
	}
	
	Mat templ; string filename;
	Mat result; Mat result_templ;
	int counter = 0; int result_hero; 
	double tempScore; Point matchLoc;

	// Crop the source image so it only looks at the lower right quadrant. 
	Rect cropRect = Rect(frame.cols / 2, frame.rows / 2, frame.cols / 2, frame.rows / 2);
	Mat cropped = frame(cropRect);

	while (counter < NUM_HEROES) {
		filename = templ_file_prefix + heroes[counter] ;
		templ = template_array[counter];

		matchTemplate(cropped, templ, result, match_method);

		double minVal; double maxVal; Point minLoc; Point maxLoc;
		minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
		//normalize(result, result, 0, 1, NORM_L2, -1, Mat());

		if (match_method == TM_SQDIFF || match_method == TM_SQDIFF_NORMED) {
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

	return expected_hero == heroes[result_hero];
}

// Test method to attempt identifying a sample frame 
/*
int main(int argc, char** argv) {
    const char* sample_mercy_frame = "Detection_Algorithm/Data/Extracted_Frames/Mercy/Wand/Idle_Light_Test/frame_280.png";
    const char* sample_lucio_frame = "Detection_Algorithm/Data/Extracted_Frames/Lucio/Walking1/frame_466.png";
	Mat frame = imread(sample_mercy_frame, IMREAD_COLOR);
	identifyHero(frame);
	waitKey(0);
	frame = imread(sample_lucio_frame, IMREAD_COLOR);
	identifyHero(frame);
	waitKey(0);
}*/