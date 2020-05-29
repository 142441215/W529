#include<opencv2/opencv.hpp>
#include<iostream>
using namespace cv;
using namespace std;
void XIXI(Mat & mag, Mat & angle, int bsize, float * hist, int bX, int bY, int b) {
	int b_PicX = bX * bsize, b_PicY = bY * bsize;
	for (int i = b_PicX; i < bsize + b_PicX; i++) {
		for (int j = b_PicY; j < bsize + b_PicY; j++) {
			int range = (angle.at<float>(j, i)) / 40;
			*(hist + range + b) += mag.at<float>(j, i);
		}
	}
}

void SOBELA(Mat & src, int bsize, int nX, int nY, float * hist) {
	Mat gx, gy;
	Mat mag, angle;
	Sobel(src, gx, CV_32F, 1, 0, 1);
	Sobel(src, gy, CV_32F, 0, 1, 1);
	cartToPolar(gx, gy, mag, angle, true);
	int b = 0;
	for (int i = 0; i < nY; i++) {
		for (int j = 0; j < nX; j++) {
			XIXI(mag, angle, bsize, hist, j, i, b);
			b += 9;
		}
	}
}

double QIUHE(float * a, float * b, int n) {
	double sum = 0;
	for (int i = 0; i < n; i++) {
		sum += abs(*(a + i) - *(b + i));
	}
	return sum;
}
int main() {
	Mat refMat = imread("c://project//template.png", 0);
	Mat test = imread("c://project//img.png", 0);
	int bsize = 12;
	int nX = refMat.cols / bsize;
	int nY = refMat.rows / bsize;
	int bins = 9 * nX*nY;
	double MinR = 1e30;
	int MinX;
	int MinY;
	int nHeight = test.rows;
	int nWidth = test.cols;
	int NHEIGHT = refMat.rows;
	int NWIDTH = refMat.cols;
	float * ref_hist = new float[bins];
	memset(ref_hist, 0, sizeof(float)*bins);
	SOBELA(refMat, bsize, nX, nY, ref_hist);
	for (int i = 0; i < nHeight - NHEIGHT + 1; i++)
	{
		for (int j = 0; j < nWidth - NWIDTH + 1; )
		{
			Rect rect(j, i, NWIDTH, NHEIGHT);
			Mat image_roi = test(rect);
			float * roi_hist = new float[bins];
			memset(roi_hist, 0, sizeof(float)*bins);
			SOBELA(image_roi, bsize, nX, nY, roi_hist);

			double HAHA = QIUHE(ref_hist, roi_hist, bins);
			if (HAHA < MinR)
			{
				MinR = HAHA;
				MinX = j;
				MinY = i;
			}
			delete[]  roi_hist;
			j = j + 5;
		}
		i = i + 5;
	}

	delete[] ref_hist;
	Rect rect(MinX, MinY, NWIDTH, NHEIGHT);
	rectangle(test, rect, Scalar(100,100, 100), 2, 8, 0);
	imshow("result", test);
	imwrite("result.png", test);
	waitKey(0);
	return 0;

}