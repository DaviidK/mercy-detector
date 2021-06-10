/***************************************************************************************************
 * Object Detection - Template Matching 
 *
 * @author Sana Suse
 * @date 5/20/21
 *
 * This class detects what is present in an input image of gameplay. It can detect either an action 
 * or a hero. 
 * 
 * Configurations / Assumptions 
 * - There are preprocessing configurations that can be changed in the header file. These are the 
 *   TRY_EDGE and TRY_GRAYSCALE private instance variables
 * - TEMPL_FILE_PREFIX contains the template images to be used for template matching
 * - TM_ACCEPTED_HEROES only contains the heroes that are able to be detected in the image. 
 **************************************************************************************************/

#include "template_matching.h"

/***************************************************************************************************
 * Default Constructor
 *
 * The constructor for the template matching class.
 * It loads in the template, mask and edge images into a global vector of Mats.
 *
 * @pre The images must be valid PNG images and stored in the appropriate directory.
 * @post Initializes the class instance variables.
 **************************************************************************************************/
template_matching::template_matching() {
	string filename;
	for (int i = 0; i < TM_ACCEPTED_HEROES.size(); i++) {
		filename = TEMPL_FILE_PREFIX + OWConst::getHeroString(TM_ACCEPTED_HEROES[i]) + ".png";
		HERO_TEMPLATES[i] = imread(filename);

		filename = TEMPL_FILE_PREFIX + OWConst::getHeroString(TM_ACCEPTED_HEROES[i]) + "_Edge.png";
		EDGE_TEMPLATES[i] = imread(filename);

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
	vector<Mat> edgeTemplates;
	vector<OWConst::WeaponActions> actions;

	for (int i = 0; i < paths.size(); i++) {
		if (paths[i].size() == 1) {
			if (i != 0) {
				WA_TEMPLATES[hero] = templates;
				WA_MASKS[hero] = masks;
				WA_TEMPL_RECT[hero] = positionRects;
				WA_EDGE_TEMPLS[hero] = edgeTemplates;
				ACTIONS[hero] = actions;
			}
			templates.clear();
			masks.clear();
			positionRects.clear();
			edgeTemplates.clear();

			heroname = paths[i][0];
			hero = OWConst::getHero(heroname);
		}
		else {
			filename = TEMPL_FILE_PREFIX + "Weapon_Actions/" + paths[i][0] + ".png";
			templates.push_back(imread(filename));

			filename = TEMPL_FILE_PREFIX + "Weapon_Actions/" + paths[i][0] + "_mask.png";
			masks.push_back(imread(filename));

			filename = TEMPL_FILE_PREFIX + "Weapon_Actions/" + paths[i][0] + "_Edge.png";
			edgeTemplates.push_back(imread(filename));

			actions.push_back(OWConst::getAction(paths[i][1]));
			
			Rect cropRect = Rect(std::stoi(paths[i][2]), std::stoi(paths[i][3]), 
				                 std::stoi(paths[i][4]), std::stoi(paths[i][5]));
			positionRects.push_back(cropRect);
		}
		
		if (paths.size() - 1 == i) {
			WA_TEMPLATES[hero] = templates;
			WA_MASKS[hero] = masks;
			WA_TEMPL_RECT[hero] = positionRects;
			WA_EDGE_TEMPLS[hero] = edgeTemplates;
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
 * @param frame : The source image as a Mat which will be used to see if a hero can be detected 
 *                from within it.
 * @param match_method : An integer determining which matching method to use.
 * @param use_mask : A boolean indicator for whether a mask should be used with the matching method. 
 * 
 * @pre The input image must be a valid Mat, match_method must be a value between 0 and 5 (both 
 *      inclusive) and use_mask must only be true if match_method is equal to 0 or 3.
 * @post Returns a hero enum of which hero was detected in the input image.
 * 
 * @return A hero enum that was detected in the input frame.
 **************************************************************************************************/
OWConst::Heroes template_matching::identifyHero(Mat& frame, int match_method, bool use_mask) {
	if (match_method < 0 || match_method > 5) {
		cout << "The match method was invalid." << endl;
		return OWConst::No_Hero;
	}
	
	Mat templ; Mat mask; Mat result; Mat result_templ;
	OWConst::Heroes result_hero = OWConst::No_Hero;
	double currScore; 
	Point matchLoc;

	// Crop the source image so it only looks at the lower right quadrant. 
	Rect cropRect = Rect(frame.cols / 2, frame.rows / 2, frame.cols / 2, frame.rows / 2);
	Mat cropped = frame(cropRect);

	if (TRY_GRAYSCALE) {
		Mat copy;
		cropped.copyTo(copy);
		cvtColor(copy, cropped, COLOR_BGR2GRAY);
		equalizeHist(cropped, cropped);
	}

	if (TRY_EDGE) {
		cropped = createEdgeMap(cropped);
	}

	for (int i = 0; i < TM_ACCEPTED_HEROES.size(); i++) {
		if (use_mask) {
			mask = HERO_MASKS[i];
		}
		if (TRY_EDGE) {
			templ = EDGE_TEMPLATES[i];
		}
		else {
			templ = HERO_TEMPLATES[i];
		}

		if (TRY_GRAYSCALE) {
			Mat copy;
			templ.copyTo(copy);
			cvtColor(copy, templ, COLOR_BGR2GRAY);
			equalizeHist(templ, templ);
			if (use_mask) {
				mask.copyTo(copy);
				cvtColor(copy, mask, COLOR_BGR2GRAY);
				equalizeHist(mask, mask);			
			}
		}

		if (use_mask && (match_method == TM_SQDIFF || match_method == TM_CCORR_NORMED)) {
			matchTemplate(cropped, templ, result, match_method, mask);
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
			// If first run or score is less than current score
			if (i == 0 || minVal < currScore) {
				currScore = minVal;
				matchLoc = minLoc;
				result_hero = TM_ACCEPTED_HEROES[i];
				result_templ = templ;
			}
		}
		else {
			// If first run or score is greater than current score
			if (i == 0 || maxVal > currScore) {
				currScore = maxVal;
				matchLoc = maxLoc;
				result_hero = TM_ACCEPTED_HEROES[i];
				result_templ = templ;
			}
		}
	} 

	// Displays the source image with detected region and prints to console the detected hero 
	// for debugging purposes
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
 * @param frame : The source image as a Mat which will be used to see if a hero can be detected
 *   			  from within it.
 * @param match_method : An integer determining which matching method to use.
 * @param expected_hero : An OWConst::Heroes enum of which hero is expected in the given frame.
 * @param use_mask : A boolean indicator for whether a mask should be used with the matching method.
 *
 * @pre The input image must be a valid Mat, match_method must be a value between 0 and 5 (both
 *      inclusive), expected_hero must be a valid OWConst::Heroes enum and use_mask must only be 
 *      true if match_method is equal to 0 or 3.
 * @post Returns a hero enum of which hero was detected in the input image.
 *
 * @return An integer that represents the boolean value of whether the expected hero was detected 
 *         in the frame or not.
 **************************************************************************************************/
int template_matching::evalIdentifyHero(Mat& frame, int match_method, OWConst::Heroes expected_hero, 
										bool use_mask) {
	string result = OWConst::getHeroString(
						identifyHero(frame, match_method, use_mask));

	
	return result == OWConst::getHeroString(expected_hero);
}

/***************************************************************************************************
 * Identify Action
 *
 * This method detects what action is being done by the hero in the frame
 *
 * @param frame : The source image to see if a hero can be detected.
 * @param match_method : An integer determining which matching method to use.
 * @param use_mask : A boolean indicator for whether a mask should be used with the matching method.
 * @param hero : The hero that was detected in the given frame and the hero of which their weapon 
 *               action shall be detected in the frame.
 *
 * @pre The input image must be a valid Mat, match_method must be a value between 0 and 5 (both
 *      inclusive), hero must be a valid OWConst::Heroes enum and use_mask must only be true if
 *      match_method is equal to 0 or 3.
 * @post Returns a WeaponActions enum of which weapon action was detected in the given input image.
 *
 * @return A WeaponActions enum that describes the current 
 **************************************************************************************************/
OWConst::WeaponActions template_matching::identifyAction(Mat& frame, 
														 int match_method, 
														 bool use_mask, 
	                                                     OWConst::Heroes hero) {
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

		if (TRY_EDGE) {
			cropped = createEdgeMap(cropped);
			templ = WA_EDGE_TEMPLS[hero][i];
		}
		else {
			templ = WA_EDGE_TEMPLS[hero][i];
		}

		if (TRY_GRAYSCALE) {
			Mat copy;
			cropped.copyTo(copy);
			cvtColor(copy, cropped, COLOR_BGR2GRAY);
			equalizeHist(cropped, cropped);

			templ.copyTo(copy);
			cvtColor(copy, templ, COLOR_BGR2GRAY);
			equalizeHist(templ, templ);
			if (use_mask) {
				mask.copyTo(copy);
				cvtColor(copy, mask, COLOR_BGR2GRAY);
				equalizeHist(mask, mask);
			}
		}

		if (use_mask && (match_method == TM_SQDIFF || match_method == TM_CCORR_NORMED)) {
			matchTemplate(cropped, templ, result, match_method, mask);
		}
		else {
			matchTemplate(cropped, templ, result, match_method);
		}

		double minVal; double maxVal; Point minLoc; Point maxLoc;
		minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

		if (match_method == TM_SQDIFF || match_method == TM_SQDIFF_NORMED) {
			// If first run or score is less than current score
			if (i == 0 || minVal < currScore) {
				currScore = minVal;
				matchLoc = minLoc;
				resultRect = cropRect;
				resultTempl = templ;
				result_action = ACTIONS[hero][i];
			}
		}
		else {
			// If first run or score is greater than current score
			if (i == 0 || maxVal > currScore) {
				currScore = maxVal;
				matchLoc = maxLoc;
				resultRect = cropRect;
				resultTempl = templ;
				result_action = ACTIONS[hero][i];
			}
		}
	}

	// Displays the source image with detected region and prints to console the detected hero 
	// for debugging purposes
	// cout << "Action: " << OWConst::getWeaponActionString(result_action) << endl;
	// displayDetectedArea(frame, matchLoc, resultRect, resultTempl);
	
	return result_action;
}

/***************************************************************************************************
 * Display Detected Region
 *
 * This method displays the region in which the object was detected onto the input image. 
 *
 * @param frame: The source image to see if a hero can be detected.
 * @param matchLoc: 
 * @param resultRect: 
 * @param resultTempl: The hero that was detected in the given frame.
 * 
 * @pre The Mat parameters must be valid Mats, matchLoc must be within the sizes of the input frame,
 *		and the resultRect must be within the size of the input frame once added to MatchLoc.
 * @post Displays the input image and the detected region to the screen.
 **************************************************************************************************/
void template_matching::displayDetectedArea(Mat frame, Point matchLoc, 
											Rect resultRect, Mat resultTempl) {
	Mat display_img;
	frame.copyTo(display_img);
	Point modifiedPt = Point(matchLoc.x + resultRect.x, matchLoc.y + resultRect.y);
	rectangle(display_img, modifiedPt, 
		      Point(modifiedPt.x + resultTempl.cols, modifiedPt.y + resultTempl.rows), 
		      Scalar::all(0), 2, 8, 0);
	imshow("result", display_img);
	waitKey(0);
}
