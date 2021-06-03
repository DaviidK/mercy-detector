/***************************************************************************************************
 * Object Detection - Template Matching 
 *
 * @author Sana Suse
 * @date 5/20/21
 *
 * This method detects which Overwatch hero is being played from an input image of gameplay. When 
 * an expected hero is passed into the method, it returns whether the detected hero was correct.
 *
 **************************************************************************************************/

#include "template_matching.h"

const vector<OWConst::Heroes> HEROES = { OWConst::Mercy, OWConst::Lucio };
const char* templ_file_prefix = "Detection_Algorithm/Data/Templates/";
static vector<OWConst::Heroes> TM_ACCEPTED_HEROES = { OWConst::Mercy, OWConst::Lucio };
static Mat HERO_TEMPLATES[2]; static Mat HERO_MASKS[2];
static map<OWConst::Heroes, Mat[]> WA_TEMPLATES;
static const string TEMPL_FILE_PREFIX = "Detection_Algorithm/Data/Templates/";

/***************************************************************************************************
 * Temp Matching Setup
 *
 * This method is a specific set up helper method for the template matching method.
 * It loads in the template images into a global vector of Mats.
 *
 **************************************************************************************************/
void tempMatchingSetup() {
	Mat orig; Mat resized;
	for (int i = 0; i < TM_ACCEPTED_HEROES.size(); i++) {
		string filename = TEMPL_FILE_PREFIX + OWConst::getHeroString(TM_ACCEPTED_HEROES[i]) + ".png";
		orig = imread(filename);
		resize(orig, resized, Size(orig.cols, orig.rows));
		HERO_TEMPLATES[i] = resized;

		filename = TEMPL_FILE_PREFIX + OWConst::getHeroString(TM_ACCEPTED_HEROES[i]) + "_mask.png";
		orig = imread(filename);
		resize(orig, resized, Size(orig.cols, orig.rows));
		HERO_MASKS[i] = resized;
	}	
}

/***************************************************************************************************
 * Identify Hero
 *
 * This method detects which Overwatch hero is being played from an input image of gameplay. 
 * It returns an enum of whichever hero was detected in the image.
 * 
 * @params
 *           Mat& frame: The source image to see if a hero can be detected.
 *     int match_method: An integer determining which matching method to use.
 *        bool use_mask: A boolean indicator for whether a mask should be used with the 
 *                       matching method. 
 *
 **************************************************************************************************/
OWConst::Heroes identifyHero(Mat& frame, int match_method, bool use_mask) {
	if (match_method < 0 || match_method > 5) {
		cout << "The match method was invalid." << endl;
		return OWConst::No_Hero;
	}
	
	Mat templ; Mat result; Mat result_templ;
	OWConst::Heroes result_hero = OWConst::No_Hero;
	double tempScore; 
	Point matchLoc;

	// Crop the source image so it only looks at the lower right quadrant. 
	Rect cropRect = Rect(frame.cols / 2, frame.rows / 2, frame.cols / 2, frame.rows / 2);
	Mat cropped = frame(cropRect);

	for (int i = 0; i < HEROES.size(); i++) {
		templ = HERO_TEMPLATES[i];

		if (use_mask && (match_method == TM_SQDIFF || match_method == TM_CCORR_NORMED)) {
			matchTemplate(cropped, templ, result, match_method, HERO_MASKS[i]);
		}
		else {
			matchTemplate(cropped, templ, result, match_method);
		}

		double minVal; double maxVal; Point minLoc; Point maxLoc;
		minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

		// Normalization commented out so the results are comparable to each other. 
		// TODO: Test normalization methods.
		// normalize(result, result, 0, 1, NORM_L2, -1, Mat());

		if (match_method == TM_SQDIFF || match_method == TM_SQDIFF_NORMED) {
			// If first run or score is less than temp score
			if (i == 0 || minVal < tempScore) {
				tempScore = minVal;
				matchLoc = minLoc;
				result_hero = HEROES[i];
				result_templ = templ;
			}
		}
		else {
			if (i == 0 || maxVal > tempScore) {
				tempScore = maxVal;
				matchLoc = maxLoc;
				result_hero = HEROES[i];
				result_templ = templ;
			}
		}
	} 

	// Commented out display of the source image and print of the detected object
	//Mat display_img;
	//frame.copyTo(display_img);
	//Point modifiedPt = Point(matchLoc.x + cropped.cols, matchLoc.y + cropped.rows);
	//rectangle(display_img, modifiedPt, Point(modifiedPt.x + result_templ.cols, modifiedPt.y + result_templ.rows), Scalar::all(0), 2, 8, 0);
	//imshow("result", display_img);
	//cout << OWConst::getHeroString(result_hero) << endl;

	return result_hero;
}

/***************************************************************************************************
 * Eval Identify Hero
 *
 * This method checks if the hero detected from identifyHero() is consistent with the provided 
 * expected_hero parameter.
 * 
 * @params
 *                    Mat& frame: The source image to see if a hero can be detected.
 *              int match_method: An integer determining which matching method to use.
 * OWConst::Heroes expected_hero: An OW constant of what hero is expected to be detected in this 
 *                                image.
 *                 bool use_mask: A boolean indicator for whether a mask should be used with the 
 *                                matching method. 
 *
 **************************************************************************************************/
int evalIdentifyHero(Mat& frame, int match_method, OWConst::Heroes expected_hero, bool use_mask) {
	string result = OWConst::getHeroString(identifyHero(frame, match_method, use_mask));
	return result == OWConst::getHeroString(expected_hero);
}

/***************************************************************************************************
 * Identify Action
 *
 * This method detects what action is being done.
 * TODO: Implement!
 *
 * @params
 *           Mat& frame: The source image to see if a hero can be detected.
 *     int match_method: An integer determining which matching method to use.
 *        bool use_mask: A boolean indicator for whether a mask should be used with the
 *                       matching method.
 *      OWConst::Heroes: The hero that was detected in the given frame.
 *
 **************************************************************************************************/
OWConst::WeaponActions identifyAction(Mat& frame, int match_method, bool use_mask, OWConst::Heroes hero) {

}