# Tools

Files in this directory are tools to be used with the variety of components being built in this repository.

## Video Matching Evaluation `video_matching_eval.cpp`

This program helps to evaluate the success rate of the object detection methods. Each detection method will need to have its own process video method that takes the following parameters:

* `VideoCapture capture` - The VideoCapture of the video being analyzed.
* `OWConst::Heroes expectedHero` - The expected hero to be detected in the specified video.
* `vector<vector<string>> output` - The output 2D vector to be written as a CSV file at the end of the main method.

Some detection type-specific variables may needed to be added.
