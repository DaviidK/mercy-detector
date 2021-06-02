// ----------------------------------classifier_detection.h----------------------------------------
// Author: David Kang
// Last modified: 05/31/21
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
#include <iostream>
#include "Detection_Algorithm/Src/Overwatch_Constants/overwatchConstants.h"

using namespace cv;
using namespace std;

class classifier_detection {

public:
    //------------------------------------------------------------------------Public member methods
    void cascadeClassifierSetup(const vector<OWConst::Heroes>& = {OWConst::No_Hero});

    void evaluateClassifier(vector<CascadeClassifier>, Mat, vector<OWConst::Heroes> = {OWConst::No_Hero});

private:
    //------------------------------------------------------------------------Private member fields
    vector<CascadeClassifier>* classifiers = nullptr;
    string classifierDirectory = "Detection_Algorithm/Data/Cascade_Classifiers/";
};

#endif