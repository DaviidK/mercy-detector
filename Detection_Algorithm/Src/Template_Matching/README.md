# Template Matching 

[Template matching](https://docs.opencv.org/3.4/de/da9/tutorial_template_matching.html) is a matching technique that searches through an entire source image for a section that matches a given template image. The OpenCV library includes the `matchTemplate()` method that does this. 

## OpenCV's `matchTemplate()`

Parameters:
**InputArray** 	 image,
**InputArray** 	 templ,
**OutputArray**  result,
**int** 	     method,
**InputArray** 	 mask = noArray()

Within the configurations, there are 6 total matching methods to choose from, these calculate the level of matching differently, thus producing different results. There is also an optional parameter for the mask image, which determines regions of the template image that can be ignored when making comparisons to the source image.

Below are the available matching methods:
* TM_SQDIFF*
* TM_SQDIFF_NORMED
* TM_CCORR
* TM_CCORR_NORMED*
* TM_CCOEFF
* TM_CCOEFF_NORMED

*Accepts mask images

## Application
For this project, template matching was used to detect the weapon that was being held by the hero. When detecting the hero, the template image is an image of the weapon that is held by the hero when idle. When detecting the weapon action, multiple images that depict the particular action were chosen as the template image. The mask image is of each template image with the background removed; this was done using [OpenCV's grabcut](https://docs.opencv.org/3.4/d8/d34/samples_2cpp_2grabcut_8cpp-example.html) algorithm.

![](https://raw.githubusercontent.com/DaviidK/mercy-detector/main/Documentation/Images/template_matching.PNG)

For this project, we found that the TM_CCORR_NORMED matching method with a mask image worked most effectively, working 80.37% of the time. Therefore it was chosen as the final configuration options for the detection algorithm.  
