# Training Cascade Classifiers for Basic Object Detection in OpenCV
## Overview
Object detection can be achieved using feature-based cascade classifiers, as demonstrated by Viola & Jones in their paper "Rapid Object Detection using a Boosted Cascade of Simple Features. Doing this consists of two steps:

1. Training a classifier based on positive and negative images.
2. Using the trained classifier to identify objects within a test image using a cascading algorithm. 

This tutorial will cover the first step, and the tutorial "Object_Detection_Using_Cascade_Classifiers" will cover the second.

## References
This tutorial covers the steps necessary to train samples, but does not describe the reasoning behind the process itself. A brief description can be found in the [OpenCV docs for object detection](https://docs.opencv.org/3.4/db/d28/tutorial_cascade_classifier.html), which this tutorial is based on, and further detail can be found in the Viola & Jones paper mentioned above. 

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
For the purposes of this tutorial, I downloaded OpenCV version 3.4.14 from [the release page here](https://opencv.org/releases/ "OpenCV Release Versions"). The necessary files were found in:
>`{Download_Folder}/opencv/build/x64/vc15/bin}`

I copied the following files into my local build folder under `{Local_OpenCV_Directory}/x64/vc16/bin/`:
- opencv_createsamples.exe
- opencv_world3414.dll
- opencv_traincascade.exe

## Setting up training data and directory structure
### Positive Images
Positive training images must contain the desired object to be detected, and multiple objects can be present in the same image. Images should be stored in their own directory, with a separate text file containing the filepaths to each image on their own line. In this tutorial, the directory is set as follows:

    Data/
      Positive_Images/
        pos1.jpg
        pos2.jpg
        pos3.jpg
        etc.
    positive_paths.txt

In this example, "positive_paths.txt" contains the relative file paths to each image on its own line. Thus, we can expect the file to look like:
>Data/Positive_Images/pos1.jpg <br>
Data/Positive_Images/pos2.jpg <br>
Data/Positive_Images/pos3.jpg <br>
etc.

### Negative Images
Negative training images must not contain any instances of the desired object to be detected, and the directory should be set up in the same manner as positive images. For this tutorial, the directory is as follows: 

    Data/
      Negative_Images/
        neg1.jpg
        neg2.jpg
        neg3.jpg
        etc.
    negative_paths.txt

Once again, "negative_paths.txt" contains relative file paths to every image on its own line.

## Bounding the Positive Samples
In order to identify the objects present within each positive image, the opencv_annotation tool is used. This tool will allow visual selection of the bounding box which contains objects in each positive image. To use it, run the following command:

>opencv_annotation --annotations=<path_to_annotation_file> --images=<path_to_positive_images_folder>

The tool will open a new window and allow the user to drag-select boxes around each instance of the desired object. The following key commands can be used within the annotation window:

- Pressing 'c': Confirm the annotation, turning the annotation green and confirming it is stored
- Pressing 'd': Delete the last annotation from the list of annotations (easy for removing wrong annotations)
- Pressing 'n': Continue to the next image
- Pressing 'ESC': Exit the annotation software

After running, the tool will create a file in the location specified via the --annotations flag. This file will contain the file paths to each image, followed by the x and y coordinates for the top-left corner of the bounding box, and the width and height of the bounding box created. 

In this tutorial, the command run is: 
> `opencv_annotation --annotations=positive_annotations.txt --images=Data/Positive_Samples/`

## Creating Positive Samples
Once bounding boxes have been identified for each positive sample, the opencv_createsamples tool is used to pull samples from within these boxes. 

Creating samples can be done in two different ways:
1. Using a single positive image to produce many different samples via the OpenCV tool.
2. Using many different positive images, each with bounding boxes surrounding every instance of the desired object, to positive samples. 

The first method is useful for static, fixed objects, but the second is much more robust. This tutorial will solely utilize the second method. <br>
The opencv_createsamples tool will require the following arguments:

- -info <path_to_annotation_text_file>
  - This is the file created after using opencv_annotation in the prior step
- -vec <path_to_output_vector_file>
  - This is the output file which will contain all identified positive samples
- -w <sample_width>
  - This will specify the width in pixels that samples should be resized towards
- -h <sample_height>
  - This will specify the height in pixels that samples should be resized towards

In this tutorial, the command that was run was:

>// TODO: ADD COMMAND HERE

After running, an output file will be created in the path specified by the -vec argument containing vector objects of the specified width and height for each positive sample. This file can be used in the next step.

## Training the Cascade Classifier
The final step of the training process is to use the positive and negative samples to train a boosted cascade of classifiers. This is done using the opencv_traincascade tool. The tool offers many different flags that can be used to tune the training process, all of which can be found [here](https://docs.opencv.org/3.4/dc/d88/tutorial_traincascade.html). The following arguments are necessary, and used in this tutorial:

- -data <cascade_directory_name>
  - This specifies the output directory for the final cascade.xml file
  - **Note!** This directory must be created before running the command
- -vec <positive_vector_file>
  - This is the vector file created using opencv_createsamples in the previous step
- -bg <negative_images_paths_text_file>
- -numPos <number_of_positive_samples>
- -numNeg <number_of_negative_samples>
- -numStages <number_of_cascade_stages>
  - The best number of stages to use will vary on a case-by-case basis. Experimentation and tuning is required to determine how many stages to use. 
- -w <sample_width>
  - Must match the width specified in the previous step
- -h <sample_height>
  - Must match the height specified in the previous step

In this tutorial, the command that was run was:

>// TODO: ADD COMMAND HERE

After running, the folder specified by the -data flag will be populated with files. The "cascade.xml" file is necessary for object detection, and all others can be deleted (they are used in case training fails).