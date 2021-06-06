// ----------------------------------classifier_detector.cpp--------------------------------------
// Author: David Kang
// Last modified: 06/03/21
// ------------------------------------------------------------------------------------------------
// Purpose: 
// ------------------------------------------------------------------------------------------------
// Assumptions:
//   - Trained cascade heroClassifiers are stored in 'Detection_Algorithm/Data/Cascade_Classifiers/' 
//     and have the same name as the hero they are meant to detect. 

#include "classifier_detector.h"

const string& HERO_CLASSIFIER_DIRECTORY = "Detection_Algorithm/Data/Cascade_Classifiers/Heroes/";
const string& WEAPON_CLASSIFIER_DIRECTORY = "Detection_Algorithm/Data/Cascade_Classifiers/Weapons/";

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
classifier_detector::classifier_detector() {
    this->heroClassifiers = vector<CascadeClassifier>();
    this->heroConstants = vector<OWConst::Heroes>();
    this->weaponClassifiers = vector<CascadeClassifier>();
    this->weaponConstants = vector<OWConst::WeaponActions>();

    // Iterate through all files in the classifier directory
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
OWConst::WeaponActions classifier_detector::identifyWeaponAction(const Mat& image) {
    for (int i = 0; i < this->weaponClassifiers.size(); i++) {
        if (detect(image, this->weaponClassifiers.at(i))) {
            return this->weaponConstants.at(i);
        }
    }

    return OWConst::No_Action;
}

/**

*/
bool classifier_detector::evaluateClassifier(const Mat& image, const OWConst::Heroes& knownHero) {
    OWConst::Heroes detectedHero = OWConst::No_Hero;

    for (int i = 0; i < this->heroClassifiers.size(); i++) {
        if (detect(image, this->heroClassifiers.at(i))) {
            detectedHero = this->heroConstants.at(i);
            return detectedHero == knownHero; 
        }
    }

    return false;
}



bool classifier_detector::detect(const Mat& image, CascadeClassifier& classifier) {
    // Crop the source image so it only looks at bottom right quarter of screen
    Rect newSize = Rect(image.cols / 2, image.rows / 2, image.cols / 2, image.rows / 2);
    Mat croppedImage = image(newSize);

    Mat grayImage;
    cvtColor(croppedImage, grayImage, COLOR_BGR2GRAY);
    equalizeHist(grayImage, grayImage);

    // Determine if a hero can be detected in the screenshot 
    vector<Rect> heroOccurrences;
    classifier.detectMultiScale(grayImage, heroOccurrences);

    if (heroOccurrences.size() != 0) {
        return true;
    }
    return false;
}