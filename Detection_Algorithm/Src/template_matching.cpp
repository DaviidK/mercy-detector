#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

bool use_mask;
Mat img; Mat templ; Mat mask; Mat result;
const char* image_window = "Source Image";
const char* result_window = "Result window";
const char* image_name = "C:/Users/12068/Documents/GitHub/mercy-detector/Detection_Algorithm/Data/Static_Test_Im/busy.jpg";
const char* template_name = "C:/Users/12068/Documents/GitHub/mercy-detector/Detection_Algorithm/Data/Static_Test_Im/template.jpg";
int match_method;
int max_Trackbar = 5;

void MatchingMethod(int, void*);

int main(int argc, char** argv) {
    const char* partialPath = "Detection_Algorithm/Data/Static_Test_Im/busy.jpg";
    char full[_MAX_PATH];
    if (_fullpath(full, partialPath, _MAX_PATH) != NULL)
        printf("Full path is: %s\n", full);
    else
        printf("Invalid path\n");
    
    img = imread(image_name, IMREAD_COLOR);
    templ = imread(template_name, IMREAD_COLOR);

    if (img.empty() || templ.empty())   {
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
    int result_cols = img.cols - templ.cols + 1;
    int result_rows = img.rows - templ.rows + 1;
    result.create(result_rows, result_cols, CV_32FC1);
    bool method_accepts_mask = (TM_SQDIFF == match_method || match_method == TM_CCORR_NORMED);

    matchTemplate(img, templ, result, match_method);

    normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
    double minVal; double maxVal; Point minLoc; Point maxLoc;
    Point matchLoc;
    minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

    if (match_method == TM_SQDIFF || match_method == TM_SQDIFF_NORMED) {
        matchLoc = minLoc;
    } else {
        matchLoc = maxLoc;
    }

    rectangle(img_display, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
    rectangle(result, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
    imshow(image_window, img_display);
    imshow(result_window, result);

    return;
}