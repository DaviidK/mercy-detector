/***************************************************************************************************
* Image processer tool - Edge Map
*
* @author: Irene Wachirawutthichai
* @date: 06/03/2021
*
* This collection of method(s) is a general tool for generating edge maps
*
***************************************************************************************************/

#include "edge_processing.h"

const int CANNY_THRESHOLD_1 = 100;
const int CANNY_THRESHOLD_2 = 200;
const Size KERNEL_SIZE = Size(3, 3);


/***************************************************************************************************
* Create Edge Map
*
* This method takes an input image, creates an edge map of the same size, and
* returns the edge map as a new Mat Object
*
* @param    Mat& input_image  : Mat object of input image, to be used as source of edge map 
*
* @pre      Input Mat received is valid and not null 
* @post     Edge map is created based on the input image and returned as a new object
* 
* @return   Edge map of input image as Mat object 
* 
***************************************************************************************************/
Mat createEdgeMap(Mat& input_image) {
    //create and return edge map of input image
    Mat image_gray, image_canny;
    cvtColor(input_image, image_gray, COLOR_BGR2GRAY);
    blur(image_gray, image_gray, KERNEL_SIZE);
    Canny(image_gray, image_canny, CANNY_THRESHOLD_1, CANNY_THRESHOLD_2 * 2);
    vector<vector<Point> > contours;
    findContours(image_canny, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

    //setup a black Mat to draw contours
    Mat edge_map = Mat::zeros(image_canny.size(), CV_8UC3);
    for (size_t i = 0; i < contours.size(); i++)
    {
        Scalar color = Scalar(255, 255, 255);   //white
        drawContours(edge_map, contours, (int)i, color);
    }

    return edge_map;
}