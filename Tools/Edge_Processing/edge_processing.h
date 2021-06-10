/***************************************************************************************************
* Image processer tool - Edge Map
*
* @author: Irene Wachirawutthichai
* @date: 06/03/2021
*
* General tool for generating edge maps
*
***************************************************************************************************/

#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/shape/shape_distance.hpp>
#include <iostream>

using namespace cv;
using namespace std;


/***************************************************************************************************
* Create Edge Map
*
* This method takes an image as an input, then creates an edge map of the same size and
* returns the edge map as a new object
*
* @param    Mat& input_image  : Mat object of input image
*
* @pre      Input Mat received is valid
* @post     Edge map is created based on the input image and returned
*
* @return   Edge map of input image as Mat object
*
***************************************************************************************************/
Mat createEdgeMap(Mat& input_image);
