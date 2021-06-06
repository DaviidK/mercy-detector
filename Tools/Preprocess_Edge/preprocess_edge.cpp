/************************************************************************************************
* Image Preprocesser Tool - Edge Map
*
* @author: Irene Wachirawutthichai
* @date: 06/03/2021
* 
* This method takes an input image, creates an edge map of the same size, and 
* returns the edge map as a Mat Object
*
************************************************************************************************/

#include "preprocess_edge.h"

const int THRESHOLD = 100;


/************************************************************************************************
* createEdgeMap
*   
* This method takes an input image, creates an edge map of the same size, and 
* returns the edge map as a Mat Object
* 
* @params
*       Mat& input_image:   Source image to create an edge map of 
* 
*************************************************************************************************/
Mat createEdgeMap(Mat& input_image) {
    //create and return edge map of input image
    Mat image_gray, image_canny;
    cvtColor(input_image, image_gray, COLOR_BGR2GRAY);
    blur(image_gray, image_gray, Size(3, 3));
    Canny(image_gray, image_canny, THRESHOLD, THRESHOLD * 2);
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