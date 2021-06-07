#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/shape/shape_distance.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat createEdgeMap(Mat& input_image);
