#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <map>
#include "Detection_Algorithm/Src/Overwatch_Constants/overwatchConstants.h"
#include "Tools/CSV/csv_wrapper.h"

using namespace std;
using namespace cv;

class template_matching {

public: 
	template_matching();
	
	OWConst::Heroes identifyHero(Mat& frame, int match_method, bool use_mask = false);

	int evalIdentifyHero(Mat& frame, int match_method, OWConst::Heroes expected_hero, bool use_mask = false);

	OWConst::WeaponActions identifyAction(Mat& frame, int match_method, bool use_mask, OWConst::Heroes hero);

private: 
	const vector<OWConst::Heroes> HEROES = { OWConst::Mercy, OWConst::Lucio };
	vector<OWConst::WeaponActions> ACTIONS;
	const char* templ_file_prefix = "Detection_Algorithm/Data/Templates/";
	vector<OWConst::Heroes> TM_ACCEPTED_HEROES = { OWConst::Mercy, OWConst::Lucio };
	Mat HERO_TEMPLATES[2]; 
	Mat HERO_MASKS[2];
	map<OWConst::Heroes, vector<Mat>> WA_TEMPLATES;
	string WA_TEMPLATE_FILEPATHS;
	const string TEMPL_FILE_PREFIX = "Detection_Algorithm/Data/Templates/";
};