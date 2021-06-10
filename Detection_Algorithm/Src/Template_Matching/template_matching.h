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

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <map>
#include "Detection_Algorithm/Src/Overwatch_Constants/overwatchConstants.h"
#include "Tools/CSV/csv_wrapper.h"
#include "Tools/Edge_Processing/edge_processing.h"

using namespace std;
using namespace cv;

class template_matching {

public:
	template_matching();

	/***********************************************************************************************
	 * Identify Hero
	 *
	 * This method detects which Overwatch hero is being played from an input image of gameplay.
	 * It returns an OWConst::Heroes enum of whichever hero was detected in the image.
	 *
	 * @param frame : The source image as a Mat which will be used to see if a hero can be detected
	 *				  from within it.
	 * @param match_method : An integer determining which matching method to use.
	 * @param use_mask : A boolean indicator for whether a mask should be used with the matching
						 method.
	 *
	 * @pre The input image must be a valid Mat, match_method must be a value between 0 and 5 (both
	 *      inclusive) and use_mask must only be true if match_method is equal to 0 or 3.
	 * @post Returns a hero enum of which hero was detected in the input image.
	 *
	 * @return A hero enum that was detected in the input frame.
	 **********************************************************************************************/
	OWConst::Heroes identifyHero(Mat& frame, int match_method, bool use_mask = false);

	/***********************************************************************************************
     * Eval Identify Hero
     *
     * This method checks if the hero detected from identifyHero() is consistent with the provided
     * expected_hero parameter.
     *
     * @param frame : The source image as a Mat which will be used to see if a hero can be detected
	 *  			  from within it.
     * @param match_method : An integer determining which matching method to use.
     * @param expected_hero : An OWConst::Heroes enum of which hero is expected in the given frame.
     * @param use_mask : A boolean indicator for whether a mask should be used with the matching 
	 *                   method.
     *
     * @pre The input image must be a valid Mat, match_method must be a value between 0 and 5 (both
     *      inclusive), expected_hero must be a valid OWConst::Heroes enum and use_mask must only 
     *      be true if match_method is equal to 0 or 3.
     * @post Returns a hero enum of which hero was detected in the input image.
     *
     * @return An integer that represents the boolean value of whether the expected hero was 
	 *	       detected in the frame or not.
     **********************************************************************************************/
	int evalIdentifyHero(Mat& frame,
		                 int match_method,
		                 OWConst::Heroes expected_hero,
		                 bool use_mask = false);

	/***********************************************************************************************
	 * Identify Action
	 *
	 * This method detects what action is being done by the hero in the frame
	 *
	 * @param frame : The source image to see if a hero can be detected.
	 * @param match_method : An integer determining which matching method to use.
	 * @param use_mask : A boolean indicator for whether a mask should be used with the matching 
	 *                   method.
	 * @param hero : The hero that was detected in the given frame and the hero of which their 
	 *               weapon action shall be detected in the frame.
	 *
	 * @pre The input image must be a valid Mat, match_method must be a value between 0 and 5 (both
	 *      inclusive), hero must be a valid OWConst::Heroes enum and use_mask must only be true if
	 *      match_method is equal to 0 or 3.
	 * @post Returns a WeaponActions enum of which weapon action was detected in the given input 
	 *       image.
	 *
	 * @return A WeaponActions enum that describes the current
	 **********************************************************************************************/
	OWConst::WeaponActions identifyAction(Mat& frame,
										  int match_method,
										  bool use_mask,
		                                  OWConst::Heroes hero);

	/***********************************************************************************************
	 * Display Detected Region
	 *
	 * This method displays the region in which the object was detected onto the input image.
	 *
	 * @param frame: The source image to see if a hero can be detected.
	 * @param matchLoc:
	 * @param resultRect:
	 * @param resultTempl: The hero that was detected in the given frame.
	 *
	 * @pre The Mat parameters must be valid Mats, matchLoc must be within the sizes of the input 
	 *      frame, and resultRect must be within the size of the input frame once added to MatchLoc.
	 * @post Displays the input image and the detected region to the screen.
	 **********************************************************************************************/
	void displayDetectedArea(Mat frame, Point matchLoc, Rect resultRect, Mat resultTempl);

private:
	// File paths for accessing data
	const string TEMPL_FILE_PREFIX = "Detection_Algorithm/Data/Templates/";
	const string WA_TEMPLATE_FILEPATHS = TEMPL_FILE_PREFIX + "Weapon_Actions/templ_paths.csv";

	// Hero Detection Instance Variables
	vector<OWConst::Heroes> TM_ACCEPTED_HEROES = { OWConst::Mercy, OWConst::Lucio };
	Mat HERO_TEMPLATES[2];
	Mat EDGE_TEMPLATES[2];
	Mat HERO_MASKS[2];

	// Weapon Action Detection Instance Variables
	map<OWConst::Heroes, vector<OWConst::WeaponActions>> ACTIONS;
	map<OWConst::Heroes, vector<Mat>> WA_TEMPLATES;
	map<OWConst::Heroes, vector<Mat>> WA_MASKS;
	map<OWConst::Heroes, vector<Rect>> WA_TEMPL_RECT;
	map<OWConst::Heroes, vector<Mat>> WA_EDGE_TEMPLS;

	// Configurations
	static const bool TRY_EDGE = true;
	static const bool TRY_GRAYSCALE = false;
};