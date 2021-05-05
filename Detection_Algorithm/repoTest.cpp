/***************************************************************************************************
 * Repo File Access Test
 *
 * @author Matthew Munson
 * @date 5/3/21
 *
 * Use this to make sure that you can run files in the repository and access our
 * data files using relative paths.
 *
 * Assumptions:
 *
 * - Your working directory should be mercy-detector
 * - Detection_Algorithm/Static_Test_Im/busy.jpg should exist
 *
 **************************************************************************************************/

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

// Different imports depend on whether you're on Windows or Unix
#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif


using namespace std;
using namespace cv;

void displayImage(const Mat& image, const string& windowName, int scale);

string get_current_dir();

/***************************************************************************************************
 * Main Function
 *
 * Simply opens a file from the data directory, saves a copy as output.jpg,
 * and displays the image to the screen.
 *
 * @pre: Detection_Algorithm/Static_Test_Im/busy.jpg exists
 * @post: Copy of
 *
 * @return exit code indicating program status. Zero indicates success.
 **************************************************************************************************/
int main()
{
    cout << "Opening Files" << endl;

    cout << "Working Directory is: " << get_current_dir() << endl;

    string testImage_filename = "Detection_Algorithm/Data/Static_Test_Im/busy.jpg";

    Mat testImage = imread(testImage_filename, IMREAD_COLOR);

    if(testImage.empty())
    {
        cout << "Image could not be loaded!" << endl;
    }
    else
    {
        imwrite("Detection_Algorithm/Data/Output/output.jpg", testImage);

        displayImage(testImage, "Test Image", 2);
    }
}

/***************************************************************************************************
 * Get Current Image - Implementation
 *
 * Provides the current working directory as a string.
 *
 * Sourced from: https://www.tutorialspoint.com/find-out-the-current-working-directory-in-c-cplusplus
 *
 * @pre: None
 * @post: A string representing the working directory is populated and returned
 *
 * @return A string representing the working directory.
 **************************************************************************************************/
string get_current_dir()
{
    char buff[FILENAME_MAX]; //create string buffer to hold path
    GetCurrentDir( buff, FILENAME_MAX );
    string current_working_dir(buff);
    return current_working_dir;
}

/***************************************************************************************************
 * Display Image - Implementation
 *
 * @param image : The image to be displayed to the user.
 * @param windowName : The name of the window to display to
 * @param scale : The fraction of the image resolution that image will be scaled to
 *
 * Purpose:
 *
 * Provides a wrapper around opencv functions for creating a window, showing an image, waiting for
 * the user to press any key, and destroying the window. Allows the user to specify the window name
 * and the scale it will be displayed at.
 *
 * @pre: Image has been initialized and windowName is not already in use in another window.
 * @post: An image has been displayed to the user.
 *
 * @return None.
 **************************************************************************************************/
void displayImage(const Mat& image, const string& windowName, int scale)
{
    namedWindow(windowName,  WINDOW_NORMAL);
    resizeWindow(windowName, image.cols / scale, image.rows / scale);
    imshow(windowName, image);

    waitKey(0);
    destroyWindow(windowName);
}
