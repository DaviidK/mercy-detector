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
	
	OWConst::Heroes identifyHero(Mat& frame, int match_method, bool use_mask = false);

	int evalIdentifyHero(Mat& frame, int match_method, OWConst::Heroes expected_hero, bool use_mask = false);

	OWConst::WeaponActions identifyAction(Mat& frame, int match_method, bool use_mask, OWConst::Heroes hero);

	void displayDetectedArea(Mat frame, Point matchLoc, Rect resultRect, Mat resultTempl);

private: 
	const vector<OWConst::Heroes> HEROES = { OWConst::Mercy, OWConst::Lucio };
	map<OWConst::Heroes, vector<OWConst::WeaponActions>> ACTIONS;
	const char* templ_file_prefix = "Detection_Algorithm/Data/Templates/";
	string WA_TEMPLATE_FILEPATHS = "Detection_Algorithm/Data/Templates/Weapon_Actions/templ_paths.csv";
	const string TEMPL_FILE_PREFIX = "Detection_Algorithm/Data/Templates/";
	vector<OWConst::Heroes> TM_ACCEPTED_HEROES = { OWConst::Mercy, OWConst::Lucio };

	// Hero Detections
	Mat HERO_TEMPLATES[2];
	Mat EDGE_TEMPLATES[2];
	Mat HERO_MASKS[2];

	// Weapon Action Detections
	map<OWConst::Heroes, vector<Mat>> WA_TEMPLATES;
	map<OWConst::Heroes, vector<Mat>> WA_MASKS;
	map<OWConst::Heroes, vector<Rect>> WA_TEMPL_RECT;
	map<OWConst::Heroes, vector<Mat>> WA_EDGE_TEMPLS;

	static const bool TRY_EDGE = false;
	static const bool TRY_GRAYSCALE = false;
};