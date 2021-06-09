/***************************************************************************************************
 * Classifier Detector
 *
 * @author David Kang
 * @date 06/06/21
 *
 * This file implements an object detector using cascade classifiers. It will use different cascade
 * classifiers, each corresponding to a hero or weapon action, to determine what hero/weapon action
 * is currently displayed in a passed image.
 *
 * Configuration / Assumptions:
 * - HERO_CLASSIFIER_DIRECTORY contains trained cascade classifiers for different heroes, stored 
 *   with the name of that hero.
 * - WEAPON_CLASSIFIER_DIRECTORY contains trained cascade classifiers for different weapon actions, 
     stored with the name of that weapon action.
 **************************************************************************************************/

#include "classifier_detector.h"

/***************************************************************************************************
 * Default Constructor
 *
 * Initializes a classifier_detector object by iterating through all files present in the 
 * HERO_CLASSIFIER_DIRECTORY & WEAPON_CLASSIFIER_DIRECTORY locations. For each file, it will load 
 * the classifier and find the matching OWConst value which corresponds to that classifier
 * 
 * @pre: Trained classifiers have been created and are stored in the location specified by 
 *       HERO_CLASSIFIER_DIRECTORY & WEAPON_CLASSIFIER_DIRECTORY
 * @post: A classifier_detector object will be created with all classifiers loaded, as well as
 *        the corresponding OWConst values for each
***************************************************************************************************/
classifier_detector::classifier_detector() {
    this->heroClassifiers = vector<CascadeClassifier>();
    this->heroConstants = vector<OWConst::Heroes>();
    this->weaponClassifiers = vector<CascadeClassifier>();
    this->weaponConstants = vector<OWConst::WeaponActions>();

    // Iterate through all hero classifiers
    for (const auto& file : filesystem::directory_iterator(HERO_CLASSIFIER_DIRECTORY)) {
        // Save the filepath, then convert it to a string
        filesystem::path filePath(file);
        string filePathString = filePath.generic_string();
        CascadeClassifier heroClassifier;

        // Load the classifier for a given file, and push it to the heroClassifiers field
        heroClassifier.load(filePathString);
        this->heroClassifiers.push_back(heroClassifier);

        // Push the corresponding hero to the heroConstants field
        const size_t heroNameIndex = filePathString.find_last_of("\\/") + 1;
        const size_t extensionIndex = filePathString.find_last_of(".");
        const int heroNameSize = extensionIndex - heroNameIndex;
        const string heroName = filePathString.substr(heroNameIndex, heroNameSize);
        this->heroConstants.push_back(OWConst::getHero(heroName));
    }

    // Iterate through all weapon classifiers 
    for (const auto& file : filesystem::directory_iterator(WEAPON_CLASSIFIER_DIRECTORY)) {
        // Save the filepath, then convert it to a string
        filesystem::path filePath(file);
        string filePathString = filePath.generic_string();
        CascadeClassifier weaponClassifier;

        // Load the classifier for a given file, and push it to the heroClassifiers field
        weaponClassifier.load(filePathString);
        this->weaponClassifiers.push_back(weaponClassifier);

        // Push the corresponding hero to the heroConstants field
        const size_t weaponNameIndex = filePathString.find_last_of("\\/") + 1;
        const size_t extensionIndex = filePathString.find_last_of(".");
        const int weaponNameSize = extensionIndex - weaponNameIndex;
        const string weaponName = filePathString.substr(weaponNameIndex, weaponNameSize);
        this->weaponConstants.push_back(OWConst::getAction(weaponName));
    }
}

/***************************************************************************************************
 * Hero-specific Constructor
 *
 * Initializes a classifier_detector that is specific to certain heroes. It will pull the names of 
 * classifiers from the passed parameter vector and search through all files in the 
 * HERO_CLASSIFIER_DIRECTORY location to find a matching .xml file. IF found, the .xml classifier 
 * will be loaded along with the corresponding OWConst representation of the hero. Weapon action
 * classifiers are not loaded
 * 
 * @param heroesToDetect: A vector containing the heroes which will be detected by the created 
 *        classifier_detector object. Trained classifiers must exist for each hero
 * 
 * @pre:  Trained classifiers for each hero in the passed parameter vector have been created and are
 *        stored in the location specified by HERO_CLASSIFIER_DIRECTORY.
 * @post: A classifier_detector object will be created with classifiers for the specified heroes 
 *        loaded, as well as the corresponding OWConst values for each
***************************************************************************************************/
classifier_detector::classifier_detector(const vector<OWConst::Heroes>& heroesToDetect) {
    this->heroClassifiers = vector<CascadeClassifier>();
    this->heroConstants = vector<OWConst::Heroes>();
    this->weaponClassifiers = vector<CascadeClassifier>();
    this->weaponConstants = vector<OWConst::WeaponActions>();

    for (int i = 0; i < heroesToDetect.size(); i++) {
        CascadeClassifier heroClassifier;
        string classifierFilePath = HERO_CLASSIFIER_DIRECTORY 
                                    + OWConst::getHeroString(heroesToDetect[i])
                                    + ".xml";

        if (!heroClassifier.load(classifierFilePath)) {
            //throw invalid_argument("No available classifier for given hero");
            cout << "ERROR!: Cannot load classifier for " 
                 << OWConst::getHeroString(heroesToDetect[i]) 
                 << endl;
            break;
        }
        else {
            this->heroClassifiers.push_back(heroClassifier);
            this->heroConstants.push_back(heroesToDetect[i]);
        }
    }
}

