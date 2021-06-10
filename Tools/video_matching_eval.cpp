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
#include <opencv2/imgproc/imgproc.hpp>
#include "Detection_Algorithm/Src/Overwatch_Constants/overwatchConstants.h"
#include "Detection_Algorithm/Src/Template_Matching/template_matching.h"
#include "Detection_Algorithm/Src/classifier_detector/classifier_detector.h"
#include "Tools/CSV/csv_wrapper.h"
#include "Tools/Meta_File/meta_file.h"

using namespace std;
using namespace cv;

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;

static const string VIDEO_FILE_PATHS = "Detection_Algorithm/Data/Video/video_paths.csv";
static const string VIDEO_FILE_PREFIX = "Detection_Algorithm/Data/Video/";
static const string DETECTION_TYPES[] = { "Template-Matching", "Cascade-Classifier", "Edge-Matching" };
static const int DETECTION_METHOD = 0;
static const bool USE_META_FILE = true;

// Template matching specific parameters
static const int NUM_MATCHING_METHODS = 8;
static const int MATCH_METHOD = 3;
static const bool USE_MASK = true;

void processVideoTemplateMatching(VideoCapture capture,
	OWConst::Heroes expectedHero,
	vector<vector<string>>& output,
	string filePath);

void processMetaTemplateMatching(VideoCapture capture, MetaFile& metaFile, vector<vector<string>>& output, string videoPath);

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
	MetaFile metaFile;

	vector<vector<string>> output;

	for (int i = 0; i < videoFiles.size(); i++) {
		if (USE_META_FILE && videoFiles[i].size() == 1) {
			continue;
		}

		string shortPath = videoFiles[i][0];
		string videoPath = VIDEO_FILE_PREFIX + shortPath;
		OWConst::Heroes expectedHero;

		capture = VideoCapture(videoPath);

		if (USE_META_FILE) {
			const size_t period_idx = videoPath.rfind('.');
			string metaPath = videoPath;
			if (string::npos != period_idx) {
				metaPath.erase(period_idx);
			}

			metaFile = MetaFile(metaPath + ".meta");
		}
		else {
			string hero_name = shortPath.substr(0, shortPath.find("/", 0));
			expectedHero = OWConst::getHero(hero_name);
		}

		if (!capture.isOpened()) {
			cout << "Could not open capture! Either the provided path is invalid, or your build \n" <<
				"of openCV does not support MPEG." << endl;

			return -1;
		}

		cout << "Currently on video " << i + 1 << " of " << videoFiles.size() << endl;

		if (DETECTION_METHOD == 0) {
			if (USE_META_FILE) {
				processMetaTemplateMatching(capture, metaFile, output, shortPath);
			}
			else {
				processVideoTemplateMatching(capture, expectedHero, output, shortPath);
			}
		}
		else if (DETECTION_METHOD == 1) {
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
 * Process Video for Template Matching
 *
 * This method processes a given VideoCapture to detect heroes in each frame.
 * It stores the resulting correct counts and total number of frames into a csv file for easy data
 * processing.
 *
 **************************************************************************************************/
void processVideoTemplateMatching(VideoCapture capture, OWConst::Heroes expectedHero,
	vector<vector<string>>& output, string filepath) {
	vector<string> row;
	Mat frame;
	int correctCount[NUM_MATCHING_METHODS];
	int totalFrameCount = 0;
	int evalFrameCount = 0;

	template_matching TMDetector;

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

		totalFrameCount++;
		if (totalFrameCount % 5 != 0) {
			continue;
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

			correctCount[i] += TMDetector.evalIdentifyHero(frame, match_method, expectedHero, use_mask);
		}

		evalFrameCount++;
	}
	cout << endl;

	for (int i = 0; i < NUM_MATCHING_METHODS; i++) {
		row.clear();

		row.push_back(OWConst::getHeroString(expectedHero));
		row.push_back(filepath);
		row.push_back(DETECTION_TYPES[DETECTION_METHOD]);
		row.push_back(to_string(i));
		row.push_back(to_string(correctCount[i]));
		row.push_back(to_string(evalFrameCount));
		output.push_back(row);

		displayStats(correctCount[i], evalFrameCount);
	}
}

/***************************************************************************************************
 * Process Video for Template Matching using the video metafile
 *
 * This method processes a given VideoCapture to detect heroes in each frame. Unlike the previous
 * template matching video processing video, the video received in this method does not have one
 * single hero or action that each frame corresponds to. Therefore, it uses a metafile that
 * describes the state of each frame of the video to
 *
 *
 **************************************************************************************************/
void processMetaTemplateMatching(VideoCapture capture, MetaFile& metaFile, vector<vector<string>>& output, string videoPath) {
	Mat frame;
	int frameCount = 0;
	int evalFrameCount = 0;
	OWConst::Heroes expectedHero;
	OWConst::Heroes detectedHero;
	OWConst::WeaponActions expectedAction;
	OWConst::WeaponActions detectedAction;

	template_matching TMDetector;

	int heroCorrectCt = 0;
	int heroIncorrectCt = 0;
	int actionCorrectCt = 0;
	int actionIncorrectCt = 0;

	cout << "Progress (* per 50 frame): " << endl;

	while (true) {

		capture >> frame;
		frameCount++;

		if (frame.empty()) {
			break;
		}
		if (frameCount % 50 == 0) {
			cout << "*";
		}

		if (frameCount % 5 != 0) {
			continue;
		}

		expectedHero = metaFile.getHero(frameCount);
		expectedAction = metaFile.getWeaponAction(frameCount);

		detectedHero = TMDetector.identifyHero(frame, MATCH_METHOD, USE_MASK);

		// Assume correct hero was detected to detect the weapon and limit the necessary 
		// template comparisons.
		detectedAction = TMDetector.identifyAction(frame, MATCH_METHOD, USE_MASK, expectedHero);

		cout << "Expected hero: " << OWConst::getHeroString(expectedHero);
		cout << " Result: " << OWConst::getHeroString(detectedHero) << endl;
		if (detectedHero == expectedHero) {
			heroCorrectCt++;
		}
		else {
			heroIncorrectCt++;
		}

		// To simplify the process, we are currently just looking at whether the staff 
		// or the pistol is being held by Mercy.
		if (expectedAction == OWConst::Damage_Boosting ||
			expectedAction == OWConst::Healing) {
			expectedAction = OWConst::Holding_Staff;
		}
		else if (expectedAction == OWConst::Firing) {
			expectedAction = OWConst::Holding_Pistol;
		}

		cout << "Expected action: " << OWConst::getWeaponActionString(expectedAction);
		cout << " Result: " << OWConst::getWeaponActionString(detectedAction) << endl;

		if (expectedAction == detectedAction) {
			actionCorrectCt++;
		}
		else {
			actionIncorrectCt++;
		}

		evalFrameCount++;
	}

	if (output.size() == 0) {
		vector<string> colNames;
		colNames.push_back("videoPath");
		colNames.push_back("heroCorrect");
		colNames.push_back("heroIncorrect");
		colNames.push_back("actionCorrect");
		colNames.push_back("actionIncorrect");
		colNames.push_back("totalFrames");
		output.push_back(colNames);
	}

	vector<string> row;
	row.push_back(videoPath);
	row.push_back(to_string(heroCorrectCt));
	row.push_back(to_string(heroIncorrectCt));
	row.push_back(to_string(actionCorrectCt));
	row.push_back(to_string(actionIncorrectCt));
	row.push_back(to_string(evalFrameCount));

	output.push_back(row);
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