# Object detector based on Cascade Classifiers 
## Overview
This object detector uses the method proposed by Viola & Jones in their paper "Rapid Object Detection using a Boosted Cascade of Simple Features". There are two primary steps in this process:

1. Training a classifier based on positive and negative images.
2. Using the trained classifier to identify objects within a test image using a cascading algorithm. 

Full tutorials for how to train classifiers and then use them to detect objects can be found under: <br>
`mercy-detector/Tutorials/Training_Classifiers/` <br>
`mercy-detector/Tutorials/Object_Detection_Using_Cascade_Classifiers/`

## Data Used
This detector is based on cascade classifiers stored in the following directories: 
>mercy-detector/Data/Cascade_Classifiers/Heroes/ <br>
mercy-detector/Data/Cascade_Classifiers/Weapons/

If a new classifier is trained and added to the above directory, it will be incorporated into any new classifier_detector object that is created.

## Creating a classifier_detector object
By default, all instances of classifier_detectors will load all available classifiers from the data directory for possible detection. This behavior can be configured to only detect specific heroes if desired. To do so, a user will need to pass a vector of OWConst::Heroes (see: `mercy-detector/Detection_Algorithm/Src/Overwatch_Constants`) containing the heroes to be detected during object creation.

## Using classifier_detector objects
Once created, classifier_detector objects can be used to detect Heroes or Weapon Actions. The two methods used for this are:
* identifyHero(const Mat&) <br>
This takes in a target image to scan for matching heroes and returns an OWConst::Heroes value if any is found.
* identifyAction(const Mat&) <br>
This takes in a target image to scan for matching weapon actions and returns an OWConst::WeaponActions value if any is found.

There are also two separate evaluation methods that can be used to determine the effectiveness of the detector by passing in images in which all instances of heroes or weapon actions are known. These will return a boolean value indicating if the detector found the correct hero or weapon action based on the known ground truth. 