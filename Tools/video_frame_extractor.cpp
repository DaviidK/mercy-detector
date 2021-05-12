/***************************************************************************************************
 * Video Frame Extractor
 *
 * @author Matthew Munson
 * @date 5/11/21
 *
 * This is a tool for extracting individual video frames from a video file. It can only be run
 * on OpenCV builds with MPEG4 support, which isn't included by default on Windows and Linux.
 * We plan to include instructions on how to build OpenCV with video support in the Documentation
 * directory of our repository.
 *
 * Configuration / Assumptions:
 *
 * - PATH_TO_VIDEO is the path to the video file you're breaking down
 * - SAVE_DIRECTORY_PATH is where a directory will be created and filled with frames
 * - IMAGE_FILE_EXTENSION is the type of image each frame will be saved as
 * - FRAME_SKIP is how many frames will be skipped between each saved image
 *
 * Configuration Notes:
 *
 * - Note that this program will create the directory to put images in automatically. The
 * directory is named as the captialized version of your input video file, following repo
 * naming conventions. If the directory already exists, the program will exit to prevent
 * overwrites.
 *
 * TODO: Test on a Windows build to see if the file system will allow directory creation
 *
 * - Because Overwatch video is recorded at 60 frames per second, even a short video will result
 * in a very high number of saved frames. Because this is saved in a very inefficient way of saving
 * frames, the processing may take minutes and the directory may be multiple Gigabytes in size.
 * A FRAME_SKIP value of 30 or more is recommended.
 *
 **************************************************************************************************/

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <sys/stat.h>
#include <sys/types.h>

using namespace std;
using namespace cv;

//__________________________________________________________________________________________________
// For now, inputs are simply constants that must be changed
// TODO: Convert to command line arguments

static const string PATH_TO_VIDEO = "Detection_Algorithm/Data/Video/Mercy/Wand/idle.mp4";
static const string SAVE_DIRECTORY_PATH = "Detection_Algorithm/Data/Output/";

static const string IMAGE_FILE_EXTENSION = ".png";
static const int FRAME_SKIP = 30; //How many frames to skip between saves

//__________________________________________________________________________________________________

void saveFrames(VideoCapture capture, const string& filename);

void createDirectory(const string& capitalized_filename);

string removePath(string path);

string capitalizeFirst(string filename);

/***************************************************************************************************
 * Main Function
 *
 * Program entry point. Opens the provided path to a video file. Extracts the filename and
 * capitalizes it for use as a directory name. Creates a directory with the capitalized filename
 * in at the SAVE_DIRECTORY_PATH location. Calls saveFrames() to save the video frames as
 * individual images.
 *
 * @pre: PATH_TO_VIDEO, SAVE_DIRECTORY_PATH, IMAGE_FILE_EXTENSION, and FRAME_SKIP initialized.
 * @post: Directory created and filled with individual frames from the provided video
 *
 * @return: Program exist code
 **************************************************************************************************/
int main()
{
    //Todo: Make this take input from the command line
    string directory = PATH_TO_VIDEO;

    VideoCapture capture = VideoCapture(directory);

    if(!capture.isOpened())
    {
        cout << "Could not open capture! Either the provided path is invalid, or your build \n" <<
        "of openCV does not support MPEG." << endl;

        return -1;
    }

    string filename = removePath(directory);
    string capitalized_filename = capitalizeFirst(filename);

    cout << "Successfully opened " << filename << endl;

    createDirectory(capitalized_filename);

    saveFrames(capture, capitalized_filename);
}

/***************************************************************************************************
 * Save Frames
 *
 * Loops through an initialized VideoCapture and retrieves each frame from the video. Saves a
 * frame, then skips FRAME_SKIP frames before saving another.
 *
 * @pre: capture is opened, capitalized_filename, SAVE_DIRECTORY_PATH, FRAME_SKIP initialized
 * @post: Frames saved to the SAVE_DIRECTORY_PATH/capitalized_filename
 **************************************************************************************************/
void saveFrames(VideoCapture capture, const string& capitalized_filename)
{
    Mat frame;
    string filename;

    string saveDirectory = SAVE_DIRECTORY_PATH + capitalized_filename + "/";
    int frameCount = 1;
    int frameSkipCounter = FRAME_SKIP + 1; //Initialize to capture first frame

    cout << "Writing all frames to the created directory" << endl;

    while(true)
    {
        capture >> frame;

        if(frame.empty())
            break;

        if(frameSkipCounter > FRAME_SKIP)
        {
            filename = "frame_" + to_string(frameCount) + IMAGE_FILE_EXTENSION;

            imwrite(saveDirectory + filename, frame);

            frameSkipCounter = 0;
        }

        frameCount++;
        frameSkipCounter++;
    }

    cout << "Finished Writing to " << saveDirectory << endl;
}

/***************************************************************************************************
 * Create Directory
 *
 * Creates a directory at the SAVE_DIRECTORY_PATH/capitalized_filename. If directory creation fails
 * (likely because the directory name already exists) the function calls exit() and forces the
 * program to quit.
 *
 * //TODO test this function specifically on Windows
 *
 *
 * @pre: SAVE_DIRECTORY_PATH, capitalized_filename initialized
 * @post: A directory at SAVE_DIRECTORY_PATH/capitalized_filename has been created
 **************************************************************************************************/
void createDirectory(const string& capitalized_filename)
{
    int result = mkdir((SAVE_DIRECTORY_PATH + capitalized_filename).c_str(), 0777);

    if(result != 0)
    {
        cout << "Error: Couldn't create the directory: "
             << SAVE_DIRECTORY_PATH + capitalized_filename << endl;

        cout << "This is probably because it already exists. " << endl;

        exit(-1);
    }

    cout << "Successfully created: " << SAVE_DIRECTORY_PATH + capitalized_filename << endl;
}

/***************************************************************************************************
 * Remove Path
 *
 * Extracts a filename from a given path. Removes both the preceding directories and the file
 * extension.
 *
 * Source: https://stackoverflow.com/questions/8520560/get-a-file-name-from-a-path
 *
 * @pre: path is a valid string
 * @post: The path before the filename is removed as well as the file extension.
 *
 * @return A copy of the provided filename with no path or extension included.
 **************************************************************************************************/
string removePath(string path)
{
    // Remove directory if present.
    // Do this before extension removal in case directory has a period character.
    const size_t last_slash_idx = path.find_last_of("\\/");
    if (string::npos != last_slash_idx)
    {
        path.erase(0, last_slash_idx + 1);
    }

    // Remove extension if present.
    const size_t period_idx = path.rfind('.');
    if (string::npos != period_idx)
    {
        path.erase(period_idx);
    }

    return path;
}

/***************************************************************************************************
 * Capitalize First
 *
 * Capitalizes the first letter in a string. Fails if the first index of the provided string is
 * below ASCII character 96, which is where lowercase letters start. In this case, the original
 * string is returned.
 *
 * @pre: filename is a valid string
 * @post: The first character in filename is capitalized, or the filename is unchanged
 *
 * @return A copy of filename with the first character capitalized
 **************************************************************************************************/
string capitalizeFirst(string filename)
{
    if(filename[0] < 96)
    {
        cout << "Can't capitalize: " << filename;

        return filename;
    }

    filename[0] -= 32; //32 is the difference between ASCII lowercase 'a' and uppercase 'A'
    return filename;
}
