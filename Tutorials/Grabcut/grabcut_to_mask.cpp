/*
 * Convert a grabcutt-ed image to a greyscale mask where pixels with 0 values represent 
 * areas to ignore during the template matching process. 
 */

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;

Mat removeBackground(Mat grabcutImg, Mat result) {
	int pixelBVal, pixelGVal, pixelRVal;
	for (int r = 0; r < grabcutImg.rows; r++) {
		for (int c = 0; c < grabcutImg.cols; c++) {
			pixelBVal = grabcutImg.at<Vec3b>(r, c)[0];
			pixelGVal = grabcutImg.at<Vec3b>(r, c)[1];
			pixelRVal = grabcutImg.at<Vec3b>(r, c)[2];
			
			if (pixelBVal == 0 && pixelGVal == 0 && pixelRVal == 0) {
				result.at<uchar>(r, c) = 0;
			}
			else {
				result.at<uchar>(r, c) = 255;
			}
		}
	}
	return result;
}

int main(int argc, char* argv[]) {
	Mat grabcutImg = imread("Detection_Algorithm/Data/Templates/Mercy_mask.png");

	if (grabcutImg.empty()) {
		return 0;
	}

	imshow("Grab", grabcutImg);
	waitKey(0);
	Rect myRect(4, 4, grabcutImg.rows - 5, grabcutImg.cols - 5);
	Mat cropped = grabcutImg(myRect);
	imshow("Grab", cropped);
	waitKey(0);

	Mat temp = Mat(cropped.rows, cropped.cols, CV_8U);
	imshow("Temp", temp);
	waitKey(0);

	Mat result = removeBackground(cropped, temp);
	imshow("Show", result);
	imwrite("mask_grabcut_mercy.jpg", result);
	waitKey(0);
	return 1;
}
// x: 987
// y: 548
// w: 608
// h: 495