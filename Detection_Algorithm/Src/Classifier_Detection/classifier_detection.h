// ----------------------------------classifier_detection.h----------------------------------------
// Author: David Kang
// Last modified: 06/01/21
// ------------------------------------------------------------------------------------------------
// Purpose: 
// ------------------------------------------------------------------------------------------------
// Assumptions:
//   - 

#ifndef CLASSIFIER_DETECTION_H
#define CLASSIFIER_DETECTION_H

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

class classifier_detection {

public:
    //------------------------------------------------------------------------Public member methods
    void cascadeClassifierSetup(const vector<OWConst::Heroes>& = {OWConst::No_Hero});

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