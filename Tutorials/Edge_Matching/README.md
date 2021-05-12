# Object classification using Contours and Convex Hull in OpenCV 
We have investigated the two most common detection approaches in computer vision, which are `cascade classifiers` and `template matching` to detect the hero specific weapon. In addition to applying the two approaches to our project, this tutorial aims to investigate a case specific object detection using the combination of contours and convex hulls to create an alternative detector. 

## Ideation 
- Matching a general shape to the shape of a hero specific weapon
- Ideally would ignore fine details (i.e. small convex and concave) on the object 
- Detect similar shapes even when the shape is obscured or slightly tranformed

## Libraries 
Assuming OpenCV has already been built from source. Include the following libraries: 
```c++
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
```

# References
- https://hub.packtpub.com/opencv-detecting-edges-lines-shapes/
- https://docs.opencv.org/3.4/df/d0d/tutorial_find_contours.html
- https://docs.opencv.org/3.4/d7/d1d/tutorial_hull.html
- https://docs.opencv.org/3.4/d3/dc0/group__imgproc__shape.html
