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
 *
 * - HERO_CLASSIFIER_DIRECTORY contains trained cascade classifiers for different heroes, stored 
 *   with the name of that hero.
 * - WEAPON_CLASSIFIER_DIRECTORY contains trained cascade classifiers for different weapon actions, 
     stored with the name of that weapon action.
 **************************************************************************************************/

#include "classifier_detector.h"

const string& HERO_CLASSIFIER_DIRECTORY = "Detection_Algorithm/Data/Cascade_Classifiers/Heroes/";
const string& WEAPON_CLASSIFIER_DIRECTORY = "Detection_Algorithm/Data/Cascade_Classifiers/Weapons/Mercy/";

/**

*/
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

/**

*/
classifier_detector::classifier_detector(const OWConst::Heroes& weaponHero) {
    this->heroClassifiers = vector<CascadeClassifier>();
    this->heroConstants = {weaponHero};
    this->weaponClassifiers = vector<CascadeClassifier>();
    this->weaponConstants = vector<OWConst::WeaponActions>();

    string weaponDirectory = WEAPON_CLASSIFIER_DIRECTORY + OWConst::getHeroString(weaponHero) + "/";

    // Iterate through all files in the classifier directory
    for (const auto& file : filesystem::directory_iterator(weaponDirectory)) {
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

/**

*/
classifier_detector::classifier_detector(const vector<OWConst::Heroes>& heroesToDetect) {
    this->heroClassifiers = vector<CascadeClassifier>();
    this->heroConstants = vector<OWConst::Heroes>();
    this->weaponClassifiers = vector<CascadeClassifier>();
    this->weaponConstants = vector<OWConst::WeaponActions>();

    for (int i = 0; i < heroesToDetect.size(); i++) {
        CascadeClassifier heroClassifier;
        string classifierFilePath = HERO_CLASSIFIER_DIRECTORY + OWConst::getHeroString(heroesToDetect[i]) + ".xml";

        if (!heroClassifier.load(classifierFilePath)) {
            //throw invalid_argument("No available classifier for given hero");
            cout << "ERROR!: Cannot load classifier for " << OWConst::getHeroString(heroesToDetect[i]) << endl;
            break;
        }
        else {
            this->heroClassifiers.push_back(heroClassifier);
            this->heroConstants.push_back(heroesToDetect[i]);
        }
    }
}

/**

*/
OWConst::Heroes classifier_detector::identifyHero(const Mat& image) {
    for (int i = 0; i < this->heroClassifiers.size(); i++) {
        if (detect(image, this->heroClassifiers.at(i))) {
            return this->heroConstants.at(i);
        }
    }

    return OWConst::No_Hero;
}

/**

*/
OWConst::WeaponActions classifier_detector::identifyAction(const Mat& image) {
    for (int i = 0; i < this->weaponClassifiers.size(); i++) {
        if (detect(image, this->weaponClassifiers.at(i))) {
            return this->weaponConstants.at(i);
        }
    }

    return OWConst::No_Action;
}



/**

*/
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

/**

*/
bool classifier_detector::evaluateWeaponClassifier(const Mat& image, const OWConst::WeaponActions& knownAction) {
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