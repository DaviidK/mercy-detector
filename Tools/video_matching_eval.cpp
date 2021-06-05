/***************************************************************************************************
 * Object Detection Evaluator
 *
 * @author Matthew Munson, Sana Suse, David Kang
 * @date 06/02/21
 *
 * This is a tool for determining how well the object detection methods are working. 
 * The input is a video of gameplay which is passed into the detection method. 
 * The output is a CSV file of the results.
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
#include "Detection_Algorithm/Src/Overwatch_Constants/overwatchConstants.h"
#include "Tutorials/Template_Matching/template_matching.h"
#include "Detection_Algorithm/Src/classifier_detector/classifier_detector.h"
#include "Tools/CSV/csv_wrapper.h"

using namespace std;
using namespace cv;

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;

static const string VIDEO_FILE_PATHS = "Detection_Algorithm/Data/Video/video_paths.csv";
static const string VIDEO_FILE_PREFIX = "Detection_Algorithm/Data/Video/";
static const string DETECTION_TYPES[] = { "Template-Matching", "Cascade-Classifier", "Edge-Matching" };
static const int DETECTION_METHOD = 1;

// Template matching specific parameters
static const int NUM_MATCHING_METHODS = 8;

void processVideoTemplateMatching(VideoCapture capture, 
								  OWConst::Heroes expectedHero, 
	                              vector<vector<string>> &output,
								  string filePath);

void processVideoCascadeClassifier(VideoCapture capture,
							       OWConst::Heroes expectedHero,
								   vector<vector<string>>& output,
								   string filepath);

void displayStats(const int& correct, const int& total);

string getDateTime();

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
	csv_wrapper::readFromCSV(VIDEO_FILE_PATHS, videoFiles);

	VideoCapture capture;

	vector<vector<string>> output;

	for (int i = 0; i < videoFiles.size(); i++) {
		string shortPath = videoFiles[i][0];
		string videoPath = VIDEO_FILE_PREFIX + shortPath;
		string hero_name = shortPath.substr(0, shortPath.find("/", 0));
		OWConst::Heroes expectedHero = OWConst::getHero(hero_name);

		capture = VideoCapture(videoPath);

		if (!capture.isOpened()) {
			cout << "Could not open capture! Either the provided path is invalid, or your build \n" <<
				"of openCV does not support MPEG." << endl;

			return -1;
		}
		
		cout << "Currently on video " << i + 1 << " of " << videoFiles.size() << endl;

		if (DETECTION_METHOD == 0) {
			tempMatchingSetup();
			processVideoTemplateMatching(capture, expectedHero, output, shortPath);
		}
		else if (DETECTION_METHOD == 1) {
			processVideoCascadeClassifier(capture, expectedHero, output, shortPath);
		}
		else if (DETECTION_METHOD == 2) {
			// TODO: Edge Matching frame processing method here
			// Pass in capture, expectedHero and output.
		}
		else {
			cout << "The given detection method is not one of the 3 accepted for this program." << endl;
		}
	}

	string dateTime = getDateTime();

	string output_file_name = "Tools/Eval_Results/" + DETECTION_TYPES[DETECTION_METHOD] + "-" + dateTime + ".csv";
	csv_wrapper::saveToCSV(output_file_name, output);

	return 0;
}



/***************************************************************************************************
 * Process Frame for Template Matching 
 *
 * This method processes a given VideoCapture to detect heroes in each frame using template matching.
 * It stores the resulting correct counts and total number of frames into a csv file for easy data 
 * processing.
 * 
 **************************************************************************************************/
