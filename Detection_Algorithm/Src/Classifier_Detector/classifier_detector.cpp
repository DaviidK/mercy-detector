// ----------------------------------classifier_detector.cpp--------------------------------------
// Author: David Kang
// Last modified: 06/02/21
// ------------------------------------------------------------------------------------------------
// Purpose: 
// ------------------------------------------------------------------------------------------------
// Assumptions:
//   - Trained cascade classifiers are stored in 'Detection_Algorithm/Data/Cascade_Classifiers/' 
//     and have the same name as the hero they are meant to detect. 

#include "classifier_detector.h"

const string& CLASSIFIER_DIRECTORY = "Detection_Algorithm/Data/Cascade_Classifiers/";

/**

*/
classifier_detector::classifier_detector(const vector<OWConst::Heroes>& heroesToDetect) {
    this->classifiers = vector<CascadeClassifier>();
    this->classifierHeroes = vector<OWConst::Heroes>();
    // If no specific heroes are provided, load all classifiers available
    if (heroesToDetect.at(0) != OWConst::No_Hero) {
        // Iterate through all files in the classifier directory
        for (const auto& file : filesystem::directory_iterator(CLASSIFIER_DIRECTORY)) {
            // Save the filepath, then convert it to a string
            filesystem::path filePath(file);
            string filePathString = filePath.generic_string();
            CascadeClassifier heroClassifier;

            // Load the classifier for a given file, and push it to the classifiers field
            heroClassifier.load(filePathString);
            this->classifiers.push_back(heroClassifier);
            // Push the corresponding hero to the classifierHeroes field
            this->classifierHeroes.push_back(OWConst::getHero(filePathString));
        }
    }
    // If specific heroes are provided, then only load the classifiers corresponding to those heroes
    else {
        for (int i = 0; i < heroesToDetect.size(); i++) {
            CascadeClassifier heroClassifier;
            string classifierFilePath = CLASSIFIER_DIRECTORY + OWConst::getHeroString(heroesToDetect[i]) + ".xml";

            if (!heroClassifier.load(classifierFilePath)) {
                //throw invalid_argument("No available classifier for given hero");
                cout << "ERROR!: Cannot load classifier for " << OWConst::getHeroString(heroesToDetect[i]) << endl;
                break;
            }
            else {
                this->classifiers.push_back(heroClassifier);
                this->classifierHeroes.push_back(heroesToDetect[i]);
            }
        }
    }
}


/**

*/
OWConst::Heroes classifier_detector::identifyHero(const Mat& image) {

    for (int i = 0; i < this->classifiers.size(); i++) {
        if (detect(image, this->classifiers.at(i))) {
            return this->classifierHeroes.at(i);
        }
    }

    return OWConst::No_Hero;
}

/**

*/
bool classifier_detector::evaluateClassifier(const Mat& image, const OWConst::Heroes& knownHero) {
    OWConst::Heroes detectedHero = OWConst::No_Hero;

    for (int i = 0; i < this->classifiers.size(); i++) {
        if (detect(image, this->classifiers.at(i))) {
            detectedHero = this->classifierHeroes.at(i);
            return detectedHero == knownHero; 
        }
    }

    return false;
}



bool classifier_detector::detect(const Mat& image, CascadeClassifier& classifier) {
    // Crop the source image so it only looks at bottom right quarter of screen
    Rect newSize = Rect(image.cols / 2, image.rows / 2, image.cols / 2, image.rows / 2);
    Mat croppedImage = croppedImage(newSize);

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