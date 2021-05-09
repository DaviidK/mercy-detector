# Training Cascade Classifiers for Basic Object Detection in OpenCV
## Overview
Object detection can be achieved using feature-based cascade classifiers, as demonstrated by Viola & Jones in their paper "Rapid Object Detection using a Boosted Cascade of Simple Features. Doing this consists of two steps:

1. Training a classifier based on positive and negative images.
2. Using the trained classifier to identify objects within a test image using a cascading algorithm. 

This tutorial will cover the first step, and the tutorial "Object_Detection_Using_Cascade_Classifiers" will cover the second.

## References
This tutorial covers the steps necessary to train samples, but does not describe the reasoning behind the process itself. A brief description can be found in the [OpenCV docs for object detection](https://docs.opencv.org/3.4/db/d28/tutorial_cascade_classifier.html), and further detail can be found in the Viola & Jones paper mentioned above. 

Note that in this tutorial, "images" refers to the image files themselves, and "samples" will refer to the objects within those images.

## Necessary Tools
 OpenCV provides command-line tools which can train a classifier if given the requisite data. The three tools covered in this tutorial are:
- opencv_annotation
  - A visual interface used to draw bounding boxes on objects inside a positive training image.
- opencv_createsamples
  - Will create positive samples of the desired object out of positive training images and the coordinates for a bounding box surrounding objects inside the images. 
- opencv_traincascade
  - Will perform the training of the boosted cascade of classifiers using the samples created from opencv_createsamples

### **Important Note!**
Both opencv_createsamples and opencv_traincascade were not ported to 4.X versions of the application. Thus, to use them you will need to find the corresponding .exe and .dll from a 3.X version of OpenCV.<br>
For the purposes of this tutorial, I downloaded OpenCV version 3.4.14 from [the release page here](https://opencv.org/releases/). The necessary files were found in:
>`{Download_Folder}/opencv/build/x64/vc15/bin}`

I copied the following files into my local build folder under `{Local_OpenCV_Directory}/x64/vc16/bin/`:
- opencv_createsamples.exe
- opencv_world3414.dll
- opencv_traincascade.exe