/***************************************************************************************************
 * identifyHero
 *
 * Will determine if a given hero is present in the passed image by iterating through all loaded
 * hero classifiers and performing object detection on each. If any return true, the corresponding 
 * OWConst value will be returned to the user
 * 
 * @param image: The target image which will be scanned for matching heroes
 * 
 * @post: If a hero matching one of the loaded cascade classifiers is detected, it will be
 *        returned to the user in the form of an OWConst value
 * 
 * @return: The specific hero that was detected in the passed parameter image. If no hero is 
 *          detected, OWConst::No_Hero will be returned
***************************************************************************************************/
OWConst::Heroes classifier_detector::identifyHero(const Mat& image) {
    for (int i = 0; i < this->heroClassifiers.size(); i++) {
        if (detect(image, this->heroClassifiers.at(i))) {
            return this->heroConstants.at(i);
        }
    }

    return OWConst::No_Hero;
}

/***************************************************************************************************
 * identifyAction
 *
 * Will determine if a given weapon action is present in the passed image by iterating through all 
 * loaded weapon action classifiers and performing object detection on each. If any return true, the 
 * corresponding OWConst value will be returned to the user
 * 
 * @param image: The target image which will be scanned for matching weapon actions
 * 
 * @post: If a weapon action matching one of the loaded cascade classifiers is detected, it will
 *        be returned to the user in the form of an OWConst value
 * 
 * @return: The specific weapon action that was detected in the passed parameter image. If no 
 *          weapon action is detected, OWConst::No_Action will be returned
***************************************************************************************************/
OWConst::WeaponActions classifier_detector::identifyAction(const Mat& image) {
    for (int i = 0; i < this->weaponClassifiers.size(); i++) {
        if (detect(image, this->weaponClassifiers.at(i))) {
            return this->weaponConstants.at(i);
        }
    }

    return OWConst::No_Action;
}



/***************************************************************************************************
 * evaluateHeroClassifier
 *
 * Will perform hero detection with an image for which the ground truth is known. Will iterate 
 * through all hero Classifiers and perform object detection on each. If the detected hero matches 
 * the ground truth, will return true.
 * 
 * @param image: The target image which will be scanned for matching heroes
 * @param knownHero: The known hero which is present in the passed target image
 * 
 * @pre:  The target image passed in for evaluation contains a known instance of the passed hero
 * @post: If a hero matching the ground truth hero is detected, this method will return true. 
 *        Returns false otherwise
 * 
 * @return: The result of hero evaluation, true represents a correct identification of the hero
 *          present in the image
***************************************************************************************************/
bool classifier_detector::evaluateHeroClassifier(const Mat& image, const OWConst::Heroes& knownHero) {
    OWConst::Heroes detectedHero = OWConst::No_Hero;

    for (int i = 0; i < this->heroClassifiers.size(); i++) {
        if (detect(image, this->heroClassifiers.at(i))) {
            detectedHero = this->heroConstants.at(i);
            if (detectedHero == knownHero) {
                return true;
            }
        }
    }

    return false;
}

/***************************************************************************************************
 * evaluateWeaponClassifier
 *
 * Will perform weapon action detection with an image for which the ground truth is known. Will 
 * iterate through all weapon action and perform object detection on each. If the detected weapon 
 * action matches the ground truth, will return true.
 * 
 * @param image: The target image which will be scanned for matching weapon actions
 * @param knownAction: The known weapon action which is present in the passed image
 * 
 * @pre:  The target image passed in for evaluation contains a known instance of the passed 
 *        weapon action
 * @post: If an action matching the ground truth weapon action is detected, this method will 
 *        return true. Returns false otherwise.
 * 
 * @return: The result of weapon action evaluation, true represents a correct identification of
 *          theweapon action present in the image.
***************************************************************************************************/
bool classifier_detector::evaluateWeaponClassifier(const Mat& image, 
                                                   const OWConst::WeaponActions& knownAction) {
    OWConst::WeaponActions detectedWeapon = OWConst::No_Action;

    for (int i = 0; i < this->weaponClassifiers.size(); i++) {
        if (detect(image, this->weaponClassifiers.at(i))) {
            detectedWeapon = this->weaponConstants.at(i);
            if (detectedWeapon == knownAction) {
                return true; 
            }
        }
    }

    return false;
}

/***************************************************************************************************
 * detect
 *
 * Method which will perform object detection on a target image using the specified classifier in 
 * the passed parameter.
 * 
 * @param image: The target image which will be scanned for matching objects
 * @param classifier: The classifier which is used for object detection
 * 
 * @post: If the passed image contains an object that is detected via the passed cascade 
 *        classifier, this method will return true. Returns false otherwise
 * 
 * @return: The result of detection, true indicates that an instance of an object corresponding
 *          to the passed cascade classifier has been detected
***************************************************************************************************/
bool classifier_detector::detect(const Mat& image, CascadeClassifier& classifier) {
    Mat grayImage;
    cvtColor(image, grayImage, COLOR_BGR2GRAY);
    equalizeHist(grayImage, grayImage);

    // Determine if an object can be detected in the screenshot 
    vector<Rect> occurrences;
    classifier.detectMultiScale(grayImage, occurrences);

    if (occurrences.size() != 0) {
        return true;
    }
    return false;
}