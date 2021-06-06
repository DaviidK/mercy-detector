/***************************************************************************************************
 * Classifier Detector
 *
 * @author David Kang
 * @date 06/06/21
 *
 * This file defines an object detector using cascade classifiers. It will load previously
 * trained classifiers into different vectors, along with the corresponding Heroes & Weapons via
 * the OWConst class.
 *
 * Configuration / Assumptions:
 *
 * - HERO_CLASSIFIER_DIRECTORY contains trained cascade classifiers for different heroes, stored
 *   with the name of that hero.
 * - WEAPON_CLASSIFIER_DIRECTORY contains trained cascade classifiers for different weapon actions,
     stored with the name of that weapon action.
 **************************************************************************************************/

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
    // Default constructor: Will instantiate 
    classifier_detector();

    classifier_detector(const OWConst::Heroes&);

    classifier_detector(const vector<OWConst::Heroes>&);

    OWConst::Heroes identifyHero(const Mat&);

    OWConst::WeaponActions identifyWeaponAction(const Mat&);

    bool evaluateHeroClassifier(const Mat&, const OWConst::Heroes&);

    bool evaluateWeaponClassifier(const Mat&, const OWConst::WeaponActions&);

private:
    //------------------------------------------------------------------------Private member fields
    vector<CascadeClassifier> heroClassifiers;
    vector<OWConst::Heroes> heroConstants;

    vector<CascadeClassifier> weaponClassifiers;
    vector<OWConst::WeaponActions> weaponConstants;

    //-----------------------------------------------------------------------Private member methods
    bool detect(const Mat&, CascadeClassifier&);
    
};

#endif