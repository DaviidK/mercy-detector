// ----------------------------------classifier_detection.cpp--------------------------------------
// Author: David Kang
// Last modified: 05/31/21
// ------------------------------------------------------------------------------------------------
// Purpose: 
// ------------------------------------------------------------------------------------------------
// Assumptions:
//   - Trained cascade classifiers are stored in 'Detection_Algorithm/Data/Cascade_Classifiers/' 
//     and have the same name as the hero they are meant to detect. 



#include "classifier_detection.h"

string CLASSIFIER_DIRECTORY = "Detection_Algorithm/Data/Cascade_Classifiers/";

/**

*/
void classifier_detection::cascadeClassifierSetup(const vector<OWConst::Heroes>& heroesToDetect) {
    // If no specific heroes are provided, load all classifiers available
    if (heroesToDetect.at(0) != OWConst::No_Hero) {
        for (const auto& file : std::filesystem::directory_iterator(CLASSIFIER_DIRECTORY)) {
            std::string filepath = std::filesystem::path_string(file);
            CascadeClassifier heroClassifier;
            // NOT A STRING
            heroClassifier.load(file);
            this->classifiers->push_back(heroClassifier);
            this->classifierHeroes->push_back(fileString.getHero());
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
                this->classifiers->push_back(heroClassifier);
                this->classifierHeroes->push_back();
            }
        }
    }
}


/**

*/
void classifier_detection::evaluateClassifier(vector<CascadeClassifier> classifier, Mat image, vector<OWConst::Heroes> knownHero) {
    Mat frame_gray;
    cvtColor(image, frame_gray, COLOR_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);

    // Detect if a hero can be detected in the screenshot 
    std::vector<Rect> heroOccurrences;
    classifier.detectMultiScale(frame_gray, heroOccurrences);

	if (heroOccurrences.size() != 0) {
		return true;
	}

}