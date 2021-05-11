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
	Mat grabcutImg = imread("Detection_Algorithm/Data/Static_Test_Im/template_grabcut_mercy.jpg");
	imshow("Grab", grabcutImg);
	waitKey(0);

	Mat temp = Mat(grabcutImg.rows, grabcutImg.cols, CV_8U);
	imshow("Temp", temp);
	waitKey(0);

	Mat result = removeBackground(grabcutImg, temp);
	imshow("Show", result);
	imwrite("mask_grabcut_mercy.jpg", result);
	waitKey(0);
}
