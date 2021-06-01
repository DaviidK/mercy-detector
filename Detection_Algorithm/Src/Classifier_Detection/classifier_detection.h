
#ifndef CLASSIFIER_DETECTION_H
#define CLASSIFIER_DETECTION_H

#include "opencv2/objdetect.hpp"
#include "opencv2/imgproc.hpp"
#include <vector>

using namespace cv;
using namespace std;

class classifier_detection {

public:
    void identifyHero(vector<CascadeClassifier> classifier, Mat image);

};

#endif