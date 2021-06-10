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

    const string& HERO_CLASSIFIER_DIRECTORY = "Detection_Algorithm/Data/Cascade_Classifiers/Heroes/";
    const string& WEAPON_CLASSIFIER_DIRECTORY = "Detection_Algorithm/Data/Cascade_Classifiers/Weapons/Mercy/";
    
public:
    /***********************************************************************************************
     * Default Constructor
     *
     * Initializes a classifier_detector object by loading all available classifiers present in the 
     * data directories
     * 
     * @pre: Trained classifiers have been created and are stored in the location specified by 
     *       HERO_CLASSIFIER_DIRECTORY & WEAPON_CLASSIFIER_DIRECTORY
     * @post: A classifier_detector object will be created with all classifiers loaded, as well as
     *        the corresponding OWConst values for each
    ***********************************************************************************************/
    classifier_detector();

    /***********************************************************************************************
     * Hero-specific Constructor
     *
     * Initializes a classifier_detector object by loading only the classifiers that correspond to
     * the heroes specified in the passed parameter vector
     * 
     * @param const vector<OWConst::Heroes&>: A vector containing the heroes which will be detected 
     *        by the created classifier_detector object. Trained classifiers must exist for each hero
     * 
     * @pre:  Trained classifiers for each hero in the passed parameter vector have been created and 
     *        are stored in the location specified by HERO_CLASSIFIER_DIRECTORY
     * @post: A classifier_detector object will be created with classifiers for the specified heroes 
     *        loaded, as well as the corresponding OWConst values for each
    ***********************************************************************************************/
    classifier_detector(const vector<OWConst::Heroes>&);

    /***********************************************************************************************
     * identifyHero
     *
     * Will determine if a given hero is present in the passed image by performing object detection
     * using all loaded hero cascade classifiers
     * 
     * @param const Mat&: The target image which will be scanned for matching heroes
     * 
     * @post: If a hero matching one of the loaded cascade classifiers is detected, it will be
     *        returned to the user in the form of an OWConst value
     * 
     * @return: The specific hero that was detected in the passed parameter image. If no hero is 
     *          detected, OWConst::No_Hero will be returned
    ***********************************************************************************************/
    OWConst::Heroes identifyHero(const Mat&);

    /***********************************************************************************************
     * identifyAction
     *
     * Will determine if a given weapon action is present in the passed image by performing object 
     * detection using all loaded weapon action cascade classifiers
     * 
     * @param const Mat&: The target image which will be scanned for matching weapon actions
     * 
     * @post: If a weapon action matching one of the loaded cascade classifiers is detected, it will
     *        be returned to the user in the form of an OWConst value
     * 
     * @return: The specific weapon action that was detected in the passed parameter image. If no 
     *          weapon action is detected, OWConst::No_Action will be returned
    ***********************************************************************************************/
    OWConst::WeaponActions identifyAction(const Mat&);

    /***********************************************************************************************
     * evaluateHeroClassifier
     *
     * Will perform hero detection with an image for which the ground truth is known. If the result 
     * of hero detection matches the ground truth, will return true. Returns false otherwise.
     * 
     * @param const Mat&: The target image which will be scanned for matching heroes
     * @param const OWConst::Heroes&: The known hero which is present in the passed target image
     * 
     * @pre:  The target image passed in for evaluation contains a known instance of the passed hero
     * @post: If a hero matching the ground truth hero is detected, this method will return true. 
     *        Returns false otherwise
     * 
     * @return: The result of hero evaluation, true represents a correct identification of the hero
     *          present in the image
    ***********************************************************************************************/
    bool evaluateHeroClassifier(const Mat&, const OWConst::Heroes&);

    /***********************************************************************************************
     * evaluateWeaponClassifier
     *
     * Will perform weapon action detection with an image for which the ground truth is known. If 
     * the result of weapon action detection matches the ground truth, will return true. 
     * Returns false otherwise.
     * 
     * @param const Mat&: The target image which will be scanned for matching weapon actions
     * @param const OWConst::WeaponActions&: The known weapon action which is present in the passed image
     * 
     * @pre:  The target image passed in for evaluation contains a known instance of the passed 
     *        weapon action
     * @post: If an action matching the ground truth weapon action is detected, this method will 
     *        return true. Returns false otherwise.
     * 
     * @return: The result of weapon action evaluation, true represents a correct identification of
     *          theweapon action present in the image.
    ***********************************************************************************************/
    bool evaluateWeaponClassifier(const Mat&, const OWConst::WeaponActions&);

private:
    // The loaded classifiers used for hero detection
    vector<CascadeClassifier> heroClassifiers;
    // OWConst values which correspond with each loaded hero classifier
    vector<OWConst::Heroes> heroConstants;

    // The loaded classifiers used for weapon action detection
    vector<CascadeClassifier> weaponClassifiers;
    // OWConst values which correspond with each loaded weapon action classifier
    vector<OWConst::WeaponActions> weaponConstants;

    /***********************************************************************************************
     * detect
     *
     * Method which will perform object detection using a passed parameter cascade classifier.
     * 
     * @param const Mat&: The target image which will be scanned for matching objects
     * @param CascadeClassifier&: The classifier which is used for object detection
     * 
     * @post: If the passed image contains an object that is detected via the passed cascade 
     *        classifier, this method will return true. Returns false otherwise
     * 
     * @return: The result of detection, true indicates that an instance of an object corresponding
     *          to the passed cascade classifier has been detected.
    ***********************************************************************************************/
    bool detect(const Mat&, CascadeClassifier&);
    
};

#endif