/*
 * Template Matching 
 * This program uses the opencv library's template matching function to find a location in 
 * an image where a template image is most likely located.
 */

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat img; Mat templ1; Mat templ2; Mat result1; Mat result2;
const char* image_window = "Source Image";
const char* result_window = "Result window";

// Source image
const char* image_name = "Detection_Algorithm/Data/Static_Test_Im/busy.jpg"; 
// Template 1 - True template (does exist in source)
const char* template_name1 = "Detection_Algorithm/Data/Static_Test_Im/template.jpg";
// Template 2 - False template (does not exist in source)
const char* template_name2 = "Detection_Algorithm/Data/Static_Test_Im/soldier.jpg";

int match_method;
int max_Trackbar = 5;

/*
 * MatchingMethod()
 * Calls the matchTemplate() method for both templates and draws a rectangle on the shown window
 * of where the highest score for the matching was. The black rectangle shows the true sample match
 * while the blue rectangle shows the false sample match.
 * 
 * pre: global Mat variables must be of valid images 
 * post: displays the source image and a rectangle is drawn on of where the template had the highest 
 *       match. also prints out the scores of the matches for both templates.
 */
void MatchingMethod(int, void*);

int main(int argc, char** argv) {
    img = imread(image_name, IMREAD_COLOR);
    templ1 = imread(template_name1, IMREAD_COLOR);
    templ2 = imread(template_name2, IMREAD_COLOR);

    if (img.empty() || templ1.empty() || templ2.empty())   {
        cout << "Can't read one of the images" << endl;
        return EXIT_FAILURE;
    }

    namedWindow(image_window, WINDOW_AUTOSIZE);
    namedWindow(result_window, WINDOW_AUTOSIZE);

    const char* trackbar_label = "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
    createTrackbar(trackbar_label, image_window, &match_method, max_Trackbar, MatchingMethod);
    MatchingMethod(0, 0);
    waitKey(0);

    return EXIT_SUCCESS;
}

void MatchingMethod(int, void*) {
    Mat img_display;
    img.copyTo(img_display);
                                // Removed the following commented code in order to try to make the 
                                // result the same size as source but it didn't work.
    int result_cols = img.cols; // - templ.cols + 1 
    int result_rows = img.rows; // - templ.rows + 1
    result1.create(result_rows, result_cols, COLOR_BGR2GRAY); // , CV_32FC1
    result2.create(result_rows, result_cols, COLOR_BGR2GRAY);
    matchTemplate(img, templ1, result1, match_method);
    matchTemplate(img, templ2, result2, match_method);

    // Commented out so the scores can be compared between different templates
    // normalize(result1, result1, 0, 1, NORM_MINMAX, -1, Mat());
    double minVal1; double maxVal1; Point minLoc1; Point maxLoc1;
    Point matchLoc1;
    minMaxLoc(result1, &minVal1, &maxVal1, &minLoc1, &maxLoc1, Mat());   

    if (match_method == TM_SQDIFF || match_method == TM_SQDIFF_NORMED) {
        cout << "Mercy Min value: " << minVal1 << endl;
        matchLoc1 = minLoc1;
    } else {
        cout << "Mercy Max value: " << maxVal1 << endl;
        matchLoc1 = maxLoc1;
    }

    rectangle(img_display, matchLoc1, Point(matchLoc1.x + templ1.cols, matchLoc1.y + templ1.rows), Scalar::all(0), 2, 8, 0);
    rectangle(result1, matchLoc1, Point(matchLoc1.x + templ1.cols, matchLoc1.y + templ1.rows), Scalar::all(0), 2, 8, 0);
    imshow(image_window, img_display);
    imshow(result_window, result1);

    // Commented out so the scores can be compared between different templates
    // normalize(result2, result2, 0, 1, NORM_MINMAX, -1, Mat());
    double minVal2; double maxVal2; Point minLoc2; Point maxLoc2;
    Point matchLoc2;
    minMaxLoc(result2, &minVal2, &maxVal2, &minLoc2, &maxLoc2, Mat());

    if (match_method == TM_SQDIFF || match_method == TM_SQDIFF_NORMED) {
        cout << "Soldier Min value: " << minVal2 << endl;
        matchLoc2 = minLoc2;
    }
    else {
        cout << "Soldier Max value: " << maxVal2 << endl;
        matchLoc2 = maxLoc2;
    }

    rectangle(img_display, matchLoc2, Point(matchLoc2.x + templ2.cols, matchLoc2.y + templ2.rows), Scalar(255, 0, 0), 2, 8, 0);
    rectangle(result1, matchLoc2, Point(matchLoc2.x + templ2.cols, matchLoc2.y + templ2.rows), Scalar(255, 0, 0), 2, 8, 0);
    imshow(image_window, img_display);
    imshow(result_window, result2);


    return;
}