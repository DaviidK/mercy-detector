// Tutorials for findContours() and convexHull()
// press any key to move to the next test method window

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat src_gray;
int thresh = 100;
RNG rng(12345);
const char* source_window = "Source";

// Helper to display image with a scale factor parameter for resizing
void displayImage(Mat image, String windowName, int scaleFactor) {
    namedWindow(windowName, WINDOW_NORMAL);
    resizeWindow(windowName, image.cols / scaleFactor, image.rows / scaleFactor);
    imshow(windowName, image);
}

void convex_thresh_callback(int, void*)
{
    Mat canny_output;
    Canny(src_gray, canny_output, thresh, thresh * 2);
    vector<vector<Point> > contours;
    findContours(canny_output, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
    vector<vector<Point> >hull(contours.size());
    for (size_t i = 0; i < contours.size(); i++)
    {
        convexHull(contours[i], hull[i]);
    }
    Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
    for (size_t i = 0; i < contours.size(); i++)
    {
        Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
        drawContours(drawing, contours, (int)i, color);
        drawContours(drawing, hull, (int)i, color);
    }
    displayImage(drawing, source_window, 1);
}

void contour_thresh_callback(int, void*)
{
    Mat canny_output;
    Canny(src_gray, canny_output, thresh, thresh * 2);
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
    Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
    for (size_t i = 0; i < contours.size(); i++)
    {
        Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
        drawContours(drawing, contours, (int)i, color, 2, LINE_8, hierarchy, 0);
    }
    displayImage(drawing, source_window, 1);
}



// internal helper for HAUSDORFF DISTANCE
int distance_2(const vector<Point>& a, const vector<Point>& b)
{
    int maxDistAB = 0;
    for (size_t i = 0; i < a.size(); i++)
    {
        int minB = 1000000;
        for (size_t j = 0; j < b.size(); j++)
        {
            int dx = (a[i].x - b[j].x);
            int dy = (a[i].y - b[j].y);
            int tmpDist = dx * dx + dy * dy;

            if (tmpDist < minB)
            {
                minB = tmpDist;
            }
            if (tmpDist == 0)
            {
                break; // can't get better than equal.
            }
        }
        maxDistAB += minB;
    }
    return maxDistAB;
}

double distance_hausdorff(const vector<Point>& a, const vector<Point>& b)
{
    int maxDistAB = distance_2(a, b);
    int maxDistBA = distance_2(b, a);
    int maxDist = max(maxDistAB, maxDistBA);
    double result = sqrt((double)maxDist);

    cout << "Hausdorff distance = " << result << endl;

    return result;
}



int main(int argc, char** argv)
{
    Mat src = imread("busy.jpg");
    if (src.empty())
    {
        cout << "Could not open or find the image!\n" << endl;
        cout << "Usage: " << argv[0] << " <Input image>" << endl;
        return -1;
    }
    cvtColor(src, src_gray, COLOR_BGR2GRAY);
    blur(src_gray, src_gray, Size(3, 3));
    namedWindow(source_window);
    displayImage(src, source_window, 2);
    const int max_thresh = 255;
    createTrackbar("Canny thresh:", source_window, &thresh, max_thresh, contour_thresh_callback);
    contour_thresh_callback(0, 0);
    waitKey();

    createTrackbar("Canny thresh:", source_window, &thresh, max_thresh, convex_thresh_callback);
    convex_thresh_callback(0, 0);
    waitKey();
    destroyWindow(source_window);
    
    return 0;
}