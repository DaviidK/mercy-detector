# Object Detection Using Cascade Classifiers
## Overview
This tutorial contains code which uses a trained cascade classifier to detect instances of an object in still images. The code will display a series of images to the user, drawing an ellipse around each instance of the desired object to be detected.

## References
The code in this tutorial will perform object detection, but does not cover training the classifier itself. The instructions for training can be [found here](https://github.com/DaviidK/mercy-detector/tree/main/Tutorials/Training_Classifiers "Training_Classifiers Tutorial"), in a separate tutorial. Sample code for this object detector was borrowed from Ana Huamán, in her article on object detection [found here](https://docs.opencv.org/3.4/db/d28/tutorial_cascade_classifier.html "OpenCV Docs").

Display settings for all test images in this tutorial were the same as listed in `mercy-detector/Documentation/overwatch_settings.md`

## Necessary Tools
Prior to running this code, a trained cascade classifier must have been created and stored in the repository referenced by the constant `MERCY_CLASSIFIER_LOCATION`.