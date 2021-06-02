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

const String INPUT_FILE = "Detection_Algorithm/Data/Static_Test_Im/light.jpg";
const String TEMPLATE_FILE = "Detection_Algorithm/Data/Static_Test_Im/template.jpg";
const int THRESHOLD = 100;
Mat input_gray;
Mat template_gray;
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
* 
* 
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
    cout << "Total Distance = " << maxDistAB << endl;
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
*   MAIN Method
*************************************************************************************************/
int main(int argc, char** argv)
{
    //Read INPUT file and TEMPLATE file from source
    Mat input = imread(INPUT_FILE);
    Mat templt = imread(TEMPLATE_FILE);
    if (input.empty() || templt.empty() )
    {
        cout << "Could not open or find the image!\n" << endl;
        cout << "Usage: " << argv[0] << " <Input image>" << endl;
        return -1;
    }
    cout << "Template and input file loaded successfully." << endl;

    cout << "Creating edge maps..." << endl;

    //Create edge map of TEMPLATE 
    cvtColor(templt, template_gray, COLOR_BGR2GRAY);
    blur(template_gray, template_gray, Size(3, 3));
    Mat template_canny;
    Canny(template_gray, template_canny, THRESHOLD, THRESHOLD * 2);
    vector<vector<Point> > template_contours;
    findContours(template_canny, template_contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
    /*Mat template_drawing = Mat::zeros(template_canny.size(), CV_8UC3);
    for (size_t i = 0; i < template_contours.size(); i++)
    {
        Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
        drawContours(template_drawing, template_contours, (int)i, color);
    }
    cout << "Template image edge map created." << endl;
    imshow("Template", template_drawing);
    waitKey();*/

    //Create edge map of INPUT 
    cvtColor(input, input_gray, COLOR_BGR2GRAY);
    blur(input_gray, input_gray, Size(3, 3));
    Mat input_canny;
    Canny(input_gray, input_canny, THRESHOLD, THRESHOLD * 2);
    vector<vector<Point> > input_contours;
    findContours(input_canny, input_contours, RETR_LIST, CHAIN_APPROX_SIMPLE); 
    /*Mat input_drawing = Mat::zeros(input_canny.size(), CV_8UC3);
    for (size_t i = 0; i < input_contours.size(); i++)
    {
        Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
        drawContours(input_drawing, input_contours, (int)i, color);
    }
    cout << "Input image edge map created." << endl;
    imshow("Input", template_drawing);
    waitKey();*/
  
    cout << "Pooling template image points..." << endl;
    vector<Point> template_points_pool = {}; 
    for (int i = 0; i < template_contours.size(); i++) {
        template_points_pool.insert(template_points_pool.end(), template_contours[i].begin(), template_contours[i].end());
    }

    cout << "Pooling input image points..." << endl;
    vector<Point> input_points_pool = {};
    for (int i = 0; i < input_contours.size(); i++) {
        input_points_pool.insert(input_points_pool.end(), input_contours[i].begin(), input_contours[i].end());
    }

    cout << "Calculating Hausdorff Distance with internal method..." << endl;
    chrono::steady_clock::time_point start = chrono::steady_clock::now();
    double distance_hausdorff_internal = distance_hausdorff(template_points_pool, input_points_pool);
    chrono::steady_clock::time_point end = chrono::steady_clock::now();

    cout << "[Elapsed time in milliseconds: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms]" << endl;
    waitKey();

    return 0;
}