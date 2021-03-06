#include <iostream>
#include <string>
#include "opencv2/core/core.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
using namespace cv;
using namespace std;
Mat Q;
int main(int argc, char* argv[])
{
        Q=(Mat_<double>(4,4)<< 1., 0., 0., -2.9615028381347656e+02, 0., 1., 0., -2.3373317337036133e+02, 0.
    ,0., 0., 5.6446880931501073e+02 ,0., 0., -1.1340974198400260e-01,
4.1658568844268817e+00);
	Q.convertTo(Q, CV_32F);
	if (Q.cols != 4 || Q.rows != 4)
	{
		std::cerr << "ERROR: Q is not 4x4)" << std::endl;
		return 1;
	}

	// Loading disparity image
	cv::Mat disparity = cv::imread("/home/shivani/Opencv/disparity-image.pgm", cv::IMREAD_GRAYSCALE);
	if (disparity.empty())
	{
		std::cerr << "ERROR: Could not read disparity-image.pgm" << std::endl;
		return 1;
	}

	// Conversion of the disparity map to 32F before reprojecting to 3D
	// NOTE: also take care to do not scale twice the disparity
        namedWindow("Disparity Map",WINDOW_NORMAL);
        resizeWindow("Disparity Map",400,400);
        imshow("Disparity Map",disparity);
	disparity.convertTo(disparity, CV_32F, 1.0 / 16.0);
        // Reproject image to 3D by OpenCV
	cv::Mat image3D;
	cv::reprojectImageTo3D(disparity, image3D, Q, false, CV_32F);
        cv::Mat image(image3D.rows,image3D.cols,5);
        cv::Mat depth;
        float z[image3D.rows][image3D.cols];
           int k=0;
        for (int i = 0; i < image3D.rows; i++)
	{
		const cv::Vec3f* image3D_ptr = image3D.ptr<cv::Vec3f>(i);


		for (int j = 0; j < image3D.cols; j++)
		{      
                            image.at<float>(i,j)=image3D_ptr[j][2];
		}
	}
            
        normalize(image, depth, 0, 255, CV_MINMAX, CV_8U);
        namedWindow("Depth Map",WINDOW_NORMAL);
        resizeWindow("Depth Map",400,400);
        imshow("Depth Map",depth);
        
        waitKey(0);

	return 0;
}
