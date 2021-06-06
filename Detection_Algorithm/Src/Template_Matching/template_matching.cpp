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

/***************************************************************************************************
 * temp_matching constructor
 *
 * This method is a constructor for the template matching method.
 * It loads in the template images into a global vector of Mats.
 *
 **************************************************************************************************/

template_matching::template_matching() {
	string filename;
	for (int i = 0; i < TM_ACCEPTED_HEROES.size(); i++) {
		filename = TEMPL_FILE_PREFIX + OWConst::getHeroString(TM_ACCEPTED_HEROES[i]) + ".png";
		HERO_TEMPLATES[i] = imread(filename);

		filename = TEMPL_FILE_PREFIX + OWConst::getHeroString(TM_ACCEPTED_HEROES[i]) + "_mask.png";
		HERO_MASKS[i] = imread(filename);
	}

	vector<vector<string>> paths;
	csv_wrapper::readFromCSV(WA_TEMPLATE_FILEPATHS, paths);
	string heroname;
	OWConst::Heroes hero;
	vector<Mat> templates;
	vector<Mat> masks;
	vector<Rect> positionRects;
	vector<OWConst::WeaponActions> actions;

	for (int i = 0; i < paths.size(); i++) {
		if (paths[i].size() == 1) {
			if (i != 0) {
				WA_TEMPLATES[hero] = templates;
				WA_MASKS[hero] = masks;
				WA_TEMPL_RECT[hero] = positionRects;
				ACTIONS[hero] = actions;
			}
			templates.clear();
			masks.clear();
			positionRects.clear();

			heroname = paths[i][0];
			hero = OWConst::getHero(heroname);
		}
		else {
			filename = TEMPL_FILE_PREFIX + "Weapon_Actions/" + paths[i][0] + ".png";
			templates.push_back(imread(filename));

			filename = TEMPL_FILE_PREFIX + "Weapon_Actions/" + paths[i][0] + "_mask.png";
			masks.push_back(imread(filename));

			actions.push_back(OWConst::getAction(paths[i][1]));
			
			Rect cropRect = Rect(std::stoi(paths[i][2]), std::stoi(paths[i][3]), 
				                 std::stoi(paths[i][4]), std::stoi(paths[i][5]));
			positionRects.push_back(cropRect);
		}
		
		if (paths.size() - 1 == i) {
			WA_TEMPLATES[hero] = templates;
			WA_MASKS[hero] = masks;
			WA_TEMPL_RECT[hero] = positionRects;
			ACTIONS[hero] = actions;
		}
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
OWConst::Heroes template_matching::identifyHero(Mat& frame, int match_method, bool use_mask) {
	if (match_method < 0 || match_method > 5) {
		cout << "The match method was invalid." << endl;
		return OWConst::No_Hero;
	}
	
	Mat templ; Mat result; Mat result_templ;
	OWConst::Heroes result_hero = OWConst::No_Hero;
	double currScore; 
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
			if (i == 0 || minVal < currScore) {
				currScore = minVal;
				matchLoc = minLoc;
				result_hero = HEROES[i];
				result_templ = templ;
			}
		}
		else {
			if (i == 0 || maxVal > currScore) {
				currScore = maxVal;
				matchLoc = maxLoc;
				result_hero = HEROES[i];
				result_templ = templ;
			}
		}
	} 

	// Commented out display of the source image and print of the detected object
	// cout << "Hero: " << OWConst::getHeroString(result_hero) << endl;
	// displayDetectedArea(frame, matchLoc, resultRect, resultTempl);

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
int template_matching::evalIdentifyHero(Mat& frame, int match_method, OWConst::Heroes expected_hero, bool use_mask) {
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
OWConst::WeaponActions template_matching::identifyAction(Mat& frame, int match_method, bool use_mask, OWConst::Heroes hero) {
	OWConst::WeaponActions result_action = OWConst::No_Action;
	Mat templ; Mat result; Mat mask; Mat cropped;
	Rect resultRect; Mat resultTempl;

	double currScore;
	Point matchLoc;

	for (int i = 0; i < WA_TEMPLATES[hero].size(); i++) {
		templ = WA_TEMPLATES[hero][i];
		mask = WA_MASKS[hero][i];
		Rect cropRect = WA_TEMPL_RECT[hero][i];
		cropped = frame(cropRect);

		if (use_mask && (match_method == TM_SQDIFF || match_method == TM_CCORR_NORMED)) {
			matchTemplate(cropped, templ, result, match_method, mask);
		}
		else {
			matchTemplate(cropped, templ, result, match_method);
		}

		double minVal; double maxVal; Point minLoc; Point maxLoc;
		minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

		if (match_method == TM_SQDIFF || match_method == TM_SQDIFF_NORMED) {
			// If first run or score is less than temp score
			if (i == 0 || minVal < currScore) {
				currScore = minVal;
				matchLoc = minLoc;
				resultRect = cropRect;
				resultTempl = templ;
				result_action = ACTIONS[hero][i];
			}
		}
		else {
			if (i == 0 || maxVal > currScore) {
				currScore = maxVal;
				matchLoc = maxLoc;
				resultRect = cropRect;
				resultTempl = templ;
				result_action = ACTIONS[hero][i];
			}
		}
	}

	// Commented out display of the source image and print of the detected object
	// cout << "Action: " << OWConst::getWeaponActionString(result_action) << endl;
	// displayDetectedArea(frame, matchLoc, resultRect, resultTempl);
	
	return result_action;
}

void template_matching::displayDetectedArea(Mat frame, Point matchLoc, Rect resultRect, Mat resultTempl) {
	// Commented out display of the source image and print of the detected object
	Mat display_img;
	frame.copyTo(display_img);
	Point modifiedPt = Point(matchLoc.x + resultRect.x, matchLoc.y + resultRect.y);
	rectangle(display_img, modifiedPt, Point(modifiedPt.x + resultTempl.cols, modifiedPt.y + resultTempl.rows), Scalar::all(0), 2, 8, 0);
	imshow("result", display_img);
	waitKey(0);
}
