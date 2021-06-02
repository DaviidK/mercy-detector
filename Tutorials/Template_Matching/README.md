# Template Matching

Template Matching is a methodology implemented in OpenCV that helps find a specified template
image in a source image.

## OpenCV matchTemplate()

Parameters:
**InputArray** 	 image,
**InputArray** 	 templ,
**OutputArray**  result,
**int** 	     method,
**InputArray** 	 mask = noArray()

The matchTemplate method takes in a source image and a template image and outputs the scores of
how well the template image matched each location in the source image by outputting a resulting
Mat that corresponds to those scores.

The function also takes in an int that represents which method of matching to be used. There
are six template matching methods available, as listed below:
* TM_SQDIFF
* TM_SQDIFF_NORMED
* TM_CCORR
* TM_CCORR_NORMED
* TM_CCOEFF
* TM_CCOEFF_NORMED

There is also an optional parameter to include a mask which specifies which regions of the
template should not be compared with the input image.

## template_matching.cpp

This program takes in an image and detects what Overwatch hero can be seen in it. You can change the method that matchTemplate() uses by changing the global constant MATCH_METHOD to a value between 0 and 5. The accepted heroes can be changed by changing the HEROES constant. 

## template_matching_tutorial.cpp

This program is based off of this [OpenCV Tutorial](https://docs.opencv.org/3.4/de/da9/tutorial_template_matching.html)
for template matching.
The current state of this program takes a source image of a screenshot of a Mercy player
gameplay and two template images to map the resulting "match" of where the template image might
exist on the source image.
The two template images are of Mercy's wand (true sample) and Soldier's gun (false sample).

You can change the matching method by using the trackbar that appears above the source image.
With each change to the trackbar, a black rectangle is drawn onto the image to depict where the
match for the true sample is found, and a blue rectangle for the false sample.
In addition to the rectangle's drawn onto the source image, the score for how well the template
matched the patch in the image is printed to the console.
