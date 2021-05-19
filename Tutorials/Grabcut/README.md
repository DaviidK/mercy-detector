# Grabcut Masks

2 of the 6 template matching methods allow for an optional mask image parameter. The masks image define regions of the template image which should be viewed or ignored. Any pixels that are black will be pixels on the template image to ignore, whilst every other color pixel will be considered when comparing the template image to the source image.

Because of how the mask image works, I decided it would be best to prepare the mask image by taking a grabcut of the template image.

## grabcut.cpp

This program is taken from the [grabcut tutorial](https://docs.opencv.org/3.4/d8/d34/samples_2cpp_2grabcut_8cpp-example.html) on openCV documentation. It provides an interactive UI to create and fine-tune the grabcut. Ensure that the image you are including into this grabcut program is a png so there is no loss in detail when saving.

## grabcut_to_mask.cpp

This program converts a grabcut image to a monochrome image where the background is set to black and the rest is set to white.

## template_matching_wmask.cpp

This program coordinates the template matching with the methods that accept a mask image. Only methods 0 and 3 accept mask images, so ensure that the global constant MATCH_METHOD is set to either of those two methods.
