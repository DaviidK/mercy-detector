#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "Detection_Algorithm/Src/Overwatch_Constants/overwatchConstants.h"

using namespace std;
using namespace cv;

OWConst::Heroes identifyHero(Mat& frame, Mat* templ_array, int match_method);

int evalIdentifyHero(Mat& frame, Mat* templ_array, int match_method, OWConst::Heroes expected_hero);
