/************************************************************************************************
* Edge Matching with Haudorff Distance
*
* @author: Irene Wachirawutthichai
* @date: May 25 2021
*
*
* references:
* - https://programmersought.com/article/14824834608/
* - https://stackoverflow.com/questions/21482534/how-to-use-shape-distance-and-common-interfaces-to-find-hausdorff-distance-in-op
* - https://titanwolf.org/Network/Articles/Article?AID=813ecc86-ae22-4844-bd2b-0e516f9d15ce#gsc.tab=0
*
*
************************************************************************************************/

#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/shape/shape_distance.hpp>
#include <iostream>
#include <chrono>

using namespace cv;
using namespace std;

const String INPUT_FILE = "Detection_Algorithm/Data/Static_Test_Im/template.jpg";
const String TEMPLATE_FILE = "Detection_Algorithm/Data/Static_Test_Im/template.jpg";
const int THRESHOLD = 100;
RNG rng(98765);


/************************************************************************************************
*   displayImage
*   Helper to display image with a scale factor parameter for resizing
*
************************************************************************************************/
void displayImage(Mat image, String windowName, int scaleFactor) {
    namedWindow(windowName, WINDOW_NORMAL);
    resizeWindow(windowName, image.cols / scaleFactor, image.rows / scaleFactor);
    imshow(windowName, image);
}


/************************************************************************************************
*   distanceFromTo
*   internal helper for HAUSDORFF DISTANCE
************************************************************************************************/
int distanceFromTo(const vector<Point>& a, const vector<Point>& b)
{
    int maxDistAB = 0;
    for (size_t i = 0; i < a.size(); i++) {
        int minB = 1000000;
        for (size_t j = 0; j < b.size(); j++) {
            int dx = (a[i].x - b[j].x);
            int dy = (a[i].y - b[j].y);
            int tmpDist = dx * dx + dy * dy;
            if (tmpDist < minB) {
                minB = tmpDist;
            }
            if (tmpDist == 0) {
                break; // can't get better than equal.
            }
        }
        maxDistAB += minB;
    }
    cout << "(Total Distance) = " << maxDistAB << endl;
    return maxDistAB;
}


/************************************************************************************************
*   distance_hausdorff
*************************************************************************************************/
double distance_hausdorff(const vector<Point>& a, const vector<Point>& b)
{
    int maxDistAB = distanceFromTo(a, b);
    int maxDistBA = distanceFromTo(b, a);
    int maxDist = max(maxDistAB, maxDistBA);
    double result = sqrt((double)maxDist);

    cout << "Hausdorff distance = " << result << endl;

    return result;
}


/************************************************************************************************
*   showContours
*   draws contours on an empty image, has option to use random colors (false for white)
*************************************************************************************************/
bool showContours(Mat image_canny, vector<vector<Point>> contours, bool randomColors) {
    Mat image_drawing = Mat::zeros(image_canny.size(), CV_8UC3);

    for (size_t i = 0; i < contours.size(); i++)
    {
        if (randomColors) {
            Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
            drawContours(image_drawing, contours, (int)i, color);
        } else { 
            //white contours
            drawContours(image_drawing, contours, (int)i, Scalar(255, 255, 255));
        }
    }
    cout << "Edge map created." << endl;
    imshow("Edge Map", image_drawing);
    waitKey();

    return true;
}


/************************************************************************************************
*   createEdgeMap
*   create edge map of image passed into it, has option to show drawn image
*************************************************************************************************/
vector<vector<Point>> createEdgeMap(Mat image, bool showImage, bool randomColors) {
    //Create edge map of image
    Mat image_gray, image_canny;

    cvtColor(image, image_gray, COLOR_BGR2GRAY);
    blur(image_gray, image_gray, Size(3, 3));
    Canny(image_gray, image_canny, THRESHOLD, THRESHOLD * 2);
    vector<vector<Point> > contours;
    findContours(image_canny, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

    if (showImage) {
        showContours(image_canny, contours, randomColors);
    }

    return contours;
}


/************************************************************************************************
*   poolPoints
*   pull all points in a collection of vectors into one pool of points 
*************************************************************************************************/
vector<Point> poolPoints(vector<vector<Point>> contours) {
    vector<Point> points_pool = {};
    for (int i = 0; i < contours.size(); i++) {
        points_pool.insert(points_pool.end(), contours[i].begin(), contours[i].end());
    }
    return points_pool;
}


/************************************************************************************************
*   MAIN Method
*************************************************************************************************/
int main(int argc, char** argv)
{
    //Read INPUT file and TEMPLATE file from source
    Mat input = imread(INPUT_FILE);
    Mat templt = imread(TEMPLATE_FILE);
    if (input.empty() || templt.empty())
    {
        cout << "Could not open or find the image!\n" << endl;
        cout << "Usage: " << argv[0] << " <Input image>" << endl;
        return -1;
    }
    cout << "Template and input file loaded successfully." << endl;

    //Create edge maps for image
    cout << "Creating edge maps..." << endl;
    vector<vector<Point>> template_contours = createEdgeMap(templt, true, false);
    vector<vector<Point>> input_contours = createEdgeMap(input, true, false);

    //Pool points from contours for hausdorff distance calculations, start timer
    cout << "Calculating Hausdorff Distance with internal method... (timer start!)" << endl;
    cout << "Pooling image points..." << endl;
    chrono::steady_clock::time_point start = chrono::steady_clock::now();
    vector<Point> template_points_pool = poolPoints(template_contours);
    vector<Point> input_points_pool = poolPoints(input_contours);

    //Calculate Hausdorff Distance, records time 
    double distance_hausdorff_internal = distance_hausdorff(template_points_pool, input_points_pool);
    
    //Stop timer and show elapsed time
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    int time = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "[Hausdorff Distance caluclation time: " << time << " ms]" << endl;

    waitKey(5000); //wait for 5 sec

    return 0;
}