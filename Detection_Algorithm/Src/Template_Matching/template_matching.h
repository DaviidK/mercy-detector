#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "Detection_Algorithm/Src/Overwatch_Constants/overwatchConstants.h"

using namespace std;
using namespace cv;

void tempMatchingSetup();

OWConst::Heroes identifyHero(Mat& frame, int match_method, bool use_mask = false);

int evalIdentifyHero(Mat& frame, int match_method, OWConst::Heroes expected_hero, bool use_mask = false);
