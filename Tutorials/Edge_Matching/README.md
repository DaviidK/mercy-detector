# Detection using Contours and Convex Hull in OpenCV

We have investigated the two most common detection approaches in computer vision, which are `cascade classifiers` and `template matching` to detect the hero specific weapon. In addition to applying the two approaches to our project, this tutorial aims to investigate a case specific object detection using the combination of contours and convex hulls to create an alternative detector. 

## Ideation 
- The target object has a fixed general location, viewing angle, minimal vertical and horizontal translation, and minute to no rotation
- Input will be 2D images of 3D rendered scenes, so little to no complications are expected in edge detection
- Ideally would ignore changes in fine details (i.e. small convex and concave) on the object 

## Libraries used
Assuming OpenCV has already been built from source. Include the following libraries: 
```c++
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
```

## Conclusion
We believe the hyper-realistic characteristics in 3D rendered scenes, such as overly high illumination, clarity of all objects (both nearby and distant), and limited textures may have contributed to difficulties in accurately perceiving field of depth. Thus, ...(con't)

### References
- https://hub.packtpub.com/opencv-detecting-edges-lines-shapes/
- https://docs.opencv.org/3.4/df/d0d/tutorial_find_contours.html
- https://docs.opencv.org/3.4/d7/d1d/tutorial_hull.html
- https://docs.opencv.org/3.4/d3/dc0/group__imgproc__shape.html
