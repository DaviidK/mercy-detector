// ----------------------------------classifier_detection.h----------------------------------------
// Author: David Kang
// Last modified: 06/02/21
// ------------------------------------------------------------------------------------------------
// Purpose: 
// ------------------------------------------------------------------------------------------------
// Assumptions:
//   - 

#ifndef CLASSIFIER_DETECTOR_H
#define CLASSIFIER_DETECTOR_H

#include "opencv2/objdetect.hpp"
#include "opencv2/imgproc.hpp"
#include <vector>
#include <stdexcept>
#include <filesystem>
#include <string>
#include <iostream>
#include "Detection_Algorithm/Src/Overwatch_Constants/overwatchConstants.h"

using namespace cv;
using namespace std;

class classifier_detector {

public:
    //------------------------------------------------------------------------Public member methods
    classifier_detector(const vector<OWConst::Heroes>& = {OWConst::No_Hero});

    OWConst::Heroes identifyHero(const Mat&);

    bool evaluateClassifier(const Mat&, const OWConst::Heroes&);

private:
    //------------------------------------------------------------------------Private member fields
    vector<CascadeClassifier>* classifiers = nullptr;
    vector<OWConst::Heroes>* classifierHeroes = nullptr;

    //-----------------------------------------------------------------------Private member methods
    bool detect(const Mat&, CascadeClassifier&);
    
};

#endif