void processVideoTemplateMatching(VideoCapture capture, OWConst::Heroes expectedHero, 
								  vector<vector<string>> &output, string filepath) {
	vector<string> row; 

	Mat frame;
	int correctCount[NUM_MATCHING_METHODS];
	int totalFrameCount = 0;

	for (int i = 0; i < NUM_MATCHING_METHODS; i++) {
		correctCount[i] = 0;
	}

	cout << "Progress (* per 50 frame): " << endl;
	int match_method; bool use_mask;
	while (true) {

		capture >> frame;

		if (frame.empty()) {
			break;
		}

		if (totalFrameCount % 50 == 0) {
			cout << "*";
		}
		
		for (int i = 0; i < NUM_MATCHING_METHODS; i++) {
			if (i == 6) {
				match_method = 0;
				use_mask = true;
			}
			else if (i == 7) {
				match_method = 3;
				use_mask = true;
			}
			else {
				match_method = i;
				use_mask = false;
			}
			correctCount[i] += evalIdentifyHero(frame, match_method, expectedHero, use_mask);
		}

		totalFrameCount++;
	}
	cout << endl;

	for (int i = 0; i < NUM_MATCHING_METHODS; i++) {
		row.clear();

		row.push_back(OWConst::getHeroString(expectedHero));
		row.push_back(filepath);
		row.push_back(DETECTION_TYPES[DETECTION_METHOD]);
		row.push_back(to_string(i));
		row.push_back(to_string(correctCount[i]));
		row.push_back(to_string(totalFrameCount));
		output.push_back(row);
		
		displayStats(correctCount[i], totalFrameCount);
	}
}

/***************************************************************************************************
 * Process Frame for Cascade Classifiers
 *
 * This method processes a given VideoCapture to detect heroes in each frame using cascade classifiers.
 * It stores the resulting correct counts and total number of frames into a csv file for easy data
 * processing.
 *
 **************************************************************************************************/
void processVideoCascadeClassifier(VideoCapture capture, OWConst::Heroes expectedHero,
								   vector<vector<string>>& output, string filepath) {
	vector<string> row;

	Mat frame;
	int correctCount = 0;
	int totalFrameCount = 0;

	cout << "Progress (* per 50 frame): " << endl;
	int match_method; bool use_mask;

	vector<OWConst::Heroes> knownHero = { expectedHero };
	classifier_detector CCDetector(knownHero);

	capture >> frame;
	while (!frame.empty()) {
		capture >> frame;
		if (frame.empty()) {
			break;
		}

		if (totalFrameCount % 50 == 0) {
			cout << "*";
		}
		
		if (CCDetector.evaluateClassifier(frame, expectedHero)) {
			correctCount++;
		}

		totalFrameCount++;
		capture >> frame;
	}
	cout << endl;

	row.clear();

	row.push_back(OWConst::getHeroString(expectedHero));
	row.push_back(filepath);
	row.push_back(DETECTION_TYPES[DETECTION_METHOD]);
	row.push_back(to_string(correctCount));
	row.push_back(to_string(totalFrameCount));
	output.push_back(row);

	displayStats(correctCount, totalFrameCount);
}

/***************************************************************************************************
 * Display Stats
 *
 * This method takes in a correct count and a total number of frames count. Using this, it prints 
 * out what those values are, and as a percentage returns the effectiveness of the detection method.
 *
 **************************************************************************************************/
void displayStats(const int& correct, const int& total) {
	double effectiveness = static_cast<double>(correct) / total * 100.0;
	cout << DETECTION_TYPES[DETECTION_METHOD] << " had: " << endl;

	cout << "Correct: " << correct << ", out of Total: " << total << endl;
	cout << "Effectiveness: " << effectiveness << "% success rate." << endl;
}

/***************************************************************************************************
 * Get Date Time
 *
 * This helper method retrieves the current time. 
 * This can be used to help name the outputted csv files.
 *
 **************************************************************************************************/
string getDateTime() {
	time_t now;
	struct tm timeinfo;
	char buffer[80];

	time(&now);
	localtime_s(&timeinfo, &now);
	strftime(buffer, 80, "%d%m%Y-%H%M%S", &timeinfo);
	return string(buffer);
}